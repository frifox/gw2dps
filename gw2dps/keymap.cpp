#ifndef KEYMAP_CPP
#define KEYMAP_CPP

#include "keymap.h"

std::map<UINT, string> keycode_descriptions;

void init_keymap()
{
	keycode_descriptions = std::map<UINT, string>();
	keycode_descriptions[VK_LBUTTON] = "Left mouse button";
	keycode_descriptions[VK_RBUTTON] = "Right mouse button";
	keycode_descriptions[VK_CANCEL] = "Control-break processing";
	keycode_descriptions[VK_MBUTTON] = "Middle mouse button (three-button mouse)";
	keycode_descriptions[VK_XBUTTON1] = "X1 mouse button";
	keycode_descriptions[VK_XBUTTON2] = "X2 mouse button";
	// 0x07 Undefined
	keycode_descriptions[VK_BACK] = "BACKSPACE key";
	keycode_descriptions[VK_TAB] = "TAB key";
	// 0x0A-0B = Reserved
	keycode_descriptions[VK_CLEAR] = "CLEAR key";
	keycode_descriptions[VK_RETURN] = "ENTER key";
	// 0x0E-0F = Undefined
	keycode_descriptions[VK_SHIFT] = "SHIFT key";
	keycode_descriptions[VK_CONTROL] = "CTRL key";
	keycode_descriptions[VK_MENU] = "ALT key";
	keycode_descriptions[VK_PAUSE] = "PAUSE key";
	keycode_descriptions[VK_CAPITAL] = "CAPS LOCK key";
	keycode_descriptions[VK_KANA] = "IME Kana mode";
	// keycode_descriptions[VK_HANGUEL] = "IME Hanguel mode";
	keycode_descriptions[VK_HANGUL] = "IME Hangul mode";
	// 0x16 = Undefined
	keycode_descriptions[VK_JUNJA] = "IME Junja mode";
	keycode_descriptions[VK_FINAL] = "IME final mode";
	keycode_descriptions[VK_HANJA] = "IME Hanja mode";
	keycode_descriptions[VK_KANJI] = "IME Kanji mode";
	// 0x1A = Undefined
	keycode_descriptions[VK_ESCAPE] = "ESC key";
	keycode_descriptions[VK_CONVERT] = "IME convert";
	keycode_descriptions[VK_NONCONVERT] = "IME nonconvert";
	keycode_descriptions[VK_ACCEPT] = "IME accept";
	keycode_descriptions[VK_MODECHANGE] = "IME mode change request";
	keycode_descriptions[VK_SPACE] = "SPACEBAR";
	keycode_descriptions[VK_PRIOR] = "PAGE UP key";
	keycode_descriptions[VK_NEXT] = "PAGE DOWN key";
	keycode_descriptions[VK_END] = "END key";
	keycode_descriptions[VK_HOME] = "HOME key";
	keycode_descriptions[VK_LEFT] = "LEFT ARROW key";
	keycode_descriptions[VK_UP] = "UP ARROW key";
	keycode_descriptions[VK_RIGHT] = "RIGHT ARROW key";
	keycode_descriptions[VK_DOWN] = "DOWN ARROW key";
	keycode_descriptions[VK_SELECT] = "SELECT key";
	keycode_descriptions[VK_PRINT] = "PRINT key";
	keycode_descriptions[VK_EXECUTE] = "EXECUTE key";
	keycode_descriptions[VK_SNAPSHOT] = "PRINT SCREEN key";
	keycode_descriptions[VK_INSERT] = "INS key";
	keycode_descriptions[VK_DELETE] = "DEL key";
	keycode_descriptions[VK_HELP] = "HELP key";
	keycode_descriptions[0x30] = "0 key";
	keycode_descriptions[0x31] = "1 key";
	keycode_descriptions[0x32] = "2 key";
	keycode_descriptions[0x33] = "3 key";
	keycode_descriptions[0x34] = "4 key";
	keycode_descriptions[0x35] = "5 key";
	keycode_descriptions[0x36] = "6 key";
	keycode_descriptions[0x37] = "7 key";
	keycode_descriptions[0x38] = "8 key";
	keycode_descriptions[0x39] = "9 key";
	// 0x3A-40 = Undefined
	keycode_descriptions[0x41] = "A key";
	keycode_descriptions[0x42] = "B key";
	keycode_descriptions[0x43] = "C key";
	keycode_descriptions[0x44] = "D key";
	keycode_descriptions[0x45] = "E key";
	keycode_descriptions[0x46] = "F key";
	keycode_descriptions[0x47] = "G key";
	keycode_descriptions[0x48] = "H key";
	keycode_descriptions[0x49] = "I key";
	keycode_descriptions[0x4A] = "J key";
	keycode_descriptions[0x4B] = "K key";
	keycode_descriptions[0x4C] = "L key";
	keycode_descriptions[0x4D] = "M key";
	keycode_descriptions[0x4E] = "N key";
	keycode_descriptions[0x4F] = "O key";
	keycode_descriptions[0x50] = "P key";
	keycode_descriptions[0x51] = "Q key";
	keycode_descriptions[0x52] = "R key";
	keycode_descriptions[0x53] = "S key";
	keycode_descriptions[0x54] = "T key";
	keycode_descriptions[0x55] = "U key";
	keycode_descriptions[0x56] = "V key";
	keycode_descriptions[0x57] = "W key";
	keycode_descriptions[0x58] = "X key";
	keycode_descriptions[0x59] = "Y key";
	keycode_descriptions[0x5A] = "Z key";
	keycode_descriptions[VK_LWIN] = "Left Windows key";
	keycode_descriptions[VK_RWIN] = "Right Windows key";
	keycode_descriptions[VK_APPS] = "Applications key";
	// 0x5E = Reserved
	keycode_descriptions[VK_SLEEP] = "Computer Sleep key";
	keycode_descriptions[VK_NUMPAD0] = "Numeric keypad 0 key";
	keycode_descriptions[VK_NUMPAD1] = "Numeric keypad 1 key";
	keycode_descriptions[VK_NUMPAD2] = "Numeric keypad 2 key";
	keycode_descriptions[VK_NUMPAD3] = "Numeric keypad 3 key";
	keycode_descriptions[VK_NUMPAD4] = "Numeric keypad 4 key";
	keycode_descriptions[VK_NUMPAD5] = "Numeric keypad 5 key";
	keycode_descriptions[VK_NUMPAD6] = "Numeric keypad 6 key";
	keycode_descriptions[VK_NUMPAD7] = "Numeric keypad 7 key";
	keycode_descriptions[VK_NUMPAD8] = "Numeric keypad 8 key";
	keycode_descriptions[VK_NUMPAD9] = "Numeric keypad 9 key";
	keycode_descriptions[VK_MULTIPLY] = "Multiply key";
	keycode_descriptions[VK_ADD] = "Add key";
	keycode_descriptions[VK_SEPARATOR] = "Separator key";
	keycode_descriptions[VK_SUBTRACT] = "Subtract key";
	keycode_descriptions[VK_DECIMAL] = "Decimal key";
	keycode_descriptions[VK_DIVIDE] = "Divide key";
	keycode_descriptions[VK_F1] = "F1 key";
	keycode_descriptions[VK_F2] = "F2 key";
	keycode_descriptions[VK_F3] = "F3 key";
	keycode_descriptions[VK_F4] = "F4 key";
	keycode_descriptions[VK_F5] = "F5 key";
	keycode_descriptions[VK_F6] = "F6 key";
	keycode_descriptions[VK_F7] = "F7 key";
	keycode_descriptions[VK_F8] = "F8 key";
	keycode_descriptions[VK_F9] = "F9 key";
	keycode_descriptions[VK_F10] = "F10 key";
	keycode_descriptions[VK_F11] = "F11 key";
	keycode_descriptions[VK_F12] = "F12 key";
	keycode_descriptions[VK_F13] = "F13 key";
	keycode_descriptions[VK_F14] = "F14 key";
	keycode_descriptions[VK_F15] = "F15 key";
	keycode_descriptions[VK_F16] = "F16 key";
	keycode_descriptions[VK_F17] = "F17 key";
	keycode_descriptions[VK_F18] = "F18 key";
	keycode_descriptions[VK_F19] = "F19 key";
	keycode_descriptions[VK_F20] = "F20 key";
	keycode_descriptions[VK_F21] = "F21 key";
	keycode_descriptions[VK_F22] = "F22 key";
	keycode_descriptions[VK_F23] = "F23 key";
	keycode_descriptions[VK_F24] = "F24 key";
	// 0x88-8F = Unassigned
	keycode_descriptions[VK_NUMLOCK] = "NUM LOCK key";
	keycode_descriptions[VK_SCROLL] = "SCROLL LOCK key";
	// 0x92-96 = OEM specific
	// 0x97-9F = Unassigned
	keycode_descriptions[VK_LSHIFT] = "Left SHIFT key";
	keycode_descriptions[VK_RSHIFT] = "Right SHIFT key";
	keycode_descriptions[VK_LCONTROL] = "Left CONTROL key";
	keycode_descriptions[VK_RCONTROL] = "Right CONTROL key";
	keycode_descriptions[VK_LMENU] = "Left MENU key";
	keycode_descriptions[VK_RMENU] = "Right MENU key";
	keycode_descriptions[VK_BROWSER_BACK] = "Browser Back key";
	keycode_descriptions[VK_BROWSER_FORWARD] = "Browser Forward key";
	keycode_descriptions[VK_BROWSER_REFRESH] = "Browser Refresh key";
	keycode_descriptions[VK_BROWSER_STOP] = "Browser Stop key";
	keycode_descriptions[VK_BROWSER_SEARCH] = "Browser Search key";
	keycode_descriptions[VK_BROWSER_FAVORITES] = "Browser Favorites key";
	keycode_descriptions[VK_BROWSER_HOME] = "Browser Start and Home key";
	keycode_descriptions[VK_VOLUME_MUTE] = "Volume Mute key";
	keycode_descriptions[VK_VOLUME_DOWN] = "Volume Down key";
	keycode_descriptions[VK_VOLUME_UP] = "Volume Up key";
	keycode_descriptions[VK_MEDIA_NEXT_TRACK] = "Next Track key";
	keycode_descriptions[VK_MEDIA_PREV_TRACK] = "Previous Track key";
	keycode_descriptions[VK_MEDIA_STOP] = "Stop Media key";
	keycode_descriptions[VK_MEDIA_PLAY_PAUSE] = "Play/Pause Media key";
	keycode_descriptions[VK_LAUNCH_MAIL] = "Start Mail key";
	keycode_descriptions[VK_LAUNCH_MEDIA_SELECT] = "Select Media key";
	keycode_descriptions[VK_LAUNCH_APP1] = "Start Application 1 key";
	keycode_descriptions[VK_LAUNCH_APP2] = "Start Application 2 key";
	// 0xB8-B9 = Reserved
	// VK_OEM_1 = Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the ';:' key
	keycode_descriptions[VK_OEM_PLUS] = "\"+\" key";
	keycode_descriptions[VK_OEM_COMMA] = "\",\" key";
	keycode_descriptions[VK_OEM_MINUS] = "\"-\" key";
	keycode_descriptions[VK_OEM_PERIOD] = "\".\" key";
	// VK_OEM_2 = 
	// Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the '/?' key
	// VK_OEM_3 = 
	// Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the '`~' key
	// 0xC1-D7 = Reserved
	// 0xD8-DA = Unassigned
	// VK_OEM_4 = 
	// Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the '[{' key
	// VK_OEM_5 = 
	// Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the '\|' key
	// VK_OEM_6 = 
	// Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the ']}' key
	// VK_OEM_7 = 
	// Used for miscellaneous characters; it can vary by keyboard.
	// For the US standard keyboard, the 'single-quote/double-quote' key
	// VK_OEM_8 = Used for miscellaneous characters; it can vary by keyboard.
	// 0xE0 = Reserved
	// 0xE1 = OEM specific
	// VK_OEM_102 = Either the angle bracket key or the backslash key on the RT 102-key keyboard
	// 0xE3-E4 = OEM specific
	keycode_descriptions[VK_PROCESSKEY] = "IME PROCESS key";
	// 0xE6 = OEM specific
	// VK_PACKET = Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
	// 0xE8 = Unassigned
	// 0xE9-F5 = OEM specific
	keycode_descriptions[VK_ATTN] = "Attn key";
	keycode_descriptions[VK_CRSEL] = "CrSel key";
	keycode_descriptions[VK_EXSEL] = "ExSel key";
	keycode_descriptions[VK_EREOF] = "Erase EOF key";
	keycode_descriptions[VK_PLAY] = "Play key";
	keycode_descriptions[VK_ZOOM] = "Zoom key";
	// keycode_descriptions[VK_NONAME] = "Reserved";
	keycode_descriptions[VK_PA1] = "PA1 key";
	keycode_descriptions[VK_OEM_CLEAR] = "Clear key";
}

string get_key_description(UINT keycode)
{
	if (keycode_descriptions.find(keycode) == keycode_descriptions.end())
	{
		return "unknown description";
	}
	else 
	{
		return keycode_descriptions[keycode];
	}
}

string get_key_description(HotKey* hotkey)
{
	if (hotkey == NULL)
	{
		return NULL;
	}
	return get_key_description(hotkey->key);
}

string get_key_description(string config_key)
{
	HotKey* hotkey = read_hotkey(config_key);
	string result = "";
	if (hotkey->alt)
	{
		result += "ALT + ";
	}
	if (hotkey->shift)
	{
		result += "SHIFT + ";
	}
	if (hotkey->ctrl)
	{
		result += "CTRL + ";
	}
	result += get_key_description(hotkey);
	delete hotkey;
	return result;
}
#endif