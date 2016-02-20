#ifndef HOTKEY_CHANGE_BUTTON_CPP
#define HOTKEY_CHANGE_BUTTON_CPP

#include <wx/utils.h>

#include "HotKeyChangeButton.h"
#include "MainPane.h"

wxBEGIN_EVENT_TABLE(HotKeyChangeButton, wxButton)
    EVT_BUTTON(CHANGE_KEY_ID,  HotKeyChangeButton::OnButtonPress)
wxEND_EVENT_TABLE()

HotKeyChangeButton::HotKeyChangeButton(MainPane* parent, wxString label, string config_key) 
    : wxButton((wxWindow*) parent, CHANGE_KEY_ID, label)
{
    this->config_key = config_key;
    this->parent = parent;
}

void HotKeyChangeButton::OnButtonPress(wxCommandEvent& WXUNUSED(event))
{
    parent->OnChangeKey(this->config_key);
}

#endif