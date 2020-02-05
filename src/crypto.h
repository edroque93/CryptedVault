#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <string_view>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

namespace CryptedVault::CryptoUtils
{
    std::vector<uint8_t> SHA256(std::string_view str);

    std::vector<uint8_t> EncryptAES256(std::vector<uint8_t> data, std::string_view key);
    std::vector<uint8_t> DecryptAES256(std::vector<uint8_t> data, std::string_view key);
}