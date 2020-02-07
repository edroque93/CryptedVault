#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <fstream>

#include "crypto.h"
#include "cvexception.h"

namespace CryptedVault::Vault
{
    enum class Error
    {
        CannotOpenVault,
        EmptyVault,
        VaultFileMinSize,
        CannotReadVault,
        WrongFile,
        WrongVersion,
        DataCorrupted,
    };

    static std::ostream& operator<< (std::ostream& stream, Error error)
    {
        switch (error)
        {
            case Error::CannotOpenVault : return stream << "Vault file cannot be opened";
            case Error::EmptyVault : return stream << "Vault file is empty";
            case Error::VaultFileMinSize : return stream << "Vault file is too small";
            case Error::CannotReadVault : return stream << "Cannot read vault file";
            case Error::WrongFile : return stream << "Wrong file detected";
            case Error::WrongVersion : return stream << "Wrong file version detected";
            case Error::DataCorrupted : return stream << "Data is corrupted";
        };
        return stream << static_cast<uint32_t>(error);
    }

    static const uint32_t vaultMagicNumber = 0xCA9320FE;
    static const uint16_t vaultVersionMajor = 0x0001;
    static const uint16_t vaultVersionMinor = 0x0000;
    
    struct LoginEntry
    {
        std::string domain;
        std::string user;
        std::string password;
        std::string comment;
    };

    typedef std::vector<LoginEntry> LoginCollection;

    #pragma pack(push, 1)

    struct VaultFile
    {
        uint32_t magic;
        uint16_t versionMajor;
        uint16_t versionMinor;
        __uint128_t md5;
        uint16_t entries;
        std::vector<LoginEntry> data;
    };

    #pragma pack(pop)

    static const uint32_t vaultFileMinSize = sizeof(VaultFile) - sizeof(VaultFile::data);

    static std::ostream& operator<< (std::ostream& stream, LoginEntry entry)
    {
        stream << entry.domain.length() << entry.domain;
        stream << entry.user.length() << entry.user;
        stream << entry.password.length() << entry.password;
        stream << entry.comment.length() << entry.comment;
        return stream;
    }

    static std::ostream& operator<< (std::ostream& stream, VaultFile vault)
    {
        stream << std::hex 
            << vaultMagicNumber
            << vaultVersionMajor
            << vaultVersionMinor
            << static_cast<uint64_t>(vault.md5 >> 64)
            << static_cast<uint64_t>(vault.md5)
            << vault.entries;
        for (size_t i = 0; i < vault.entries; i++)
        {
            stream << vault.data[i];
        }
        return stream;
    }

    VaultFile readVaultFile(const std::string &path);
    void writeVaultFile(const std::string &path, std::vector<LoginEntry> &logins);
}
