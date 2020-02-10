#include "vault.h"

namespace CryptedVault::Vault
{
    namespace
    {
        size_t writeDataMemberHelper(std::vector<uint8_t> &data, std::string_view member, size_t offset)
        {
            data.resize(data.size() + sizeof(size_t) + member.length());
            auto ptrBase = data.data();
            auto ptr = ptrBase + offset;
            size_t length = member.length();
            std::memcpy(ptr, &length, sizeof(size_t));
            ptr += sizeof(size_t);
            std::memcpy(ptr, member.data(), member.length());
            ptr += member.length();
            return (ptr - ptrBase);
        }

        LoginCollection parseDataBlock(const std::vector<uint8_t> &data, uint16_t entries)
        {
            LoginCollection collection(entries);

            auto ptr = data.data();
            for (int i = 0; i < entries; i++)
            {
                size_t length;
                // Domain
                std::memcpy(&length, ptr, sizeof(size_t));
                collection[i].domain.resize(length);
                ptr += sizeof(size_t);
                std::memcpy(collection[i].domain.data(), ptr, length);
                ptr += length;

                // User
                std::memcpy(&length, ptr, sizeof(size_t));
                collection[i].user.resize(length);
                ptr += sizeof(size_t);
                std::memcpy(collection[i].user.data(), ptr, length);
                ptr += length;

                // Password
                std::memcpy(&length, ptr, sizeof(size_t));
                collection[i].password.resize(length);
                ptr += sizeof(size_t);
                std::memcpy(collection[i].password.data(), ptr, length);
                ptr += length;

                // Comment
                std::memcpy(&length, ptr, sizeof(size_t));
                collection[i].comment.resize(length);
                ptr += sizeof(size_t);
                std::memcpy(collection[i].comment.data(), ptr, length);
                ptr += length;
            }
            

            return collection;
        }
    }

    LoginCollection readVaultFile(const std::string &path, std::string_view key)
    {
        std::ifstream stream(path, std::ios::binary | std::ios::ate);

        if (!stream || !stream.is_open()) 
        {
            throw CryptedVaultException<Error>(Error::CannotOpenVault, "Check if file exists or if it is in use");
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);

        std::size_t size = std::size_t(end - stream.tellg());

        if (size == 0) 
        {
            throw CryptedVaultException<Error>(Error::EmptyVault);
        }

        if (size < vaultFileMinSize)
        {
            throw CryptedVaultException<Error>(Error::VaultFileMinSize);
        }

        VaultFile vault;

        if (!stream.read((char*)&vault, vaultFileMinSize))
        {
            throw CryptedVaultException<Error>(Error::CannotReadVault, "Cannot read primary block");
        }
        
        if (vault.magic != vaultMagicNumber)
        {
            throw CryptedVaultException<Error>(Error::WrongFile);
        }

        if (vault.versionMajor != vaultVersionMajor || vault.versionMinor != vaultVersionMinor)
        {
            throw CryptedVaultException<Error>(
                Error::WrongVersion, 
                "File version is " + std::to_string(vault.versionMajor) + "." + std::to_string(vault.versionMinor) +
                " and program version is " + std::to_string(vaultVersionMajor) + "." + std::to_string(vaultVersionMinor));
        }

        std::vector<uint8_t> bucket(size - vaultFileMinSize);
        if (!stream.read((char*)bucket.data(), size - vaultFileMinSize))
        {
            throw CryptedVaultException<Error>(Error::CannotReadVault, "Cannot read data block");
        }

        if (vault.md5 != *reinterpret_cast<__uint128_t *>(CryptoUtils::MD5(bucket).data()))
        {
            throw CryptedVaultException<Error>(Error::DataCorrupted);
        }

        std::vector<uint8_t> dataDecrypted = CryptoUtils::DecryptAES256(bucket, key);

        return parseDataBlock(dataDecrypted, vault.entries);;
    }

    void writeVaultFile(const std::string &path, LoginCollection &logins, std::string_view key)
    {
        std::ofstream stream(path, std::ios::binary | std::ios::trunc);

        std::vector<uint8_t> data;
        size_t offset = 0;
        for (size_t i = 0; i < logins.size(); i++)
        {
            offset = writeDataMemberHelper(data, logins[i].domain, offset);
            offset = writeDataMemberHelper(data, logins[i].user, offset);
            offset = writeDataMemberHelper(data, logins[i].password, offset);
            offset = writeDataMemberHelper(data, logins[i].comment, offset);
        }
        std::vector<uint8_t> dataEncrypted = CryptoUtils::EncryptAES256(data, key);
        std::vector<uint8_t> md5 = CryptoUtils::MD5(dataEncrypted);

        VaultFile vault;
        vault.magic = vaultMagicNumber;
        vault.versionMajor = vaultVersionMajor;
        vault.versionMinor = vault.versionMinor;
        vault.md5 = *reinterpret_cast<__int128_t *>(md5.data());
        vault.entries = logins.size();

        stream.write(reinterpret_cast<char *>(&vault), vaultFileMinSize);
        stream.write(reinterpret_cast<char *>(dataEncrypted.data()), dataEncrypted.size());

        stream.close();
    }
}
