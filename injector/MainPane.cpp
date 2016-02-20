#ifndef MAIN_PANE_CPP
#define MAIN_PANE_CPP

#include <algorithm>
#include <boost/format.hpp>
#include <wx/utils.h>

#include "HotKeyChangeButton.h"
#include "MainPane.h"
#include "ChangeKeyFrame.h"

#include "../gw2dps/config_keys.h"
#include "../gw2dps/config.h"
#include "../gw2dps/keymap.h"
#include "../gw2dps/hotkey.h"

using boost::format;

MainPane::MainPane(wxWindow* parent) 
    : wxScrolledWindow(parent, wxID_ANY)
{
    // config
    string hotkey_descriptions_string = getDescriptionTextBlock();
    char* hotkey_descriptions = new char[hotkey_descriptions_string.length() + 1];
    strcpy(hotkey_descriptions, hotkey_descriptions_string.c_str());

    char* cur_hotkey_description;
    cur_hotkey_description = strtok(hotkey_descriptions, "\n");

    list<string> config_keys = get_help_config_keys();
    list<string>::iterator it = config_keys.begin();

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* inner_sizer;
    sizer->AddSpacer(10);

    while (cur_hotkey_description != NULL)
    {
        inner_sizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* cur_txt = new wxStaticText(this, -1, "hotkey_description");
        inner_sizer->AddSpacer(10);
        inner_sizer->Add(cur_txt, 0, wxALIGN_CENTER | wxALIGN_LEFT, 0);
        inner_sizer->AddSpacer(10);

        int n_braces = static_cast<int>(count(cur_hotkey_description, cur_hotkey_description + strlen(cur_hotkey_description), '('));
        for (int i = 0; i < n_braces && it != config_keys.end(); i++)
        {
            HotKeyChangeButton* cur_btn = new HotKeyChangeButton(this, "change hotkey", *it);
            inner_sizer->Add(cur_btn, 0, wxALIGN_RIGHT, 0);
            buttons.push_back(cur_btn);
            it ++;
        }

        inner_sizer->AddSpacer(10);
        labels.push_back(cur_txt);
        sizer->Add(inner_sizer, 0, 0, 0);
        sizer->AddSpacer(10);
        cur_hotkey_description = strtok(NULL, "\n");
    }

    delete hotkey_descriptions;

    updateLabels();

    SetSizer(sizer);
    SetScrollRate(5, 5);
}

void MainPane::OnChangeKey(string config_key)
{
    changeKeyDialog = new ChangeKeyFrame(this, "change hotkey", config_key);
    windowDisabler = new wxWindowDisabler(this);
    changeKeyDialog->Show();
}

void MainPane::OnChangeKeyFinish(string config_key)
{
    HotKey* hotkey = changeKeyDialog->hotkey;
    if (hotkey != NULL)
    {
        write_config_value(config_key, hotkey->toConfigString());
        save_config();
        updateLabels();
    }
    delete windowDisabler;
}

void MainPane::updateLabels()
{
    string hotkey_descriptions_string = getDescriptionTextBlock();
    char* hotkey_descriptions = new char[hotkey_descriptions_string.length() + 1];
    strcpy(hotkey_descriptions, hotkey_descriptions_string.c_str());
    char* cur_hotkey_description;

    cur_hotkey_description = strtok(hotkey_descriptions, "\n");
    list<wxStaticText*>::iterator it = labels.begin();
    while (cur_hotkey_description != NULL && it != labels.end())
    {
        (*it)->SetLabel(cur_hotkey_description);

        cur_hotkey_description = strtok(NULL, "\n");
        it++;
    }
    delete hotkey_descriptions;
    FitInside();
}

