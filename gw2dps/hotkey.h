#ifndef HOTKEY_H
#define HOTKEY_H

class HotKey
{
	public:
		bool alt, shift, ctrl;
		UINT key;
		
		HotKey(bool _alt, bool _shift, bool _ctrl, UINT _key) : alt(_alt), shift(_shift), ctrl(_ctrl), key(_key) {}
		HotKey(String str);
}
#endif