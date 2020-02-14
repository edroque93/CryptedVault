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
    static const uint16_t vaultVersion = 0x0002;
    
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
        uint16_t version;
        __uint128_t md5;
        uint16_t entries;
        std::vector<LoginEntry> data;
    };

    #pragma pack(pop)

    static const uint32_t vaultFileMinSize = sizeof(VaultFile) - sizeof(VaultFile::data);

    LoginCollection readVaultFile(const std::string &path, std::string_view key);
    void writeVaultFile(const std::string &path, LoginCollection &logins, std::string_view key);
}
