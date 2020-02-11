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
            wxDEFAULT_FRAME_STYLE & ~(wxMAXIMIZE_BOX | wxRESIZE_BORDER))
        , mainSizer(nullptr)
        , handCursorWasEnabled(false)
    {
        SetMinSize(wxSize(700, 350));
        buildMenu();

        mainSizer = new wxBoxSizer(wxVERTICAL);

        generateVaultView();
        // generateEmptyView();

        Bind(wxEVT_MOTION, &Window::mouseOverFrame, this, wxID_ANY);

        SetSizer(mainSizer);
    }

    void Window::generateEmptyView()
    {
        mainSizer->Clear(true);

        wxBoxSizer *textSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText *text1 = new wxStaticText(this, wxID_ANY, "Open");
        wxStaticText *text2 = new wxStaticText(this, wxID_ANY, " or ");
        wxStaticText *text3 = new wxStaticText(this, wxID_ANY, "create");
        wxStaticText *text4 = new wxStaticText(this, wxID_ANY, " a vault file!");
        text1->SetFont(wxFontInfo(26).Bold());
        text2->SetFont(wxFontInfo(26));
        text3->SetFont(wxFontInfo(26).Bold());
        text4->SetFont(wxFontInfo(26));

        text1->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event) { OpenVaultFile(); }, wxID_ANY);
        text3->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event) { CreateVaultFile(); }, wxID_ANY);
        text1->Bind(wxEVT_MOTION, &Window::mouseOverLabels, this, wxID_ANY);
        text3->Bind(wxEVT_MOTION, &Window::mouseOverLabels, this, wxID_ANY);

        textSizer->Add(text1, 0, 0);
        textSizer->Add(text2, 0, 0);
        textSizer->Add(text3, 0, 0);
        textSizer->Add(text4, 0, 0);

        mainSizer->AddStretchSpacer();
        mainSizer->Add(textSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
        mainSizer->AddStretchSpacer();
    }

    void Window::testEvent(wxMouseEvent &event)
    {
        std::cout << "Event!" << std::endl;
    }

    void Window::mouseOverLabels(wxMouseEvent &event)
    {
        if (!handCursorWasEnabled)
        {
            SetCursor(wxCURSOR_HAND);
            handCursorWasEnabled = true;
        }
    }

    void Window::mouseOverFrame(wxMouseEvent &event)
    {
        if (handCursorWasEnabled) 
        {
            SetCursor(wxCURSOR_DEFAULT);
            handCursorWasEnabled = false;
        }
    }


    void Window::generateVaultView()
    {
        mainSizer->Clear(true);

        wxGrid *grid = new wxGrid(this, wxID_ANY);
        grid->SetRowLabelSize(20);
        grid->CreateGrid(2, 4);
        grid->SetMinSize(wxSize(650 + 20 + 16, 250));
        grid->SetMargins(0 - wxSYS_VSCROLL_X, 0);
        grid->DisableDragRowSize();

        mainSizer->Add(grid, 0, wxALIGN_CENTER);

        grid->SetColLabelValue(0, "Domain");
        grid->SetColLabelValue(1, "User");
        grid->SetColLabelValue(2, "Password");
        grid->SetColLabelValue(3, "Comment");
        grid->SetColumnWidth(0, 150);
        grid->SetColumnWidth(1, 150);
        grid->SetColumnWidth(2, 150);
        grid->SetColumnWidth(3, 200);

        wxBoxSizer *options = new wxBoxSizer(wxHORIZONTAL);
        wxButton *button = new wxButton(this, wxID_ANY, "Add");
        button->Bind(wxEVT_LEFT_DOWN, [grid](wxMouseEvent &event) { 
            grid->AppendRows(1);
        }, wxID_ANY);
        options->Add(button, 0, wxALL, 10);
        mainSizer->Add(options, 0, wxALIGN_RIGHT);
    }

    void Window::buildMenu()
    {
        wxMenu *menuFile = new wxMenu;
        menuFile->Append(wxID_NEW, "&New...\tCtrl-N", "New vault file");
        menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open vault file");
        menuFile->AppendSeparator();
        menuFile->Append(wxID_EXIT);
        wxMenu *menuHelp = new wxMenu;
        menuHelp->Append(wxID_ABOUT);
        wxMenuBar *menuBar = new wxMenuBar;
        menuBar->Append(menuFile, "&File");
        menuBar->Append(menuHelp, "&Help");
        SetMenuBar( menuBar );

        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { CreateVaultFile(); }, wxID_NEW);
        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { OpenVaultFile(); }, wxID_OPEN);
        Bind(wxEVT_MENU, 
            [](wxCommandEvent &event) { 
                wxMessageBox("Store your passwords in a safe vault!", "About", wxOK | wxICON_INFORMATION); 
            }, 
            wxID_ABOUT);
        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { Close(true); }, wxID_EXIT);
    }

    void Window::OpenVaultFile()
    {
        wxFileDialog openFileDialog(this, "Open vault file", "", "", "CVF files (*.cvf)|*.cvf|Any file (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (openFileDialog.ShowModal() == wxID_CANCEL) return;
        currentVaultPath = openFileDialog.GetPath().ToStdString();

        wxTextEntryDialog passwordInputDialog(this, "Enter file password", "Password", wxEmptyString, wxTextEntryDialogStyle | wxTE_PASSWORD);
        if (passwordInputDialog.ShowModal() == wxID_CANCEL) return;
        currentVaultPassword = passwordInputDialog.GetValue().ToStdString();

        try
        {
            logins = Vault::readVaultFile(currentVaultPath, currentVaultPassword);
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

    void Window::CreateVaultFile()
    {
        wxFileDialog saveFileDialog(this, "New vault file", "", "", "CVF files (*.cvf)|*.cvf|Any file (*)|*", wxFD_SAVE);
        saveFileDialog.SetFilename("vault.cvf");
        if (saveFileDialog.ShowModal() == wxID_CANCEL) return;
        currentVaultPath = saveFileDialog.GetPath().ToStdString();

        wxTextEntryDialog passwordInputDialog(this, "Enter file password", "Password", wxEmptyString, wxTextEntryDialogStyle | wxTE_PASSWORD);
        while (currentVaultPassword == "")
        {
            if (passwordInputDialog.ShowModal() == wxID_CANCEL) return;
            currentVaultPassword = passwordInputDialog.GetValue().ToStdString();
            if (currentVaultPassword == "")
            {
                wxMessageBox("Cannot set an empty password", "Error", wxOK | wxICON_ERROR, this);
            }
        }
    }
}