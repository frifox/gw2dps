#ifndef HOTKEY_CHANGE_BUTTON_H
#define HOTKEY_CHANGE_BUTTON_H

#include <wx/wxprec.h>

class MainPane;

using namespace std;

class HotKeyChangeButton : public wxButton
{
public:
    HotKeyChangeButton(MainPane* parent, wxString label, string config_key);
	
	void OnButtonPress(wxCommandEvent& WXUNUSED(event));

private:
	string config_key;
	MainPane* parent;
    wxDECLARE_EVENT_TABLE();
};

enum
{
	CHANGE_KEY_ID,
};
#endif