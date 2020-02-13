#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/stattext.h>
#include <wx/artprov.h>
#include <chrono>
#include <string>
#include <algorithm>

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
            void openVaultFile();
            void createVaultFile();
            void saveVaultFile();
            wxBoxSizer *buildHeader();
            void testEvent(wxMouseEvent &event);
            void mouseOverLabels(wxMouseEvent &event);
            void mouseOverFrame(wxMouseEvent &event);
            void setReadOnly(bool value, bool setUI = true);
            void addEntry(std::string domain, std::string user, std::string password, std::string comment);

            struct uiEntryData;

            std::string currentVaultPath;
            std::string currentVaultPassword;
            Vault::LoginCollection logins;
            std::vector<uiEntryData> uiEntries;
            wxBoxSizer *mainSizer;
            wxScrolledWindow *scrolledWindow;
            wxCheckBox *readOnly;
            bool handCursorWasEnabled;

            enum ctrlID {
                ID_DOMAIN = 0x1000,
                ID_USER,
                ID_PASSWORD,
                ID_COMMENT,
            };

            struct uiEntryData
            {
                wxTextCtrl *domain, *user, *password, *comment;
                
                bool operator==(const struct uiEntryData& o) const
                {
                    return 
                        o.domain == this->domain && 
                        o.user == this->user && 
                        o.password == this->password && 
                        o.comment == this->comment;
                }         
            };
    };
    
}
