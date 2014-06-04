#ifndef GW2DPS_H
#define GW2DPS_H

// Boost v1.55 library
#include <boost/circular_buffer.hpp>
#include <boost/timer/timer.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

#include <sstream>		// stringstream
#include <algorithm>	// min/max_element()
#include <fstream>		// ofstream / file
#include <locale>

#include "gw2lib.h"		// GW2Lib library by Rafi

using namespace GW2LIB;
using namespace boost;
using namespace std;

Font font;
static const DWORD fontColor = 0xffffffff;
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

// FUNCTIONS //

float Dist(Vector3 p1, Vector3 p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}
string secondsToString(double input)
{
	int hours = int(input) / 60 / 60;
	int minutes = (int(input) - hours * 60 * 60) / 60;
	double seconds = (input - hours * 60 * 60 - minutes * 60);

	stringstream ss;
	if (hours > 0) ss << format("%i hr ") % hours;
	if (minutes > 0) ss << format("%i min ") % minutes;
	if (seconds > 0) ss << format("%0.2f sec") % seconds;
	if (input == 0) ss << "0.00 sec";

	return ss.str();
}
int LineCount(string str)
{
	regex token("\n");
	sregex_token_iterator i(str.begin(), str.end(), token, -1);
	sregex_token_iterator j;

	int count = 0;
	while (i != j)
	{
		cout << *i++ << endl;
		count++;
	}
	return count;
}

#endif