#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>

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
            void OpenVaultFile();
            void CreateVaultFile();
            void testEvent(wxMouseEvent &event);
            void mouseOverLabels(wxMouseEvent &event);
            void mouseOverFrame(wxMouseEvent &event);

            std::string currentVaultPath;
            std::string currentVaultPassword;
            Vault::LoginCollection logins;
            wxBoxSizer *mainSizer;
            bool handCursorWasEnabled;
    };
}
