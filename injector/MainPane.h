#ifndef MAIN_PANE_H
#define MAIN_PANE_H

#include <list>

#include <wx/wxprec.h>

#include <boost/thread.hpp>

class ChangeKeyFrame;

using boost::thread;
using namespace std;

class MainPane : public wxScrolledWindow
{
public:
    MainPane(wxWindow* parent);
	
	void OnChangeKey(wxCommandEvent& WXUNUSED(event));
	void OnChangeKeyFinish();
	
private:
	void updateLabels();
	string getDescriptionTextBlock();

	ChangeKeyFrame* changeKeyDialog;
	wxWindowDisabler* windowDisabler;
	list<wxButton*> buttons;
	list<wxStaticText*> labels;
	
    wxDECLARE_EVENT_TABLE();
};

enum
{
	CHANGE_KEY_ID = 2,
};
#endif