#ifndef KEYMAP_H
#define KEYMAP_H

#include <windows.h>
#include <map>
#include <string>

using namespace std;

extern std::map<UINT, string> keycode_descriptions;

void init_keymap();
#endif