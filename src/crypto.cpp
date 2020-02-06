#include "crypto.h"

namespace CryptedVault::CryptoUtils
{
    std::vector<uint8_t> SHA256(std::string_view str)
    {
        std::vector<uint8_t> ptr(SHA256_DIGEST_LENGTH);
        ::SHA256((unsigned char *) str.data(), str.length(), ptr.data());
        return ptr;
    }

    std::vector<uint8_t> MD5(std::string_view str)
    {
        std::vector<uint8_t> ptr(MD5_DIGEST_LENGTH);
        ::MD5((unsigned char *) str.data(), str.length(), ptr.data());
        return ptr;
    }

    std::vector<uint8_t> EncryptAES256(std::vector<uint8_t> data, std::string_view key)
    {
        std::vector<uint8_t> encryptedData(data.size() + 16);
        std::vector<uint8_t> hashedKey = SHA256(key);
        std::vector<uint8_t> initializationVector = MD5(key);
        int len1, len2;

        cipher_ctx_uptr ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

        if (!EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, hashedKey.data(), initializationVector.data()))
        {
            throw CryptoException("Cannot setup AES encryption", EncryptionError::AESInitialization);
        }

        if (!EVP_EncryptUpdate(ctx.get(), encryptedData.data(), &len1, data.data(), data.size()))
        {
            throw CryptoException("Cannot perform AES encryption", EncryptionError::AESUpdate);
        }

        if (!EVP_EncryptFinal_ex(ctx.get(), encryptedData.data() + len1, &len2))
        {
            throw CryptoException("Cannot finalize AES encryption", EncryptionError::AESFinalize);
        }

        encryptedData.resize(len1 + len2);

        return encryptedData;
    }
    
    std::vector<uint8_t> DecryptAES256(std::vector<uint8_t> data, std::string_view key)
    {
        std::vector<uint8_t> decryptedData(data.size());
        std::vector<uint8_t> hashedKey = SHA256(key);
        std::vector<uint8_t> initializationVector = MD5(key);
        int len1, len2;

        cipher_ctx_uptr ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

        if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, hashedKey.data(), initializationVector.data()))
        {
            throw CryptoException("Cannot initialize AES decryption", EncryptionError::AESInitialization);
        }

        if (!EVP_DecryptUpdate(ctx.get(), decryptedData.data(), &len1, data.data(), data.size()))
        {
            throw CryptoException("Cannot perform AES decryption", EncryptionError::AESUpdate);
        }

        if (!EVP_DecryptFinal_ex(ctx.get(), decryptedData.data() + len1, &len2))
        {
            throw CryptoException("Cannot finalize AES decryption", EncryptionError::AESFinalize);
        }

        decryptedData.resize(len1 + len2);

        return decryptedData;    
    }
}
