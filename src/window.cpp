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
        , scrolledWindow(nullptr)
        , readOnly(nullptr)
        , handCursorWasEnabled(false)
    {
        SetMinSize(wxSize(700, 350));
        buildMenu();

        mainSizer = new wxBoxSizer(wxVERTICAL);

        generateEmptyView();
        
        setReadOnly(true);

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

        text1->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event) { openVaultFile(); }, wxID_ANY);
        text3->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event) { createVaultFile(); }, wxID_ANY);
        text1->Bind(wxEVT_MOTION, &Window::mouseOverLabels, this, wxID_ANY);
        text2->Bind(wxEVT_MOTION, &Window::mouseOverFrame, this, wxID_ANY);
        text3->Bind(wxEVT_MOTION, &Window::mouseOverLabels, this, wxID_ANY);
        text4->Bind(wxEVT_MOTION, &Window::mouseOverFrame, this, wxID_ANY);

        textSizer->Add(text1, 0, 0);
        textSizer->Add(text2, 0, 0);
        textSizer->Add(text3, 0, 0);
        textSizer->Add(text4, 0, 0);

        mainSizer->AddStretchSpacer();
        mainSizer->Add(textSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
        mainSizer->AddStretchSpacer();

        mainSizer->Layout();
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

        mainSizer->Add(buildHeader(), 0, wxALL | wxEXPAND, 5);

        scrolledWindow = new wxScrolledWindow(this);
        scrolledWindow->SetScrollbars(10, 10, 10, 10);
        scrolledWindow->ShowScrollbars(wxScrollbarVisibility::wxSHOW_SB_NEVER, wxScrollbarVisibility::wxSHOW_SB_DEFAULT);
        scrolledWindow->SetSizer(new wxBoxSizer(wxVERTICAL));

        mainSizer->Add(scrolledWindow, 1, wxALL | wxGROW);

        wxBoxSizer *options = new wxBoxSizer(wxHORIZONTAL);
        wxButton *button = new wxButton(this, wxID_ADD);
        button->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event) { addEntry("", "", "", ""); });
        readOnly = new wxCheckBox(this, wxID_ANY, "Read only");
        readOnly->SetValue(true);
        readOnly->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent &event) { 
            setReadOnly(event.IsChecked(), false);
        });
        wxCheckBox *viewPassword = new wxCheckBox(this, wxID_ANY, "View password");
        viewPassword->SetValue(true);
        viewPassword->Enable(false);

        options->Add(viewPassword, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
        options->Add(readOnly, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
        options->Add(button, 0, wxALL, 10);
        mainSizer->Add(options, 0, wxALIGN_RIGHT);
        
        mainSizer->Layout();
    }

    void Window::setReadOnly(bool value, bool setUI)
    {
        if (scrolledWindow == nullptr || readOnly == nullptr) return;
        for (auto &child : scrolledWindow->GetChildren())
        {
            child->Enable(!value);
        }
        if (setUI)
        {
            readOnly->SetValue(value);
        }
    }

    void Window::addEntry(std::string domain, std::string user, std::string password, std::string comment)
    {
        if (scrolledWindow == nullptr) return;
        if (readOnly == nullptr) return;

        wxSizer *sizerOfScrolledWindow = scrolledWindow->GetSizer();
        wxBoxSizer *entrySizer = new wxBoxSizer(wxHORIZONTAL);
                
        wxTextCtrl *textDomain = new wxTextCtrl(scrolledWindow, ctrlID::ID_DOMAIN);
        wxTextCtrl *textUser = new wxTextCtrl(scrolledWindow, ctrlID::ID_USER);
        wxTextCtrl *textPassword = new wxTextCtrl(scrolledWindow, ctrlID::ID_PASSWORD);
        wxTextCtrl *textComment = new wxTextCtrl(scrolledWindow, ctrlID::ID_COMMENT);
        wxButton *removeButton = new wxButton(scrolledWindow, wxID_ANY);
        removeButton->Bind(wxEVT_LEFT_DOWN, [this, entrySizer, sizerOfScrolledWindow, textDomain](wxMouseEvent &event) {
            if (wxMessageDialog(
                    this, 
                    "Are you sure you want to delete the entry for \"" + textDomain->GetValue().ToStdString() + "\"?",
                    "Delete", 
                    wxYES_NO | wxNO_DEFAULT | wxICON_ASTERISK
                ).ShowModal() == wxID_NO) 
            {
                return;
            }

            uiEntries.erase(std::remove(uiEntries.begin(), uiEntries.end(), entrySizer));
            sizerOfScrolledWindow->Hide(entrySizer);
            sizerOfScrolledWindow->Layout();
            scrolledWindow->Layout();
            scrolledWindow->SetScrollbars(10, 10, 10, 10);
        });

        textDomain->SetMinSize(wxSize(160, -1));
        textUser->SetMinSize(wxSize(130, -1));
        textPassword->SetMinSize(wxSize(130, -1));
        textComment->SetMinSize(wxSize(175, -1));
        removeButton->SetMinSize(wxSize(30, textComment->GetSize().y));

        removeButton->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE));

        textDomain->SetValue(domain);
        textUser->SetValue(user);
        textPassword->SetValue(password);
        textComment->SetValue(comment);

        entrySizer->Add(5, -1);
        entrySizer->Add(textDomain, 0, wxALL, 5);
        entrySizer->Add(textUser, 0, wxALL, 5);
        entrySizer->Add(textPassword, 0, wxALL, 5);
        entrySizer->Add(textComment, 0, wxALL, 5);
        entrySizer->Add(removeButton, 0, wxALL, 5);

        uiEntries.push_back(entrySizer);
        sizerOfScrolledWindow->Add(entrySizer, 0, wxALL);
        scrolledWindow->FitInside();
        scrolledWindow->Scroll(wxPoint(entrySizer->GetPosition().x, entrySizer->GetPosition().y));

        setReadOnly(false);
    }

    wxBoxSizer *Window::buildHeader()
    {
        wxBoxSizer *headerSizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *header1 = new wxStaticText(this, wxID_ANY, "Domain");
        wxStaticText *header2 = new wxStaticText(this, wxID_ANY, "User");
        wxStaticText *header3 = new wxStaticText(this, wxID_ANY, "Password");
        wxStaticText *header4 = new wxStaticText(this, wxID_ANY, "Comment");
        
        header1->SetFont(wxFontInfo(14).Bold());
        header2->SetFont(wxFontInfo(14).Bold());
        header3->SetFont(wxFontInfo(14).Bold());
        header4->SetFont(wxFontInfo(14).Bold());

        header1->SetMinSize(wxSize(160, -1));
        header2->SetMinSize(wxSize(130, -1));
        header3->SetMinSize(wxSize(130, -1));
        header4->SetMinSize(wxSize(175, -1));

        headerSizer->Add(header1, 0, wxALL, 5);
        headerSizer->Add(header2, 0, wxALL, 5);
        headerSizer->Add(header3, 0, wxALL, 5);
        headerSizer->Add(header4, 0, wxALL, 5);

        return headerSizer;
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

        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { createVaultFile(); }, wxID_NEW);
        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { openVaultFile(); }, wxID_OPEN);
        Bind(wxEVT_MENU, 
            [](wxCommandEvent &event) { 
                wxMessageBox("Store your passwords in a safe vault!", "About", wxOK | wxICON_INFORMATION); 
            }, 
            wxID_ABOUT);
        Bind(wxEVT_MENU, [this](wxCommandEvent &event) { Close(true); }, wxID_EXIT);
    }

    void Window::openVaultFile()
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

    void Window::createVaultFile()
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

        generateVaultView();
    }
}
