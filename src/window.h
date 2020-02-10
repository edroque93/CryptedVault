#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/grid.h>

#include "vault.h"

namespace CryptedVault::UI
{
    class Window : public wxFrame
    {
        public:
            Window();
        private:
            void buildMenu();
            void generateVaultView();
            void generateEmptyView();
            void openFileCmd(wxCommandEvent &event);
            
            wxBoxSizer *mainSizer;
            Vault::LoginCollection logins;
    };
}
