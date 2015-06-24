#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "default_config.h"

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

#define CONFIG_FILE_NAME L"config.ini"

using boost::property_tree::ptree;
using boost::interprocess::shared_memory_object;
using boost::interprocess::mapped_region;
	
void registerHotKeyWrapper(int id, string key);
string readIniValue(ptree pt, string key);

void threadHotKeys()
{
	// read gw2dps directory from shared memory
	shared_memory_object shm(boost::interprocess::open_only, "config_path", boost::interprocess::read_only);
	mapped_region region(shm, boost::interprocess::read_only);
	wchar_t* cur_path = (wchar_t*) region.get_address();
	
	wstring abs_file_name = wstring(cur_path) + L"\\" + CONFIG_FILE_NAME;
	fstream f;
	f.open(abs_file_name.c_str(), fstream::in);
	if (!f.is_open())
	{
		f.open(abs_file_name.c_str(), fstream::out);
		f << DEFAULT_CONFIG_FILE;
	}
	// TODO: rewind is better than reopen
	f.close();
	f.open(abs_file_name.c_str(), fstream::in);
	ptree pt;
	read_ini(f, pt);
	
	registerHotKeyWrapper(KILL_APP, readIniValue(pt, "Hotkeys.KILL_APP")); // killApp

	registerHotKeyWrapper(HELP, readIniValue(pt, "Hotkeys.HELP")); // help
	registerHotKeyWrapper(EXP_MODE, readIniValue(pt, "Hotkeys.EXP_MODE")); // expMode
	registerHotKeyWrapper(SELF_FLOAT, readIniValue(pt, "Hotkeys.SELF_FLOAT")); // selfFloat
	registerHotKeyWrapper(LOOP_LIMITER, readIniValue(pt, "Hotkeys.LOOP_LIMITER")); // loopLimiter
	registerHotKeyWrapper(SELF_HEALTH_PERCENT, readIniValue(pt, "Hotkeys.SELF_HEALTH_PERCENT")); // selfHealthPercent

	registerHotKeyWrapper(TARGET_SELECTED, readIniValue(pt, "Hotkeys.TARGET_SELECTED")); // targetSelected
	registerHotKeyWrapper(TARGET_INFO, readIniValue(pt, "Hotkeys.TARGET_INFO")); // targetInfo
	registerHotKeyWrapper(TARGET_INFO_ALT, readIniValue(pt, "Hotkeys.TARGET_INFO_ALT")); // targetInfoAlt
	registerHotKeyWrapper(TARGET_LOCK, readIniValue(pt, "Hotkeys.TARGET_LOCK")); // targetLock
	registerHotKeyWrapper(DPS_ALLOW_NEGATIVE, readIniValue(pt, "Hotkeys.DPS_ALLOW_NEGATIVE")); // dpsAllowNegative

	registerHotKeyWrapper(LOG_DPS, readIniValue(pt, "Hotkeys.LOG_DPS")); // logDps
	registerHotKeyWrapper(LOG_DPS_DETAILS, readIniValue(pt, "Hotkeys.LOG_DPS_DETAILS")); // logDpsDetails

	registerHotKeyWrapper(LOG_KILL_TIMER, readIniValue(pt, "Hotkeys.LOG_KILL_TIMER")); // logKillTimer
	registerHotKeyWrapper(LOG_KILL_TIMER_DETAILS, readIniValue(pt, "Hotkeys.LOG_KILL_TIMER_DETAILS")); // logKillTimerDetails
	registerHotKeyWrapper(LOG_KILL_TIMER_TO_FILE, readIniValue(pt, "Hotkeys.LOG_KILL_TIMER_TO_FILE")); // logKillTimerToFile
	
	registerHotKeyWrapper(LOG_HITS, readIniValue(pt, "Hotkeys.LOG_HITS")); // logHits
	registerHotKeyWrapper(LOG_HITS_DETAILS, readIniValue(pt, "Hotkeys.LOG_HITS_DETAILS")); // logHitsDetails
	registerHotKeyWrapper(LOG_HITS_TO_FILE, readIniValue(pt, "Hotkeys.LOG_HITS_TO_FILE")); // logHitsToFile
	registerHotKeyWrapper(LOG_ATTACK_RATE, readIniValue(pt, "Hotkeys.LOG_ATTACK_RATE")); // logAttackRate
	registerHotKeyWrapper(LOG_ATTACK_RATE_DETAILS, readIniValue(pt, "Hotkeys.LOG_ATTACK_RATE_DETAILS")); // logAttackRateDetails
	registerHotKeyWrapper(LOG_ATTACK_RATE_TO_FILE, readIniValue(pt, "Hotkeys.LOG_ATTACK_RATE_TO_FILE")); // logAttackRateToFile
	registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_MORE, readIniValue(pt, "Hotkeys.ATTACKRATE_CHAIN_HITS_MORE")); // AttackRateChainHits +
	registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_LESS, readIniValue(pt, "Hotkeys.ATTACKRATE_CHAIN_HITS_LESS")); // AttackRateChainHits -

	registerHotKeyWrapper(LOG_CRITS, readIniValue(pt, "Hotkeys.LOG_CRITS")); // logCrits
	registerHotKeyWrapper(LOG_CRITS_SAMPLE_MORE, readIniValue(pt, "Hotkeys.LOG_CRITS_SAMPLE_MORE")); // logCritsSample +
	registerHotKeyWrapper(LOG_CRITS_SAMPLE_LESS, readIniValue(pt, "Hotkeys.LOG_CRITS_SAMPLE_LESS")); // logCritsSample -
	
	registerHotKeyWrapper(ALLIES_LIST, readIniValue(pt, "Hotkeys.ALLIES_LIST")); // alliesList
	registerHotKeyWrapper(WVW_BONUS_MORE, readIniValue(pt, "Hotkeys.WVW_BONUS_MORE")); // wvwBonus +
	registerHotKeyWrapper(WVW_BONUS_LESS, readIniValue(pt, "Hotkeys.WVW_BONUS_LESS")); // wvwBonus -

	registerHotKeyWrapper(FLOAT_CIRCLES, readIniValue(pt, "Hotkeys.FLOAT_CIRCLES")); // floatCircles
	registerHotKeyWrapper(FLOAT_TYPE, readIniValue(pt, "Hotkeys.FLOAT_TYPE")); // floatType
	registerHotKeyWrapper(FLOAT_RADIUS_MORE, readIniValue(pt, "Hotkeys.FLOAT_RADIUS_MORE")); // floatRadius +
	registerHotKeyWrapper(FLOAT_RADIUS_LESS, readIniValue(pt, "Hotkeys.FLOAT_RADIUS_LESS")); // floatRadius -
	registerHotKeyWrapper(FLOAT_ALLY_NPC, readIniValue(pt, "Hotkeys.FLOAT_ALLY_NPC")); // floatAllyNpc
	registerHotKeyWrapper(FLOAT_ENEMY_NPC, readIniValue(pt, "Hotkeys.FLOAT_ENEMY_NPC")); // floatEnemyNpc
	registerHotKeyWrapper(FLOAT_ALLY_PLAYER, readIniValue(pt, "Hotkeys.FLOAT_ALLY_PLAYER")); // floatAllyPlayer
	registerHotKeyWrapper(FLOAT_ALLY_PLAYER_PROF, readIniValue(pt, "Hotkeys.FLOAT_ALLY_PLAYER_PROF")); // floatAllyPlayerProf
	registerHotKeyWrapper(FLOAT_ENEMY_PLAYER, readIniValue(pt, "Hotkeys.FLOAT_ENEMY_PLAYER")); // floatEnemyPlayer
	registerHotKeyWrapper(FLOAT_SIEGE, readIniValue(pt, "Hotkeys.FLOAT_SIEGE")); // floatSiege

	registerHotKeyWrapper(LOG_SPEEDOMETER, readIniValue(pt, "Hotkeys.LOG_SPEEDOMETER")); // logSpeedometer
	registerHotKeyWrapper(LOG_SPEEDOMETER_ENEMY, readIniValue(pt, "Hotkeys.LOG_SPEEDOMETER_ENEMY")); // logSpeedometerEnemy
	registerHotKeyWrapper(LOG_DISPLACEMENT, readIniValue(pt, "Hotkeys.LOG_DISPLACEMENT")); // logDisplacement
	registerHotKeyWrapper(LOG_DISPLACEMENT_ENEMY, readIniValue(pt, "Hotkeys.LOG_DISPLACEMENT_ENEMY")); // logDisplacementEnemy

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

string readIniValue(ptree pt, string key)
{
	try
	{
		return pt.get<string>(key);
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
	UINT uiKey = 0;
	UINT modifier = MOD_NOREPEAT;
	string key_hex;
	for (unsigned int i = 0; i < key.size(); i++)
	{
		switch (key[i])
		{
		case ' ':
			break;
		case 'a':
		case 'A':
			if (boost::iequals(key.substr(i, alt.size()), alt))
			{
				i += alt.size();
				modifier |= MOD_ALT;
			}
			break;
		case 's':
		case 'S':
			if (boost::iequals(key.substr(i, shift.size()), shift))
			{
				i += shift.size();
				modifier |= MOD_SHIFT;
			}
			break;
		case 'c':
		case 'C':
			if (boost::iequals(key.substr(i, ctrl.size()), ctrl))
			{
				i += ctrl.size();
				modifier |= MOD_CONTROL;
			}
			break;
		case '0':
			key_hex = key.substr(i, 4);
			uiKey = strtol(key_hex.c_str(), 0, 16);
			i += 4;
			break;
		default:
			return;
		}
	}
	RegisterHotKey(NULL, id, modifier, uiKey);
}
