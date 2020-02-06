#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <string_view>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

namespace CryptedVault::CryptoUtils
{
    enum class EncryptionError
    {
        AESInitialization,
        AESUpdate,
        AESFinalize,
    };

    class CryptoException : public std::exception
    {
        public:
            CryptoException(std::string_view message, EncryptionError code) 
                : message(message)
                , code(code) {}

            const char *what() const throw ()
            {
                return message.c_str();
            }

            EncryptionError getCode() { return code; }

        private:
            std::string message;
            EncryptionError code;
    };

    typedef std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> cipher_ctx_uptr;

    std::vector<uint8_t> SHA256(std::string_view str);
    std::vector<uint8_t> MD5(std::string_view str);

    std::vector<uint8_t> EncryptAES256(std::vector<uint8_t> data, std::string_view key);
    std::vector<uint8_t> DecryptAES256(std::vector<uint8_t> data, std::string_view key);
}
