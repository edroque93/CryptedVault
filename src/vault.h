#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "cvexception.h"

namespace CryptedVault::Vault
{
    enum class Error
    {
        CannotOpenVault,
        EmptyVault,
        VaultFileMinSize,
    };

    static std::ostream& operator<< (std::ostream& os, Error error)
    {
        switch (error)
        {
            case Error::CannotOpenVault : return os << "Vault file cannot be opened" ;
            case Error::EmptyVault : return os << "Vault file is empty" ;
            case Error::VaultFileMinSize : return os << "Vault file is too small" ;
        };
        return os << static_cast<uint32_t>(error);
    }

    static const uint32_t vaultMagicNumber = 0xCA9320FE;
    static const uint32_t currentVerMajor = 0x0001;
    static const uint32_t currentVerMinor = 0x0000;

    #pragma pack(push, 1)

    struct VaultFile
    {
        uint32_t magic;
        uint16_t versionMajor;
        uint16_t versionMinor;
        __uint128_t md5;
        uint8_t *data;
    };

    static const uint32_t vaultFileMinSize = sizeof(VaultFile) - sizeof(uint8_t);

    struct LoginEntryRaw
    {
        uint16_t domainLength;
        uint16_t userLength;
        uint16_t passwordLength;
        uint16_t commentLength;
        char *domain;
        char *user;
        char *password;
    };

    #pragma pack(pop)

    struct LoginEntry
    {
        std::string domain;
        std::string user;
        std::string password;
        std::string comment;
    };

    typedef std::vector<LoginEntry> LoginCollection;

    std::vector<uint8_t> readVaultFile(const std::string &path);
}