string MainPane::getDescriptionTextBlock()
{
    stringstream ss;

    //ss << format("Exp Mode Crosshair (Alt H)\n") % expMode;
    //ss << format("\n");
    ss << format("Self Health Percent (%s)\n") % get_key_description("Hotkeys.SELF_HEALTH_PERCENT");
    ss << format("Selected/Locked Target HP (%s)\n") % get_key_description("Hotkeys.TARGET_SELECTED");
    ss << format("Selected Target Details (%s)\n") % get_key_description("Hotkeys.TARGET_INFO");
    ss << format("Lock On Target (%s)\n") % get_key_description("Hotkeys.TARGET_LOCK");
    ss << format("Allow Negative DPS (%s)\n") % get_key_description("Hotkeys.DPS_ALLOW_NEGATIVE");
    ss << format("DPS Meter (%s)\n") % get_key_description("Hotkeys.LOG_DPS");
    ss << format("DPS Meter History (%s)\n") % get_key_description("Hotkeys.LOG_DPS_DETAILS");
    ss << format("Kill Timer (%s)\n") % get_key_description("Hotkeys.LOG_KILL_TIMER");
    ss << format("Kill Timer Details (%s)\n") % get_key_description("Hotkeys.LOG_KILL_TIMER_DETAILS");
    //ss << format("Kill Timer Writes to a File (Alt Num4)\n") % logKillTimerToFile;
    ss << format("Monitor Hits (%s)\n") % get_key_description("Hotkeys.LOG_HITS");
    ss << format("Show Hits History (%s)\n") % get_key_description("Hotkeys.LOG_HITS_DETAILS");
    ss << format("Record Hits to File (%s)\n") % get_key_description("Hotkeys.LOG_HITS_TO_FILE");
    ss << format("Monitor Attack Rate (%s)\n") % get_key_description("Hotkeys.LOG_ATTACK_RATE");
    ss << format("Show Attack Rate History (%s)\n") % get_key_description("Hotkeys.LOG_ATTACK_RATE_DETAILS");
    ss << format("Record Attack Rate to File (%s)\n") % get_key_description("Hotkeys.LOG_ATTACK_RATE_TO_FILE");
    ss << format("Adjust Attack Rate Threshold +(%s) and -(%s)\n") % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_MORE") % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_LESS");
    ss << format("Nearby Ally Players List (%s)\n") % get_key_description("Hotkeys.ALLIES_LIST");
    ss << format("Adjust WvW HP Bonus +(%s) and -(%s)\n") % get_key_description("Hotkeys.WVW_BONUS_MORE") % get_key_description("Hotkeys.WVW_BONUS_LESS");
    ss << format("Count Ally NPCs (%s)\n") % get_key_description("Hotkeys.FLOAT_ALLY_NPC");
    ss << format("Count Enemy NPCs (%s)\n") % get_key_description("Hotkeys.FLOAT_ENEMY_NPC");
    ss << format("Count Ally Players (%s)\n") % get_key_description("Hotkeys.FLOAT_ALLY_PLAYER");
    ss << format("Count Enemy Players (%s)\n") % get_key_description("Hotkeys.FLOAT_ENEMY_PLAYER");
    //ss << format("Count Siege (Alt 5)\n") % floatSiege;
    ss << format("Show/Hide Floaters below counted (%s)\n") % get_key_description("Hotkeys.FLOAT_CIRCLES");
    ss << format("Floaters show Max HP / Distance (%s)\n") % get_key_description("Hotkeys.FLOAT_TYPE");
    ss << format("Speedometer (%s)\n") % get_key_description("Hotkeys.LOG_SPEEDOMETER");
    ss << format("Speedometer for Self/Enemy (%s)\n") % get_key_description("Hotkeys.LOG_SPEEDOMETER_ENEMY");
    ss << format("Measure Distance (%s)\n") % get_key_description("Hotkeys.LOG_DISPLACEMENT");
    ss << format("Distance for Self/Enemy (%s)\n") % get_key_description("Hotkeys.LOG_DISPLACEMENT_ENEMY");
    ss << format("Toggle Compass Overlay (%s)\n") % get_key_description("Hotkeys.COMP_OVERLAY");
    ss << format("Toggle Compass Overlay Z Fade (%s)\n") % get_key_description("Hotkeys.COMP_OVERLAY_ZFADE");
    return ss.str();
}

#endif