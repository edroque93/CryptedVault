#include "window.h"
namespace CryptedVault::UI
{
    #define ID_Hello 1
    
    Window::Window() : wxFrame(NULL, wxID_ANY, "CryptedVault")
    {
        wxMenu *menuFile = new wxMenu;
        menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
        menuFile->AppendSeparator();
        menuFile->Append(wxID_EXIT);
        wxMenu *menuHelp = new wxMenu;
        menuHelp->Append(wxID_ABOUT);
        wxMenuBar *menuBar = new wxMenuBar;
        menuBar->Append(menuFile, "&File");
        menuBar->Append(menuHelp, "&Help");
        SetMenuBar( menuBar );
        CreateStatusBar();
        SetStatusText("Welcome to wxWidgets!");
        Bind(wxEVT_MENU, &Window::OnHello, this, ID_Hello);
        Bind(wxEVT_MENU, &Window::OnAbout, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &Window::OnExit, this, wxID_EXIT);
    }

    void Window::OnExit(wxCommandEvent& event)
    {
        Close(true);
    }

    void Window::OnAbout(wxCommandEvent& event)
    {
        wxMessageBox("This is a wxWidgets Hello World example",
                    "About Hello World", wxOK | wxICON_INFORMATION);
    }

    void Window::OnHello(wxCommandEvent& event)
    {
        wxLogMessage("Hello world from wxWidgets!");
    }
}