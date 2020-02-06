#include "crypto.h"

namespace CryptedVault::CryptoUtils
{
    std::vector<uint8_t> SHA256(std::vector<uint8_t>  data)
    {
        std::vector<uint8_t> ptr(SHA256_DIGEST_LENGTH);
        ::SHA256(data.data(), data.size(), ptr.data());
        return ptr;
    }

    std::vector<uint8_t> MD5(std::vector<uint8_t> data)
    {
        std::vector<uint8_t> ptr(MD5_DIGEST_LENGTH);
        ::MD5(data.data(), data.size(), ptr.data());
        return ptr;
    }

    std::vector<uint8_t> SHA256(std::string_view str) 
    { 
        return SHA256(std::vector<uint8_t>(str.begin(), str.end())); 
    }

    std::vector<uint8_t> MD5(std::string_view str) 
    { 
        return MD5(std::vector<uint8_t>(str.begin(), str.end())); 
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
            throw CryptedVaultException<Error>(Error::AESInitialization, "AES encryption");
        }

        if (!EVP_EncryptUpdate(ctx.get(), encryptedData.data(), &len1, data.data(), data.size()))
        {
            throw CryptedVaultException<Error>(Error::AESUpdate, "AES encryption");
        }

        if (!EVP_EncryptFinal_ex(ctx.get(), encryptedData.data() + len1, &len2))
        {
            throw CryptedVaultException<Error>(Error::AESFinalize, "AES encryption");
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
            throw CryptedVaultException<Error>(Error::AESInitialization, "AES decryption");
        }

        if (!EVP_DecryptUpdate(ctx.get(), decryptedData.data(), &len1, data.data(), data.size()))
        {
            throw CryptedVaultException<Error>(Error::AESUpdate, "AES decryption");
        }

        if (!EVP_DecryptFinal_ex(ctx.get(), decryptedData.data() + len1, &len2))
        {
            throw CryptedVaultException<Error>(Error::AESFinalize, "AES decryption");
        }

        decryptedData.resize(len1 + len2);

        return decryptedData;    
    }
}
