#ifndef GW2DPS_H
#define GW2DPS_H

// Boost v1.55 library
#include <boost/circular_buffer.hpp>
#include <boost/timer/timer.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

#include <sstream> // stringstream
#include <algorithm> // min/max_element()
#include <fstream> // ofstream / file
#include <locale> // locale
#include <algorithm> // count()

#include "gw2lib.h"		// GW2Lib library by Rafi

using namespace GW2LIB;
using namespace boost;
using namespace std;

// Font Settings
Font font;
int lineHeight = 16;
int padX = 5;
int padY = 2;
static const DWORD fontColor = 0xffffffff;
static const DWORD backColor = 0xff000000;
static const DWORD borderColor = 0xff444444;
string strProf[] = { "Error", "Guardian", "Warrior", "Engineer", "Ranger", "Thief", "Elementalist", "Mesmer", "Necromancer", "None" };

// THREADS //
int targetLockID;
struct Target {
	Target() : valid(false), alive(true), type(0), cHealth(0), mHealth(0), pHealth(0), lvl(0), lvlActual(0) {}

	bool valid;
	bool alive;
	int id;
	int type; // 0 char, 9 struct, 10 wboss
	int cHealth;
	int mHealth;
	int pHealth;
	int lvl;
	int lvlActual;
};
Target selected;
Target locked;
struct Killtimer {
	Killtimer() : dmg(0), dps(0), time(0), samplesKnown(0), samplesUnknown(0) {}

	int dmg;
	double dps;
	double time; // seconds
	int samplesKnown;
	int samplesUnknown;
};
Killtimer bufferKillTimer;
Killtimer bufferKillTimerHistory;
boost::circular_buffer<int> bufferDps(20); // 5s of 250ms samples
boost::circular_buffer<int> bufferHits(50);
boost::circular_buffer<double> bufferAttackRate(50); // seconds
int threadHitsCounter = 0;

// Layout Anchors
struct Anchor {
	Anchor() : x(0), y(0) {}

	float x;
	float y;
};

// FUNCTIONS //
float Dist(Vector3 p1, Vector3 p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}
string SecondsToString(double input)
{
	int hours = int(input) / 60 / 60;
	int minutes = (int(input) - hours * 60 * 60) / 60;
	double seconds = (input - hours * 60 * 60 - minutes * 60);

	stringstream ss;
	if (hours > 0) ss << format("%ihr ") % hours;
	if (minutes > 0) ss << format("%im ") % minutes;
	if (seconds > 0) ss << format("%0.2fs") % seconds;
	if (input == 0) ss << "0.00s";

	return ss.str();
}
struct StrInfo {
	StrInfo() : lineCount(0), x(0), y(0) {}

	size_t lineCount;
	float x;
	float y;
};

inline wstring convert(const string& as)
{
	wchar_t* buf = new wchar_t[as.size() * 2 + 2];
	swprintf(buf, L"%S", as.c_str());
	std::wstring rval = buf;
	delete[] buf;
	return rval;
}
HWND hwnd = FindWindowEx(NULL, NULL, L"Guild Wars 2", NULL);
StrInfo StringInfo(string str)
{
	StrInfo strInfo;
	
	// Line Count
	{
		size_t lineCount = 0;
		regex token("\n");
		sregex_token_iterator i(str.begin(), str.end(), token, -1);
		sregex_token_iterator j;
		while (i != j)
		{
			cout << *i++ << endl;
			lineCount++;
		}
		strInfo.lineCount = lineCount;
	}
	
	// Width
	{
		typedef vector<string> split_vector_type;
		split_vector_type lines; // #2: Search for tokens
		split(lines, str, is_any_of("\n"), token_compress_on);

		HDC hdc = GetDC(hwnd);
		HFONT hFont = CreateFont(lineHeight, 0, 0, 0, 600, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_TT_ALWAYS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH, L"Verdana");
		HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
		
		// Sanitize
		replace_all(str, "%%", "");
		
		// find longest line
		LONG width = 0;
		string longLine;
		for (auto & line : lines) {
			RECT r = { 0, 0, 0, 0 };
			DrawText(hdc, convert(line.c_str()).c_str(), -1, &r, DT_CALCRECT);
			//DrawText(hdc, convert(line.c_str()).c_str(), -1, &r, NULL);
			LONG w = abs(r.right - r.left);

			// refine the width
			size_t i;
			i = count(line.begin(), line.end(), ':'); w -= i * 2;
			i = count(line.begin(), line.end(), ' '); w -= i * 2;
			i = count(line.begin(), line.end(), '['); w -= i * 1;
			i = count(line.begin(), line.end(), ']'); w -= i * 1;
			i = count(line.begin(), line.end(), 'T'); w -= i * 2;
			i = count(line.begin(), line.end(), 't'); w -= i * 1;
			i = count(line.begin(), line.end(), '%'); w -= i * 6;

			i = count(line.begin(), line.end(), 'm'); w += i * 1;

			if (w > width) {
				width = w;
				longLine = line;
			}
		}

		if (0) { // Test Draw
			RECT r = { 0, 0, 0, 0 };
			DrawText(hdc, convert(longLine.c_str()).c_str(), -1, &r, DT_CALCRECT);
			DrawText(hdc, convert(longLine.c_str()).c_str(), -1, &r, NULL);
		}
		DeleteObject(hFont);
		ReleaseDC(hwnd, hdc);

		strInfo.x = float(width);
		strInfo.y = float(strInfo.lineCount * lineHeight);
	}

	return strInfo;
}

#endif