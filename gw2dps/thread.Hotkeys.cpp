#include "config.h"
#include "hotkey.h"

#define KILL_APP 999

#define HELP 0
#define EXP_MODE 1
#define SELF_FLOAT 2
#define LOOP_LIMITER 3
#define SELF_HEALTH_PERCENT 4
#define WORLD_BOSSES 5

#define TARGET_SELECTED 10
#define TARGET_INFO 11
#define TARGET_INFO_ALT 12
#define TARGET_LOCK 13
#define DPS_ALLOW_NEGATIVE 14

#define LOG_DPS 20
#define LOG_DPS_DETAILS 21

#define LOG_KILL_TIMER 30
#define LOG_KILL_TIMER_DETAILS 31
#define LOG_KILL_TIMER_TO_FILE 32

#define LOG_HITS 40
#define LOG_HITS_DETAILS 41
#define LOG_HITS_TO_FILE 42

#define LOG_ATTACK_RATE 50
#define LOG_ATTACK_RATE_DETAILS 51
#define LOG_ATTACK_RATE_TO_FILE 52
#define ATTACKRATE_CHAIN_HITS_MORE 53
#define ATTACKRATE_CHAIN_HITS_LESS 54

#define LOG_CRITS 60
#define LOG_CRITS_DETAILS 61
#define LOG_CRITS_SAMPLE_MORE 62
#define LOG_CRITS_SAMPLE_LESS 63

#define ALLIES_LIST 70
#define WVW_BONUS_MORE 71
#define WVW_BONUS_LESS 72

#define FLOAT_CIRCLES 80
#define FLOAT_TYPE 81
#define FLOAT_RADIUS_MORE 82
#define FLOAT_RADIUS_LESS 83
#define FLOAT_ALLY_NPC 84
#define FLOAT_ENEMY_NPC 85
#define FLOAT_ALLY_PLAYER 86
#define FLOAT_ALLY_PLAYER_PROF 87
#define FLOAT_ENEMY_PLAYER 88
#define FLOAT_SIEGE 89

#define LOG_SPEEDOMETER 90
#define LOG_SPEEDOMETER_ENEMY 91
#define LOG_DISPLACEMENT 92
#define LOG_DISPLACEMENT_ENEMY 93

void registerHotKeyWrapper(int id, string key);
string readConfigValue(string key);

