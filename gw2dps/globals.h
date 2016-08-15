#ifndef GLOBALS_H
#define GLOBALS_H

#include "gw2dps.h"

/* DO NOT INITIALIZE VARIABLES HERE, EVEN CONSTANTS, put them in globals.cpp instead */
/* to make a variable global, it must have "extern" so it will be seen across translation units */

// Font Settings
extern const char *fontFamily;
extern int lineHeight;
extern Font font, font2;
extern Texture profIcon[GW2::PROFESSION_END];
extern Texture eliteIcon[GW2::PROFESSION_END];
extern const char *profFilterName[];
extern float icon_w;
extern float icon_h;
extern int padX;
extern int padY;
extern int floatRadiusMax;
extern const DWORD fontColor;
extern const DWORD backColor;
extern const DWORD borderColor;
extern const DWORD floatMask;
extern const DWORD bgColorMask;

extern Grapher dpsGraph;
extern Grapher hitGraph;
extern Grapher condiGraph;

// Settings //
extern bool killApp;

extern bool help;
extern bool expMode;
extern bool selfFloat;
extern bool selfHealthPercent;
extern bool loopLimiter;

extern bool showPing;
extern bool woldBosses;
extern bool compDotsFade;
extern bool compDots;
extern bool targetSelected;
extern bool targetInfo;
extern bool targetInfoAlt;
extern bool targetLock;
extern bool agentLines;

extern bool dpsAllowNegative;

extern bool logDps;
extern bool logDpsDetails;
extern string logDpsFile;

extern bool logKillTimer;
extern bool logKillTimerDetails;
extern bool logKillTimerToFile;

extern bool logHits;
extern bool logHitsDetails;
extern bool logHitsToFile;
extern string logHitsFile;
extern int threadHitsCounter;

extern bool logAttackRate;
extern bool logAttackRateDetails;
extern bool logAttackRateToFile;
extern int AttackRateChainHits;
extern int AttackRateChainTime;
extern string logAttackRateFile;
extern int threadAttackRateCounter;

extern bool logCrits;
extern bool logCritsDetails;
extern int logCritsSample;
extern int logCritsGlances;
extern int logCritsNormals;
extern int logCritsCrits;
extern bool logCritsToFile;
extern string logCritsFile;

extern bool alliesList;
extern int playerListFilter;
extern int wvwBonus;

extern bool floatAllyNpc;
extern bool floatEnemyNpc;
extern bool floatNeutEnemyNpc;
extern bool floatAllyPlayer;
extern bool floatAllyPlayerProf;
extern bool floatEnemyPlayer;
extern bool floatSiege;
extern bool floatObject;
extern bool floatCircles;
extern bool floatType;
extern bool floatSnap;
extern bool floatMouse;
extern int floatRadius;

extern bool logSpeedometer;
extern bool logSpeedometerEnemy;
extern int logDisplacementValue;
extern bool logDisplacement;
extern bool logDisplacementEnemy;
extern Vector3 logDisplacementStart;


extern DWORD thread_id_hotkey;

extern Character me;
extern Agent meAg;
extern CompassOverlay *compOverlay;

extern int targetLockID;

extern Target selected;
extern Target locked;
extern Target self;

extern Killtimer bufferKillTimer;

extern Displacement bufferDisplacement;
extern boost::circular_buffer<float> bufferDps;
extern boost::circular_buffer<float> bufferSelfDps;
extern boost::circular_buffer<int> bufferHits;
extern boost::circular_buffer<double> bufferAttackRate;
extern boost::circular_buffer<int> bufferSpeedometer;
extern std::unordered_map<int, circular_buffer<Vector3>> agPaths;

extern float speedometer;
extern float speedometerMax;
extern float speedometerReal;

extern Dmg selfDmg;

#endif



