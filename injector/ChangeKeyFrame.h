#ifndef CHANGE_KEY_FRAME_H
#define CHANGE_KEY_FRAME_H

#include <wx/wxprec.h>
#include <string>

class MainPane;
class HotKey;

using namespace std;

class ChangeKeyFrame : public wxFrame
{
public:
    ChangeKeyFrame(MainPane* parent, const wxString& title, string config_key);
	~ChangeKeyFrame();

	void OnClose(wxCloseEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	void OnCancelKey(wxCommandEvent& WXUNUSED(event));
	void OnResetKey(wxCommandEvent& WXUNUSED(event));
	void OnConfirmKey(wxCommandEvent& WXUNUSED(event));

	HotKey* hotkey;
	
private:
	string config_key;
	MainPane* parent;
	wxStaticText* label;
	wxButton* cancel;
	wxButton* reset;
	wxButton* confirm;
    wxDECLARE_EVENT_TABLE();
};

enum
{
	CANCEL_KEY_ID, 
	RESET_KEY_ID, 
	OK_KEY_ID, 
};
#endif