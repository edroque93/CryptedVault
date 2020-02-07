#include "vault.h"

namespace CryptedVault::Vault
{
    namespace
    {
        size_t writeDataMemberHelper(std::vector<uint8_t> &data, std::string &member, size_t offset)
        {
            data.resize(data.size() + sizeof(size_t) + member.length());
            auto ptrBase = data.data();
            auto ptr = ptrBase + offset;
            size_t length = member.length();
            std::memcpy(ptr, &length, sizeof(size_t));
            ptr += sizeof(size_t);
            std::copy(member.begin(), member.end(), ptr);
            ptr += member.length();
            return (ptr - ptrBase);
        }

        std::vector<uint8_t> dumpData(VaultFile &vault)
        {
            std::vector<uint8_t> data;
            size_t offset = 0;
            for (LoginEntry &entry : vault.data)
            {
                offset = writeDataMemberHelper(data, entry.domain, offset);
                offset = writeDataMemberHelper(data, entry.user, offset);
                offset = writeDataMemberHelper(data, entry.password, offset);
                offset = writeDataMemberHelper(data, entry.comment, offset);
            }
            return data;
        }

        void readDataStreamMemberHelper(std::ifstream &stream, std::string &member)
        {
            size_t length;

            if (!stream.read((char*)&length, sizeof(length)))
            {
                throw CryptedVaultException<Error>(Error::CannotReadVault, "Cannot read data block length");
            }
            member.resize(length);
            if (!stream.read((char*)member.data(), length))
            {
                throw CryptedVaultException<Error>(Error::CannotReadVault, "Cannot read data block member data");
            }
        }
    }

    VaultFile readVaultFile(const std::string &path)
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

        vault.data.resize(vault.entries);

        for (size_t i = 0; i < vault.entries; i++)
        {
            readDataStreamMemberHelper(stream, vault.data[i].domain);
            readDataStreamMemberHelper(stream, vault.data[i].user);
            readDataStreamMemberHelper(stream, vault.data[i].password);
            readDataStreamMemberHelper(stream, vault.data[i].comment);
        }

        std::vector<uint8_t> bucket = dumpData(vault);
        if (vault.md5 != *reinterpret_cast<__uint128_t *>(CryptoUtils::MD5(bucket).data()))
        {
            throw CryptedVaultException<Error>(Error::DataCorrupted);
        }

        return vault;
    }

    void writeVaultFile(const std::string &path, std::vector<LoginEntry> &logins)
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
        std::vector<uint8_t> md5 = CryptoUtils::MD5(data);

        VaultFile vault;
        vault.magic = vaultMagicNumber;
        vault.versionMajor = vaultVersionMajor;
        vault.versionMinor = vault.versionMinor;
        vault.md5 = *reinterpret_cast<__int128_t *>(md5.data());
        vault.entries = logins.size();

        stream.write(reinterpret_cast<char *>(&vault), vaultFileMinSize);
        stream.write(reinterpret_cast<char *>(data.data()), data.size());

        stream.close();
    }
}
