
#include "globals.h"

// Font Settings
const char *fontFamily = "Verdana";
int lineHeight = 16;
Font font, font2;
Texture profIcon[GW2::PROFESSION_END];
Texture eliteIcon[GW2::PROFESSION_END];
Texture POFeliteIcon[GW2::PROFESSION_END];
const char *profFilterName[] = { "ALL", "Guard", "War", "Engi", "Ranger", "Thief", "Ele", "Mes", "Necro", "Rev" };
float icon_w = 20;
float icon_h = 20;
int padX = 5;
int padY = 2;
int floatRadiusMax = 60000;
const DWORD fontColor = 0xffffffff;
const DWORD backColor = 0xff000000;
const DWORD borderColor = 0xff444444;
const DWORD floatMask = 0x30000000;
const DWORD bgColorMask = 0x33000000;

Grapher dpsGraph(100);
Grapher hitGraph(100);
Grapher condiGraph(100);

// Settings //
bool killApp = false;

bool help = false;
bool hideMe = false;
bool expMode = false;
bool selfFloat = false;
bool selfHealthPercent = true;
bool loopLimiter = true;

bool showPing = false;
bool woldBosses = false;
bool compDotsFade = false;
bool compDots = false;
bool targetSelected = true;
bool targetInfo = false;
bool targetInfoAlt = false;
bool targetLock = false;
bool agentLines = false;

bool dpsAllowNegative = false;
bool dpsGrapher = false;
bool logDps = true;
bool logDpsDetails = false;
string logDpsFile = "gw2dpsLog-Dps.txt";

bool logKillTimer = false;
bool logKillTimerDetails = false;
bool logKillTimerToFile = false;

bool logHits = false;
bool logHitsDetails = false;
bool logHitsToFile = false;
string logHitsFile = "gw2dpsLog-Hits.txt";
int threadHitsCounter = 0;

bool logAttackRate = false;
bool logAttackRateDetails = false;
bool logAttackRateToFile = false;
int AttackRateChainHits = 1;
int AttackRateChainTime = 0; // not used atm
string logAttackRateFile = "gw2dpsLog-AttackRate.txt";
int threadAttackRateCounter = 0;

bool logCrits = false;
bool logCritsDetails = true;
int logCritsSample = 0;
int logCritsGlances = 0;
int logCritsNormals = 0;
int logCritsCrits = 0;
bool logCritsToFile = false;
string logCritsFile = "gw2dpsLog-Crits.txt";

bool alliesList = false;
int playerListFilter = 0;
int wvwBonus = 0;

bool floatAllyNpc = false;
bool floatEnemyNpc = false;
bool floatNeutEnemyNpc = false;
bool floatAllyPlayer = false;
bool floatAllyPlayerProf = false;
bool floatEnemyPlayer = false;
bool floatSiege = false;
bool floatObject = false;
bool floatCircles = false;
bool floatType = true; // false = HP; true = Dist;
bool floatSnap = false;
bool floatText = true;
bool floatMouse = false;
int floatRadius = floatRadiusMax;

bool showUI = false;

bool logSpeedometer = false;
bool logSpeedometerEnemy = false;
int logDisplacementValue = 0;
bool logDisplacement = false;
bool logDisplacementEnemy = false;
Vector3 logDisplacementStart = Vector3(0, 0, 0);


DWORD thread_id_hotkey = 0;

Character me;
Agent meAg;
CompassOverlay compOverlay;

int targetLockID;

Target selected;
Target locked;
Target self;

Killtimer bufferKillTimer;

Displacement bufferDisplacement;
boost::circular_buffer<float> bufferDps(20); // 5s of 250ms samples
boost::circular_buffer<float> bufferSelfDps(20); // 5s of 250ms samples
boost::circular_buffer<int> bufferHits(50);
boost::circular_buffer<double> bufferAttackRate(50); // seconds
boost::circular_buffer<int> bufferSpeedometer(30); // inches/sec, 100ms sampleRate,3s worth
std::unordered_map<int, circular_buffer<Vector3>> agPaths;

float speedometer = 0;
float speedometerMax = 0;
float speedometerReal = 0;

Dmg selfDmg;

locale c_locale("C");

float camMinZoom = 60;

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> w_converter;

