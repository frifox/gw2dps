#ifndef MAIN_FRAME_CPP
#define MAIN_FRAME_CPP

#include "MainFrame.h"
#include "MainPane.h"

#include "Inject.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MainFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(INJECT_ID, MainFrame::OnInject)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    // menubar
    wxMenu *fileMenu = new wxMenu;

    wxMenu *helpMenu = new wxMenu;

    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

    fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit this program");
    fileMenu->Append(INJECT_ID, "&Inject\tAlt-I", "Inject gw2dps.dll");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    // main pane
    pane = new MainPane(this);
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(
        "GW2DPS is based on rafzi's hacklib \n"
        "and developed by frifox. \n"
        "\n"
        "Other contributors: \n"
        "cubimon - GUI\n"
        "hairys - 64-bit, patches",
        "About GW2DPS",
        wxOK | wxICON_INFORMATION,
        this);
}

void MainFrame::OnInject(wxCommandEvent& WXUNUSED(event))
{
    if (!inject())
    {
        wxMessageBox(
            "Could not inject dll.",
            "Error", 
            wxOK | wxICON_ERROR,
            this);
    }
    else
    {
        wxMessageBox(
            "dll was injected successfully.",
            "Success", 
            wxOK | wxICON_INFORMATION,
            this);
    }
}

#endif