void threadHotKeys()
{	
	registerHotKeyWrapper(KILL_APP, readConfigValue("Hotkeys.KILL_APP")); // killApp

	registerHotKeyWrapper(HELP, readConfigValue("Hotkeys.HELP")); // help
	registerHotKeyWrapper(EXP_MODE, readConfigValue("Hotkeys.EXP_MODE")); // expMode
	registerHotKeyWrapper(SELF_FLOAT, readConfigValue("Hotkeys.SELF_FLOAT")); // selfFloat
	registerHotKeyWrapper(LOOP_LIMITER, readConfigValue("Hotkeys.LOOP_LIMITER")); // loopLimiter
	registerHotKeyWrapper(SELF_HEALTH_PERCENT, readConfigValue("Hotkeys.SELF_HEALTH_PERCENT")); // selfHealthPercent

	registerHotKeyWrapper(TARGET_SELECTED, readConfigValue("Hotkeys.TARGET_SELECTED")); // targetSelected
	registerHotKeyWrapper(TARGET_INFO, readConfigValue("Hotkeys.TARGET_INFO")); // targetInfo
	registerHotKeyWrapper(TARGET_INFO_ALT, readConfigValue("Hotkeys.TARGET_INFO_ALT")); // targetInfoAlt
	registerHotKeyWrapper(TARGET_LOCK, readConfigValue("Hotkeys.TARGET_LOCK")); // targetLock
	registerHotKeyWrapper(DPS_ALLOW_NEGATIVE, readConfigValue("Hotkeys.DPS_ALLOW_NEGATIVE")); // dpsAllowNegative

	registerHotKeyWrapper(LOG_DPS, readConfigValue("Hotkeys.LOG_DPS")); // logDps
	registerHotKeyWrapper(LOG_DPS_DETAILS, readConfigValue("Hotkeys.LOG_DPS_DETAILS")); // logDpsDetails

	registerHotKeyWrapper(LOG_KILL_TIMER, readConfigValue("Hotkeys.LOG_KILL_TIMER")); // logKillTimer
	registerHotKeyWrapper(LOG_KILL_TIMER_DETAILS, readConfigValue("Hotkeys.LOG_KILL_TIMER_DETAILS")); // logKillTimerDetails
	registerHotKeyWrapper(LOG_KILL_TIMER_TO_FILE, readConfigValue("Hotkeys.LOG_KILL_TIMER_TO_FILE")); // logKillTimerToFile
	
	registerHotKeyWrapper(LOG_HITS, readConfigValue("Hotkeys.LOG_HITS")); // logHits
	registerHotKeyWrapper(LOG_HITS_DETAILS, readConfigValue("Hotkeys.LOG_HITS_DETAILS")); // logHitsDetails
	registerHotKeyWrapper(LOG_HITS_TO_FILE, readConfigValue("Hotkeys.LOG_HITS_TO_FILE")); // logHitsToFile
	registerHotKeyWrapper(LOG_ATTACK_RATE, readConfigValue("Hotkeys.LOG_ATTACK_RATE")); // logAttackRate
	registerHotKeyWrapper(LOG_ATTACK_RATE_DETAILS, readConfigValue("Hotkeys.LOG_ATTACK_RATE_DETAILS")); // logAttackRateDetails
	registerHotKeyWrapper(LOG_ATTACK_RATE_TO_FILE, readConfigValue("Hotkeys.LOG_ATTACK_RATE_TO_FILE")); // logAttackRateToFile
	registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_MORE, readConfigValue("Hotkeys.ATTACKRATE_CHAIN_HITS_MORE")); // AttackRateChainHits +
	registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_LESS, readConfigValue("Hotkeys.ATTACKRATE_CHAIN_HITS_LESS")); // AttackRateChainHits -

	registerHotKeyWrapper(LOG_CRITS, readConfigValue("Hotkeys.LOG_CRITS")); // logCrits
	registerHotKeyWrapper(LOG_CRITS_SAMPLE_MORE, readConfigValue("Hotkeys.LOG_CRITS_SAMPLE_MORE")); // logCritsSample +
	registerHotKeyWrapper(LOG_CRITS_SAMPLE_LESS, readConfigValue("Hotkeys.LOG_CRITS_SAMPLE_LESS")); // logCritsSample -
	
	registerHotKeyWrapper(ALLIES_LIST, readConfigValue("Hotkeys.ALLIES_LIST")); // alliesList
	registerHotKeyWrapper(WVW_BONUS_MORE, readConfigValue("Hotkeys.WVW_BONUS_MORE")); // wvwBonus +
	registerHotKeyWrapper(WVW_BONUS_LESS, readConfigValue("Hotkeys.WVW_BONUS_LESS")); // wvwBonus -

	registerHotKeyWrapper(FLOAT_CIRCLES, readConfigValue("Hotkeys.FLOAT_CIRCLES")); // floatCircles
	registerHotKeyWrapper(FLOAT_TYPE, readConfigValue("Hotkeys.FLOAT_TYPE")); // floatType
	registerHotKeyWrapper(FLOAT_RADIUS_MORE, readConfigValue("Hotkeys.FLOAT_RADIUS_MORE")); // floatRadius +
	registerHotKeyWrapper(FLOAT_RADIUS_LESS, readConfigValue("Hotkeys.FLOAT_RADIUS_LESS")); // floatRadius -
	registerHotKeyWrapper(FLOAT_ALLY_NPC, readConfigValue("Hotkeys.FLOAT_ALLY_NPC")); // floatAllyNpc
	registerHotKeyWrapper(FLOAT_ENEMY_NPC, readConfigValue("Hotkeys.FLOAT_ENEMY_NPC")); // floatEnemyNpc
	registerHotKeyWrapper(FLOAT_ALLY_PLAYER, readConfigValue("Hotkeys.FLOAT_ALLY_PLAYER")); // floatAllyPlayer
	registerHotKeyWrapper(FLOAT_ALLY_PLAYER_PROF, readConfigValue("Hotkeys.FLOAT_ALLY_PLAYER_PROF")); // floatAllyPlayerProf
	registerHotKeyWrapper(FLOAT_ENEMY_PLAYER, readConfigValue("Hotkeys.FLOAT_ENEMY_PLAYER")); // floatEnemyPlayer
	registerHotKeyWrapper(FLOAT_SIEGE, readConfigValue("Hotkeys.FLOAT_SIEGE")); // floatSiege

	registerHotKeyWrapper(LOG_SPEEDOMETER, readConfigValue("Hotkeys.LOG_SPEEDOMETER")); // logSpeedometer
	registerHotKeyWrapper(LOG_SPEEDOMETER_ENEMY, readConfigValue("Hotkeys.LOG_SPEEDOMETER_ENEMY")); // logSpeedometerEnemy
	registerHotKeyWrapper(LOG_DISPLACEMENT, readConfigValue("Hotkeys.LOG_DISPLACEMENT")); // logDisplacement
	registerHotKeyWrapper(LOG_DISPLACEMENT_ENEMY, readConfigValue("Hotkeys.LOG_DISPLACEMENT_ENEMY")); // logDisplacementEnemy

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		this_thread::interruption_point();

		PeekMessage(&msg, 0, 0, 0, 0x0001);
		switch (msg.message)
		{
		case WM_HOTKEY:
			if (msg.wParam == KILL_APP) killApp = !killApp;

			if (msg.wParam == HELP) help = !help;
			if (msg.wParam == EXP_MODE) expMode = !expMode;
			if (msg.wParam == SELF_FLOAT) selfFloat = !selfFloat;
			if (msg.wParam == LOOP_LIMITER) loopLimiter = !loopLimiter;
			if (msg.wParam == SELF_HEALTH_PERCENT) selfHealthPercent = !selfHealthPercent;
			if (msg.wParam == WORLD_BOSSES) woldBosses = !woldBosses;

			if (msg.wParam == TARGET_SELECTED) targetSelected = !targetSelected;
			if (msg.wParam == TARGET_INFO) targetInfo = !targetInfo;
			if (msg.wParam == TARGET_INFO_ALT) targetInfoAlt = !targetInfoAlt;
			if (msg.wParam == TARGET_LOCK) targetLock = !targetLock;
			if (msg.wParam == DPS_ALLOW_NEGATIVE) dpsAllowNegative = !dpsAllowNegative;
			
			if (msg.wParam == LOG_DPS) logDps = !logDps;
			if (msg.wParam == LOG_DPS_DETAILS) logDpsDetails = !logDpsDetails;

			if (msg.wParam == LOG_KILL_TIMER) logKillTimer = !logKillTimer;
			if (msg.wParam == LOG_KILL_TIMER_DETAILS) logKillTimerDetails = !logKillTimerDetails;
			if (msg.wParam == LOG_KILL_TIMER_TO_FILE) logKillTimerToFile = !logKillTimerToFile;
			
			if (msg.wParam == LOG_HITS) logHits = !logHits;
			if (msg.wParam == LOG_HITS_DETAILS) logHitsDetails = !logHitsDetails;
			if (msg.wParam == LOG_HITS_TO_FILE) logHitsToFile = !logHitsToFile;
			
			if (msg.wParam == LOG_ATTACK_RATE) logAttackRate = !logAttackRate;
			if (msg.wParam == LOG_ATTACK_RATE_DETAILS) logAttackRateDetails = !logAttackRateDetails;
			if (msg.wParam == LOG_ATTACK_RATE_TO_FILE) logAttackRateToFile = !logAttackRateToFile;
			if (msg.wParam == ATTACKRATE_CHAIN_HITS_MORE) if (AttackRateChainHits < 50) AttackRateChainHits += 1;
			if (msg.wParam == ATTACKRATE_CHAIN_HITS_LESS) if (AttackRateChainHits > 1) AttackRateChainHits -= 1;

			if (msg.wParam == LOG_CRITS) logCrits = !logCrits;
			if (msg.wParam == LOG_CRITS_SAMPLE_MORE) if (logCritsSample < 10000) logCritsSample += 1;
			if (msg.wParam == LOG_CRITS_SAMPLE_LESS) if (logCritsSample > 1) logCritsSample -= 1;

			if (msg.wParam == ALLIES_LIST) alliesList = !alliesList;
			if (msg.wParam == WVW_BONUS_MORE) if (wvwBonus < 10) wvwBonus += 1;
			if (msg.wParam == WVW_BONUS_LESS) if (wvwBonus > 0) wvwBonus -= 1;

			if (msg.wParam == FLOAT_CIRCLES) floatCircles = !floatCircles;
			if (msg.wParam == FLOAT_TYPE) floatType = !floatType;
			if (msg.wParam == FLOAT_RADIUS_MORE) if (floatRadius < 10000) floatRadius += 100;
			if (msg.wParam == FLOAT_RADIUS_LESS) if (floatRadius > 100) floatRadius -= 100;
			if (msg.wParam == FLOAT_ALLY_NPC) floatAllyNpc = !floatAllyNpc;
			if (msg.wParam == FLOAT_ENEMY_NPC) floatEnemyNpc = !floatEnemyNpc;
			if (msg.wParam == FLOAT_ALLY_PLAYER) floatAllyPlayer = !floatAllyPlayer;
			if (msg.wParam == FLOAT_ALLY_PLAYER_PROF) floatAllyPlayerProf = !floatAllyPlayerProf;
			if (msg.wParam == FLOAT_ENEMY_PLAYER) floatEnemyPlayer = !floatEnemyPlayer;
			if (msg.wParam == FLOAT_SIEGE) floatSiege = !floatSiege;

			if (msg.wParam == LOG_SPEEDOMETER) logSpeedometer = !logSpeedometer;
			if (msg.wParam == LOG_SPEEDOMETER_ENEMY) logSpeedometerEnemy = !logSpeedometerEnemy;
			if (msg.wParam == LOG_DISPLACEMENT) logDisplacement = !logDisplacement;
			if (msg.wParam == LOG_DISPLACEMENT_ENEMY) logDisplacementEnemy = !logDisplacementEnemy;
		}
	}
}

string readConfigValue(string key)
{
	try
	{
		return config_pt.get<string>(key);
	}
	catch (std::exception e)
	{
		printf("couldn't find key %s in Hotkeys.ini", key.c_str());
		return "";
	}
}

void registerHotKeyWrapper(int id, string key)
{
	string alt = "alt";
	string shift = "shift";
	string ctrl = "ctrl";
	UINT modifier = MOD_NOREPEAT;
	HotKey hotkey = HotKey(key);

	if (hotkey.alt)
	{
		modifier |= MOD_ALT;
	}
	if (hotkey.shift)
	{
		modifier |= MOD_SHIFT;
	}
	if (hotkey.ctrl)
	{
		modifier |= MOD_CONTROL;
	}
	RegisterHotKey(NULL, id, modifier, hotkey.key);
}
