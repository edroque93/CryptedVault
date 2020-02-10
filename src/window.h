#include <wx/wxprec.h>
#include <wx/wx.h>

#include "vault.h"

namespace CryptedVault::UI
{
    class Window : public wxFrame
    {
        public:
            Window();
        private:
            void buildMenu();
            void buildRow();
            void openFileCmd(wxCommandEvent &event);
            
            wxBoxSizer *mainSizer;
            Vault::LoginCollection logins;
    };
}
