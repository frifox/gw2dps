#ifndef MAIN_PANE_H
#define MAIN_PANE_H

#include <list>

#include <wx/wxprec.h>

#include <boost/thread.hpp>

class HotKeyChangeButton;
class ChangeKeyFrame;

using boost::thread;
using namespace std;

class MainPane : public wxScrolledWindow
{
public:
    MainPane(wxWindow* parent);
	
	void OnChangeKey(string config_key);
	void OnChangeKeyFinish(string config_key);
	
private:
	void updateLabels();
	string getDescriptionTextBlock();

	ChangeKeyFrame* changeKeyDialog;
	wxWindowDisabler* windowDisabler;
	list<HotKeyChangeButton*> buttons;
	list<wxStaticText*> labels;
};
#endif