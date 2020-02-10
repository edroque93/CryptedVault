#include "window.h"

namespace CryptedVault::UI
{    
    Window::Window() 
        : wxFrame(
            nullptr, 
            wxID_ANY, 
            "CryptedVault",
            wxDefaultPosition,
            wxDefaultSize,
            wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX)
    {
        SetMinSize(wxSize(500, 200));
        buildMenu();

        mainSizer = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer *options = new wxBoxSizer(wxHORIZONTAL);
        options->Add(new wxButton(this, wxID_ANY, "Add"), 0, wxALL, 10);
        mainSizer->Add(options, 0, wxALIGN_RIGHT);

        SetSizer(mainSizer);
    }

    void Window::buildRow()
    {
        wxBoxSizer *rowBox = new wxBoxSizer(wxHORIZONTAL);
        
        rowBox->Add(new wxTextCtrl(this, wxID_ANY), 1, wxEXPAND | wxALL, 10);

        mainSizer->Add(rowBox, 0, wxALIGN_CENTER);
    }

    void Window::buildMenu()
    {
        wxMenu *menuFile = new wxMenu;
        menuFile->Append(1, "&Open...\tCtrl-O", "Open vault file");
        menuFile->AppendSeparator();
        menuFile->Append(wxID_EXIT);
        wxMenu *menuHelp = new wxMenu;
        menuHelp->Append(wxID_ABOUT);
        wxMenuBar *menuBar = new wxMenuBar;
        menuBar->Append(menuFile, "&File");
        menuBar->Append(menuHelp, "&Help");
        SetMenuBar( menuBar );

        Bind(wxEVT_MENU, &Window::openFileCmd, this, wxID_ANY);
        Bind(wxEVT_MENU, 
            [](wxCommandEvent &event) { 
                wxMessageBox("Store your passwords in a safe vault!", "About", wxOK | wxICON_INFORMATION); 
            }, 
            wxID_ABOUT);
        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { Close(true); }, wxID_EXIT);
    }

    void Window::openFileCmd(wxCommandEvent &event)
    {
        wxFileDialog openFileDialog(this, "Open vault file", "", "", "CVF files (*.cvf)|*.cvf|Any file (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (openFileDialog.ShowModal() == wxID_CANCEL) return;

        wxTextEntryDialog dlg(this, "Enter file password", "Password", wxEmptyString, wxTextEntryDialogStyle | wxTE_PASSWORD);
        if (dlg.ShowModal() == wxID_CANCEL) return;

        try
        {
            logins = Vault::readVaultFile(openFileDialog.GetPath().ToStdString(), dlg.GetValue().ToStdString());
        }
        catch(const CryptedVaultException<CryptoUtils::Error>& e)
        {
            wxMessageBox("Incorrect password. Choose the correct file and try again.", "Error", wxOK | wxICON_ERROR, this);
        }
        catch (...)
        {
            wxMessageBox("Unexpected error", "Error", wxOK | wxICON_ERROR, this);
        }
    }
}