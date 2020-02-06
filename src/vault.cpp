#include "vault.h"

namespace CryptedVault::Vault
{
    std::vector<uint8_t> readVaultFile(const std::string &path)
    {
        std::ifstream stream(path, std::ios::binary | std::ios::ate);

        if (!stream || !stream.is_open()) 
        {
            throw CryptedVaultException<Error>(Error::CannotOpenVault, "Check if file exists or if it is in use");
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);

        std::size_t size = std::size_t(end - stream.tellg());

        if (size == 0) 
        {
            throw CryptedVaultException<Error>(Error::EmptyVault);
        }

        if (size < vaultFileMinSize)
        {
            throw CryptedVaultException<Error>(Error::VaultFileMinSize);
        }

        std::vector<uint8_t> blob(size);

        if (!stream.read((char*)blob.data(), blob.size())) throw std::runtime_error("");

        return blob;
    }
}
