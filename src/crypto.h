#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <string_view>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

#include "cvexception.h"

namespace CryptedVault::CryptoUtils
{
    enum class Error
    {
        AESInitialization,
        AESUpdate,
        AESFinalize,
    };

    static std::ostream& operator<< (std::ostream& os, Error error)
    {
        switch (error)
        {
            case Error::AESInitialization : return os << "Cannot initialize AES" ;
            case Error::AESUpdate : return os << "Cannot update AES" ;
            case Error::AESFinalize : return os << "Cannot finalize AES" ;
        };
        return os << static_cast<uint32_t>(error);
    }

    typedef std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> cipher_ctx_uptr;

    std::vector<uint8_t> SHA256(std::vector<uint8_t> str);
    std::vector<uint8_t> SHA256(std::string_view str);
    std::vector<uint8_t> MD5(std::vector<uint8_t>str);
    std::vector<uint8_t> MD5(std::string_view str);

    std::vector<uint8_t> EncryptAES256(std::vector<uint8_t> data, std::string_view key);
    std::vector<uint8_t> DecryptAES256(std::vector<uint8_t> data, std::string_view key);
}
