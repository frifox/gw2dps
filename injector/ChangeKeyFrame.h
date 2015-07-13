#ifndef CHANGE_KEY_FRAME_H
#define CHANGE_KEY_FRAME_H

#include <wx/wxprec.h>

class MainPane;
class HotKey;

class ChangeKeyFrame : public wxFrame
{
public:
    ChangeKeyFrame(MainPane* parent, const wxString& title);
	~ChangeKeyFrame();

	void OnClose(wxCloseEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	void OnCancelKey(wxCommandEvent& WXUNUSED(event));
	void OnResetKey(wxCommandEvent& WXUNUSED(event));
	void OnConfirmKey(wxCommandEvent& WXUNUSED(event));

	HotKey* hotkey;
	
private:
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