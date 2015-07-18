#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wxprec.h>

class MainPane;

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnInject(wxCommandEvent& event);

private:
	MainPane* pane;

    wxDECLARE_EVENT_TABLE();
};

enum
{
	INJECT_ID = 1,
};
#endif