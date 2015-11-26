#ifndef GW2DPS_H
#define GW2DPS_H

// Boost v1.56 library
#include <boost/thread.hpp>
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

// THREADS //
int targetLockID;
struct Target {
	Target() : valid(false), alive(true), id(0), type(0), cHealth(0), mHealth(0), pHealth(0), lvl(0), lvlActual(0), pos(Vector3(0,0,0)), breakbar(0) {}

	bool valid;
	bool alive;
	int id;
	int type; // 0 char, 9 struct, 10 wboss
	float cHealth;
	float mHealth;
	float pHealth;
	int lvl;
	int lvlActual;
	float breakbar;

	Vector3 pos;
};
Target selected;
Target locked;
Target self;
struct Ally {
	Ally() : id(0), profession(0), lvl(0), lvlActual(0), cHealth(0), mHealth(0), pHealth(0), vitality(0), pos({ 0, 0, 0 }), name("") {}

	int id;
	int profession;
	int lvl;
	int lvlActual;
	float cHealth; // current health
	float mHealth; // max health
	float pHealth; // current health in percent
	Vector3 pos;

	int vitality;
	string name;
};
struct Allies {
	vector<Ally> war;
	vector<Ally> necro;
	vector<Ally> rev;

	vector<Ally> mes;
	vector<Ally> ranger;
	vector<Ally> engi;

	vector<Ally> guard;
	vector<Ally> ele;
	vector<Ally> thief;
};
struct Float {
	Vector3 pos;
	float mHealth;
	int prof;
};
struct Siege {
	int type;
	Vector3 pos;
	float mHealth;
	float cHealth;
};
struct Floaters {
	vector<Float> allyNpc;
	vector<Float> enemyNpc;
	vector<Float> allyPlayer;
	vector<Float> enemyPlayer;
	vector<Siege> siege;
};
struct WBoss {
	int id;
	Vector3 pos;
	float mHealth;
	float cHealth;
	float pHealth;
};
struct WBosses {
	vector<WBoss> list;
};
struct Killtimer {
	Killtimer() : dmg(0), dps(0), time(0), samplesKnown(0), samplesUnknown(0) {}

	float dmg;
	double dps;
	double time; // seconds
	int samplesKnown;
	int samplesUnknown;
};
Killtimer bufferKillTimer;
struct Displacement {
	Displacement() : id(0), start(Vector3(0, 0, 0)), dist(0) {}

	int id;
	Vector3 start;
	int dist;
};
Displacement bufferDisplacement;
boost::circular_buffer<float> bufferDps(20); // 5s of 250ms samples
boost::circular_buffer<int> bufferHits(50);
boost::circular_buffer<double> bufferAttackRate(50); // seconds
boost::circular_buffer<int> bufferSpeedometer(30); // inches/sec, 100ms sampleRate,3s worth

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
	wstring ws = wstring(as.begin(), as.end());
	replace_all(ws, "%%", "%");
	replace_all(ws, "&", "&&");
	wstring rval = ws;

	//wchar_t* buf = new wchar_t[as.size() * 2 + 2];
	//swprintf(buf, L"%S", as.c_str());
	//wstring rval = buf;
	//delete[] buf;

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
		//replace_all(str, "%%", "");
		//replace_all(str, "&&", "&");
		
		// find longest line
		LONG width = 0;
		string longLine;
		for (auto & line : lines) {
			RECT r = { 0, 0, 0, 0 };
			DrawText(hdc, convert(line.c_str()).c_str(), -1, &r, DT_CALCRECT);
			LONG w = abs(r.right - r.left);

			// refine the width
			size_t i;
			i = count(line.begin(), line.end(), ':'); w -= i * 2;
			i = count(line.begin(), line.end(), ','); w -= i * 1;
			i = count(line.begin(), line.end(), ' '); w -= i * 2;
			i = count(line.begin(), line.end(), '['); w -= i * 1;
			i = count(line.begin(), line.end(), ']'); w -= i * 1;
			i = count(line.begin(), line.end(), 'T'); w -= i * 2;
			i = count(line.begin(), line.end(), 't'); w -= i * 1;
			//i = count(line.begin(), line.end(), '%'); w -= i * 6;

			i = count(line.begin(), line.end(), 'm'); w += i * 1;
			i = count(line.begin(), line.end(), 'o'); w += i * 1;

			if (w > width) {
				width = w;
				longLine = line;
			}
		}

		if (0) { // Test Draw
			//DbgOut("%s\n", longLine.c_str());

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

struct baseHpReturn {
	float health;
	float vitality;
};
baseHpReturn baseHp(int lvl, int profession)
{
	// base stats
	float hp = 0;
	float vit = 0;

	// calc base vit for the lvl
	int cake = 0;
	while (cake <= lvl) {

		if (cake == 2)
			vit += 44;

		if (cake > 2 && cake < 11) {
			vit += 7;
		}
		else {
			if (cake % 2 == 0) {
				if (11 < cake && cake < 21)
					vit += 10;
				if (21 < cake && cake < 25)
					vit += 14;
				if (25 < cake && cake < 27)
					vit += 15;
				if (27 < cake && cake < 31)
					vit += 16;
				if (31 < cake && cake < 41)
					vit += 20;
				if (41 < cake && cake < 45)
					vit += 24;
				if (45 < cake && cake < 47)
					vit += 25;
				if (47 < cake && cake < 51)
					vit += 26;
				if (51 < cake && cake < 61)
					vit += 30;
				if (61 < cake && cake < 65)
					vit += 34;
				if (65 < cake && cake < 67)
					vit += 35;
				if (67 < cake && cake < 71)
					vit += 36;
				if (71 < cake && cake < 75)
					vit += 44;
				if (75 < cake && cake < 77)
					vit += 45;
				if (77 < cake && cake <= 80)
					vit += 46;
			}
		}

		cake++;
	}

	// calc base hp
	switch (profession)
	{
	case GW2::PROFESSION_WARRIOR:
	case GW2::PROFESSION_NECROMANCER:
		hp = lvl * 28.f;
		if (lvl > 19) hp += (lvl - 19) * float(42);
		if (lvl > 39) hp += (lvl - 39) * float(70);
		if (lvl > 59) hp += (lvl - 59) * float(70);
		if (lvl > 79) hp += (lvl - 79) * float(70);
		hp += vit * 10;
		break;
	case GW2::PROFESSION_ENGINEER:
	case GW2::PROFESSION_RANGER:
	case GW2::PROFESSION_MESMER:
	case GW2::PROFESSION_REVENANT:
		hp = lvl * 18.f;
		if (lvl > 19) hp += (lvl - 19) * float(27);
		if (lvl > 39) hp += (lvl - 39) * float(45);
		if (lvl > 59) hp += (lvl - 59) * float(45);
		if (lvl > 79) hp += (lvl - 79) * float(45);
		hp += vit * 10;
		break;
	case GW2::PROFESSION_GUARDIAN:
	case GW2::PROFESSION_ELEMENTALIST:
	case GW2::PROFESSION_THIEF:
		hp = lvl * 5.f;
		if (lvl > 19) hp += (lvl - 19) * float(7.5);
		if (lvl > 39) hp += (lvl - 39) * float(12.5);
		if (lvl > 59) hp += (lvl - 59) * float(12.5);
		if (lvl > 79) hp += (lvl - 79) * float(12.5);
		hp += vit * 10;
		break;
	}

	baseHpReturn out;
	out.health = hp;
	out.vitality = vit;
	return out;
}

#endif