#include "main.h"

namespace CryptedVault
{
    void playground()
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
        auto decoded = CryptedVault::Vault::readVaultFile("/home/quique/Documents/Workspace/CryptedVault/bin/demo", "mypassword");
    }

    bool CryptedVaultApp::OnInit()
    {
        playground();

        UI::Window *frame = new UI::Window();
        frame->Show(true);
        return true;
    }
}