#include <wx/wxprec.h>
#include <wx/wx.h>

namespace CryptedVault::UI
{
    class Window : public wxFrame
    {
        public:
            Window();
        private:
            void OnHello(wxCommandEvent& event);
            void OnExit(wxCommandEvent& event);
            void OnAbout(wxCommandEvent& event);
    };
}
