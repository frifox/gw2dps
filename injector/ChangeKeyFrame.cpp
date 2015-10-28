#ifndef CHANGE_KEY_FRAME_CPP
#define CHANGE_KEY_FRAME_CPP

#include "ChangeKeyFrame.h"
#include "MainPane.h"

#include "../gw2dps/hotkey.h"

wxBEGIN_EVENT_TABLE(ChangeKeyFrame, wxFrame)
	EVT_CLOSE(ChangeKeyFrame::OnClose)
	EVT_CHAR_HOOK(ChangeKeyFrame::OnKeyDown)
    EVT_BUTTON(CANCEL_KEY_ID,  ChangeKeyFrame::OnCancelKey)
    EVT_BUTTON(RESET_KEY_ID,  ChangeKeyFrame::OnResetKey)
    EVT_BUTTON(OK_KEY_ID,  ChangeKeyFrame::OnConfirmKey)
wxEND_EVENT_TABLE()

ChangeKeyFrame::ChangeKeyFrame(MainPane* parent, const wxString& title, string config_key)
	: wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, 
		(wxDEFAULT_FRAME_STYLE | wxFRAME_FLOAT_ON_PARENT) & (~wxRESIZE_BORDER) & (~wxMAXIMIZE_BOX) & (~wxMINIMIZE_BOX))
{
	this->config_key = config_key;
	this->parent = parent;
	this->hotkey = new HotKey(false, false, false, 0);
	
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	
	wxBoxSizer* label_sizer = new wxBoxSizer(wxHORIZONTAL);
	label = new wxStaticText(this, -1, "please enter a new hotkey");
	label_sizer->AddSpacer(10);
	label_sizer->Add(label, 0, 0, 0);
	label_sizer->AddSpacer(10);
	sizer->Add(label_sizer);
	sizer->AddSpacer(10);
	
	wxBoxSizer* control_sizer = new wxBoxSizer(wxHORIZONTAL);
	control_sizer->AddSpacer(10);
	cancel = new wxButton(this, CANCEL_KEY_ID, "cancel");
	reset = new wxButton(this, RESET_KEY_ID, "reset");
	confirm = new wxButton(this, OK_KEY_ID, "ok");
	control_sizer->Add(cancel, 0, wxALIGN_CENTER, 0);
	control_sizer->AddSpacer(10);
	control_sizer->Add(reset, 0, wxALIGN_CENTER, 0);
	control_sizer->AddSpacer(10);
	control_sizer->Add(confirm, 0, wxALIGN_CENTER, 0);
	control_sizer->AddSpacer(10);
	sizer->Add(control_sizer, 0, 0, 0);
	sizer->AddSpacer(10);
	
	SetSizerAndFit(sizer);
}

ChangeKeyFrame::~ChangeKeyFrame()
{
	if (hotkey != NULL)
		delete hotkey;
}

void ChangeKeyFrame::OnClose(wxCloseEvent& evt)
{
	if (hotkey!= NULL && hotkey->key == 0)
	{
		delete hotkey;
		hotkey = NULL;
	}
	parent->OnChangeKeyFinish(this->config_key);
	Destroy();
}

void ChangeKeyFrame::OnKeyDown(wxKeyEvent& evt)
{
	// raw keycodes are platform dependent (ms/linux)
	int keyCode = evt.GetRawKeyCode();

	switch (keyCode)
	{
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			hotkey->shift = true;
			break;
		case VK_MENU:
			hotkey->alt = true;
			break;
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			hotkey->ctrl = true;
			break;
		default:
			hotkey->key = keyCode;
	}
	label->SetLabel(hotkey->toString());
	Fit();
}

void ChangeKeyFrame::OnCancelKey(wxCommandEvent& WXUNUSED(event))
{
	delete hotkey;
	hotkey = NULL;
	Close(true);
}

void ChangeKeyFrame::OnResetKey(wxCommandEvent& WXUNUSED(event))
{
	this->hotkey->alt = false;
	this->hotkey->shift = false;
	this->hotkey->ctrl = false;
	label->SetLabel("please enter a new hotkey");
}

void ChangeKeyFrame::OnConfirmKey(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}
	
#endif
