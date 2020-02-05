#include "crypto.h"

#include <iostream>
namespace CryptedVault::CryptoUtils
{
    std::vector<uint8_t> SHA256(std::string_view str)
    {
        std::vector<uint8_t> ptr;
        ptr.reserve(SHA256_DIGEST_LENGTH);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.data(), str.size());
        SHA256_Final(ptr.data(), &sha256);
        return ptr;
    }

    std::vector<uint8_t> EncryptAES256(std::vector<uint8_t> data, std::string_view key)
    {
        std::vector<uint8_t> encryptedData;
        std::vector<uint8_t> hashedKey = SHA256(key);

        std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>  ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

        int out_len1, out_len2;

        if (!ctx)
            throw std::runtime_error("Cannot initialise AES context");

        unsigned char *iv = (unsigned char *)"0123456789012345";
        if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, hashedKey.data(), iv) != 1)
            throw std::runtime_error("Cannot setup AES encryption");


        encryptedData.resize(data.size() + 128);
        if (EVP_EncryptUpdate(ctx.get(), encryptedData.data(), &out_len1, data.data(), data.size()) != 1)
            throw std::runtime_error("Cannot perform AES encryption");

        if (EVP_EncryptFinal_ex(ctx.get(), encryptedData.data() + out_len1, &out_len2) != 1)
            throw std::runtime_error("Cannot finalize AES encryption");

        encryptedData.resize(out_len1 + out_len2);

        return encryptedData;
    }
    
    std::vector<uint8_t> DecryptAES256(std::vector<uint8_t> data, std::string_view key)
    {
        std::vector<uint8_t> decryptedData;
        std::vector<uint8_t> hashedKey = SHA256(key);

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

        int out_len1, out_len2;
        int plaintext_len;

        if(!ctx)
            throw std::runtime_error("Cannot initialise AES context");

        unsigned char *iv = (unsigned char *)"0123456789012345";
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, hashedKey.data(), iv))
            throw std::runtime_error("Cannot setup AES decryption");

        decryptedData.resize(data.size());
        if (EVP_DecryptUpdate(ctx, decryptedData.data(), &out_len1, data.data(), data.size()) != 1)
            throw std::runtime_error("Cannot perform AES decryption");

        if (EVP_DecryptFinal_ex(ctx, decryptedData.data() + out_len1, &out_len2) != 1)
            throw std::runtime_error("Cannot finalize AES decryption");

        EVP_CIPHER_CTX_free(ctx);

        decryptedData.resize(out_len1 + out_len2);

        return decryptedData;    
    }
}