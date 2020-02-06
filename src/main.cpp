#include <iostream>
#include <iomanip>

#include "crypto.h"
#include "vault.h"

int main()
{
    auto t = CryptedVault::CryptoUtils::MD5("");
    for (auto const &a: t)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)a << " ";

    std::cout << std::endl;
    std::string message = "¡Hola mundo!";
    std::vector<uint8_t> input(message.begin(), message.end());
    auto dataE = CryptedVault::CryptoUtils::EncryptAES256(input, "MyPassword123@~%$!!");
    auto dataD = CryptedVault::CryptoUtils::DecryptAES256(dataE, "MyPassword123@~%$!!");

    std::cout << "original:\t";
    for (auto const &byteI: input)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)byteI << " ";
    std::cout << std::endl << "encrypted:\t";
    for (auto const &byteE: dataE)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)byteE << " ";
    std::cout << std::endl << "decrypted:\t";
    for (auto const &byteD: dataD)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)byteD << " ";
    std::cout << std::endl;

    auto xxx = CryptedVault::Vault::readVaultFile("/home/quique/Documents/Workspace/CryptedVault/bin/demo");

    return 0;
}
