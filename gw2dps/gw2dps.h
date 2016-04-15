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
const char *fontFamily = "Verdana";
Font font, font2;
Texture profIcon[GW2::PROFESSION_END];
const char *profFilterName[] = { "ALL", "Guard", "War", "Engi", "Ranger", "Thief", "Ele", "Mes", "Necro", "Rev" };
float icon_w = 20;
float icon_h = 20;
int lineHeight = 16;
int padX = 5;
int padY = 2;
static const DWORD fontColor = 0xffffffff;
static const DWORD backColor = 0xff000000;
static const DWORD borderColor = 0xff444444;
static const DWORD floatMask = 0x30000000;
static const DWORD bgColorMask = 0x33000000;

// THREADS //
int targetLockID;
struct Target {
    Target() : valid(false), alive(true), id(0), type(0), cHealth(0), mHealth(0), pHealth(0), lvl(0), lvlActual(0), pos(Vector3(0,0,0)), breakbar(0), rot(0), speed(0) {}

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
    float speed;

    Vector3 pos;
    float rot;
};
Target selected;
Target locked;
Target self;
struct Ally {
    Ally() : id(0), profession(0), lvl(0), lvlActual(0), cHealth(0), mHealth(0), pHealth(0), vitality(0), pos({ 0, 0, 0 }), name(""), rot(0) {}

    int id;
    int profession;
    int lvl;
    int lvlActual;
    float cHealth; // current health
    float mHealth; // max health
    float pHealth; // current health in percent

    Vector3 pos;
    float rot;

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
    float rot = 0;
    float cHealth;
    float mHealth;
    int prof;
    string name;
    GW2::Attitude att;
};
struct Siege {
    int type;
    Vector3 pos;
    float mHealth;
    float cHealth;
};
struct Object {
    Vector3 pos;
    float rot = 0;
    float cHealth;
    float mHealth;
    string name;
};
struct Floaters {
    vector<Float> allyNpc;
    vector<Float> enemyNpc;
    vector<Float> allyPlayer;
    vector<Float> enemyPlayer;
    vector<Siege> siege;
    vector<Object> object;
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
float speedometer = 0;

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


class CompassOverlay {
    const float PI = 3.141592653f;

    const DWORD BackgroundColor = 0xBB62B3E2;
    const DWORD BorderColor = 0xFFFFFFFF;
    const DWORD SelfDotColor = 0xff78eaff;

    const DWORD Color_FRIENDLY = 0x00FF30;
    const DWORD Color_HOSTILE = 0xFF0000;
    const DWORD Color_INDIFFERENT = 0xFFDE00;
    const DWORD Color_NEUTRAL = 0x665E5E;
    const DWORD Color_PLAYER = 0x0000FF; //0x60D7FE;
    const DWORD Color_NPC_FOE = 0xe76d00;
    const DWORD Color_OBJECT = 0xffffff;
    const float fadeMark = 1800.0f;
    const int alphaMask = 0xFF000000;

    float dist2D(GW2LIB::Vector3 p1, GW2LIB::Vector3 p2)
    {
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }

public:
    void RenderOverlay();
    bool FilterDot(GW2LIB::Agent ag);
    int CalcFade(GW2LIB::Vector3 self, GW2LIB::Vector3 ag);
    int GetColor(GW2LIB::Character curChar, int initialColor);
};

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
