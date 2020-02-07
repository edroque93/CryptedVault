#include <iostream>
#include <iomanip>

#include "crypto.h"
#include "vault.h"

int main()
{
    CryptedVault::Vault::LoginCollection login 
    { 
        CryptedVault::Vault::LoginEntry 
        {
            "hotmail.com", 
            "pepe", 
            "1234", 
            "meh" 
        },
        CryptedVault::Vault::LoginEntry 
        {
            "gmail.com", 
            "molaaaÄ", 
            "asdfghijkAAäËöA@@áéññlm", 
            "" 
        },
    };

    CryptedVault::Vault::writeVaultFile("/home/quique/Documents/Workspace/CryptedVault/bin/demo", login, "mypassword");
    auto x = CryptedVault::Vault::readVaultFile("/home/quique/Documents/Workspace/CryptedVault/bin/demo", "mypassword");

    return 0;
}
