#ifndef HOTKEY_CPP
#define HOTKEY_CPP

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

HotKey::HotKey(string str)
{
	alt = false;
	shift = false;
	ctrl = false;
	key = 0;
	string alt = "alt";
	string shift = "shift";
	string ctrl = "ctrl";
	UINT uiKey = 0;
	UINT modifier = MOD_NOREPEAT;
	string key_hex;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		switch (str[i])
		{
		case ' ':
			break;
		case 'a':
		case 'A':
			if (boost::iequals(str.substr(i, alt.size()), alt))
			{
				i += alt.size();
				alt = true;
			}
			break;
		case 's':
		case 'S':
			if (boost::iequals(str.substr(i, shift.size()), shift))
			{
				i += shift.size();
				shift = true;
			}
			break;
		case 'c':
		case 'C':
			if (boost::iequals(str.substr(i, ctrl.size()), ctrl))
			{
				i += ctrl.size();
				ctrl = true;
			}
			break;
		case '0':
			key_hex = str.substr(i, 4);
			key = strtol(key_hex.c_str(), 0, 16);
			i += 4;
			break;
		default:
			return;
		}
	}
}

#endif