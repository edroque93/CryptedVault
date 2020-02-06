#pragma once

#include <string>
#include <vector>

namespace CryptedVault::Vault
{
    #pragma pack(push, 1)

    struct VaultFile
    {
        uint16_t magic;
        uint32_t version;
        __uint128_t md5;
        uint8_t *data;
    };

    struct LoginEntryRaw
    {
        uint32_t length;
        uint8_t *data;
    };

    #pragma pack(pop)

    struct LoginEntry
    {
        std::string domain;
        std::string user;
        std::string password;
    };

    typedef std::vector<LoginEntry> LoginCollection;

}