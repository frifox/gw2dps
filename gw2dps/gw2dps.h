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

#include <unordered_map>
#include <sstream> // stringstream
#include <algorithm> // min/max_element()
#include <fstream> // ofstream / file
#include <locale> // locale
#include <algorithm> // count()

#include "GameData.h"
#include "config.h"
#include "keymap.h"
#include "hotkey.h"
#include "preferences.h"
#include "grapher.h"
#include "compass.h"

#include "hacklib/Main.h"

#include <imgui.h>
#include "imgui_impl_dx9.h"

using namespace GW2LIB;
using namespace boost;
using namespace std;

static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// threads
void threadHotKeys();
void threadDps();
void threadKillTimer();
void threadHits();
void threadAttackRate();
void threadCrits();
void threadSpeedometer();


enum FloatColor {
    COLOR_NONE = 0,
    COLOR_OBJECT = 0x44ffffff,
    COLOR_PLAYER_FOE = 0x44ff3300,
    COLOR_PLAYER_ALLY = 0x4455FFFF,
    COLOR_NPC_FOE = 0x44e76d00,
    COLOR_NPC_ALLY = 0x4433ff00,
    COLOR_NPC_INDIFF = 0x44dddd00,
    COLOR_NPC_NEUTRAL = 0x44dddddd
};


// THREADS //

struct Target {
    Target() : valid(false), alive(true), id(0), type(0), cHealth(0), mHealth(0), pHealth(0),
        lvl(0), lvlActual(0), pos(Vector3(0,0,0)), breakbar(0), rot(0), speed(0), maxSpeed(0), realSpeed(0) {}

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
    float maxSpeed;
    float realSpeed;

    Vector3 pos;
    float rot;
};

struct Ally {
    Ally() : id(0), profession(0), lvl(0), lvlActual(0), mLvl(0), cHealth(0), mHealth(0), pHealth(0), vitality(0), pos({ 0, 0, 0 }), name(""), rot(0) {}

    int id;
    int profession;
    int lvl;
    int lvlActual;
    int mLvl;
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
    int id = 0;
    Vector3 pos;
    float rot = 0;
    float cHealth;
    float mHealth;
    int prof;
    bool eliteSpec = false;
    bool isPlayer = false;
    string name;
    GW2::Attitude att;
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
    vector<Float> neutEnemyNpc;
    vector<Float> allyPlayer;
    vector<Float> enemyPlayer;
    vector<Siege> siege;
    vector<Float> object;
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

struct Displacement {
    Displacement() : id(0), start(Vector3(0, 0, 0)), dist(0) {}

    int id;
    Vector3 start;
    int dist;
};

struct Dmg {
    Dmg() : total(0), snapshot(0) {}

    float total;
    float snapshot;
};

// Layout Anchors
struct Anchor {
    Anchor() : x(0), y(0) {}

    float x;
    float y;
};


struct baseHpReturn {
    float health;
    float vitality;
};

// FUNCTIONS (defined in helpers.cpp) //
float Dist(Vector3 p1, Vector3 p2);
string SecondsToString(double input);
baseHpReturn baseHp(int lvl, int profession);
string dpsBufferToString(boost::circular_buffer<float> &buffer, size_t samples);
FloatColor GetFloatColor(const Agent &ag);
void DrawAgentPath(const Agent &ag);
void DrawFloater(const Float &floater, DWORD color = 0xffffffff,
    bool drawArrow = true, bool drawText = true, bool drawName = false, bool drawProfIcon = false);
void load_preferences();
void save_preferences();

#endif
