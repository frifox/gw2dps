#ifndef KEYMAP_H
#define KEYMAP_H

#include <windows.h>
#include <map>
#include <string>
#include "hotkey.h"
#include "config.h"

using namespace std;

extern std::map<UINT, string> keycode_descriptions;

void init_keymap();
string get_key_description(UINT keycode);
string get_key_description(HotKey* hotkey);
string get_key_description(string config_key);
#endif