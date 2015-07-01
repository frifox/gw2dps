#ifndef HOTKEY_H
#define HOTKEY_H

#include <windows.h>
#include <string>

using namespace std;

class HotKey
{
	public:
		bool alt, shift, ctrl;
		UINT key;

		HotKey(bool _alt, bool _shift, bool _ctrl, UINT _key);
		HotKey(string str);
};
#endif