#include <wx/wx.h>
#include <iostream>

#include "crypto.h"
#include "vault.h"
#include "window.h"

namespace CryptedVault
{
    class CryptedVaultApp : public wxApp
    {
        public:
            virtual bool OnInit();
    };
}

wxIMPLEMENT_APP(CryptedVault::CryptedVaultApp);
