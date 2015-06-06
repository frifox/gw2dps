#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "default_config.h"

#define KILL_APP 999

#define HELP 0
#define EXP_MODE 1
#define SELF_FLOAT 2
#define LOOP_LIMITER 3
#define SELF_HEALTH_PERCENT 4

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

#define ALLIES_LIST 60
#define WVW_BONUS_MORE 61
#define WVW_BONUS_LESS 62

#define FLOAT_CIRCLES 70
#define FLOAT_TYPE 71
#define FLOAT_RADIUS_MORE 72
#define FLOAT_RADIUS_LESS 73
#define FLOAT_ALLY_NPC 74
#define FLOAT_ENEMY_NPC 75
#define FLOAT_ALLY_PLAYER 76
#define FLOAT_ALLY_PLAYER_PROF 77
#define FLOAT_ENEMY_PLAYER 78
#define FLOAT_SIEGE 79

#define LOG_SPEEDOMETER 80
#define LOG_SPEEDOMETER_ENEMY 81
#define LOG_DISPLACEMENT 82
#define LOG_DISPLACEMENT_ENEMY 83

#define CONFIG_FILE_NAME "config.ini"

using boost::property_tree::ptree;
	
void registerHotKeyWrapper(int id, string key);
string readIniValue(ptree pt, string key);

void threadHotKeys()
{
	fstream f;
	f.open(CONFIG_FILE_NAME, fstream::in);
	if (!f.is_open())
	{
		f.open(CONFIG_FILE_NAME, fstream::out);
		f << DEFAULT_CONFIG_FILE;
	}
	f.close();
	f.open(CONFIG_FILE_NAME, fstream::in);
	ptree pt;
	read_ini(f, pt);
	
	registerHotKeyWrapper(KILL_APP, readIniValue(pt, "config.kill_app")); // killApp

	registerHotKeyWrapper(HELP, readIniValue(pt, "config.help")); // help
	registerHotKeyWrapper(EXP_MODE, readIniValue(pt, "config.exp_mode")); // expMode
	registerHotKeyWrapper(SELF_FLOAT, readIniValue(pt, "config.self_float")); // selfFloat
	registerHotKeyWrapper(LOOP_LIMITER, readIniValue(pt, "config.loop_limiter")); // loopLimiter
	registerHotKeyWrapper(SELF_HEALTH_PERCENT, readIniValue(pt, "config.self_health_percent")); // selfHealthPercent

	registerHotKeyWrapper(TARGET_SELECTED, readIniValue(pt, "config.target_selected")); // targetSelected
	registerHotKeyWrapper(TARGET_INFO, readIniValue(pt, "config.target_info")); // targetInfo
	registerHotKeyWrapper(TARGET_INFO_ALT, readIniValue(pt, "config.target_info_alt")); // targetInfoAlt
	registerHotKeyWrapper(TARGET_LOCK, readIniValue(pt, "config.target_lock")); // targetLock
	registerHotKeyWrapper(DPS_ALLOW_NEGATIVE, readIniValue(pt, "config.dps_allow_negative")); // dpsAllowNegative

	registerHotKeyWrapper(LOG_DPS, readIniValue(pt, "config.log_dps")); // logDps
	registerHotKeyWrapper(LOG_DPS_DETAILS, readIniValue(pt, "config.log_dps_details")); // logDpsDetails

	registerHotKeyWrapper(LOG_KILL_TIMER, readIniValue(pt, "config.log_kill_timer")); // logKillTimer
	registerHotKeyWrapper(LOG_KILL_TIMER_DETAILS, readIniValue(pt, "config.log_kill_timer_details")); // logKillTimerDetails
	registerHotKeyWrapper(LOG_KILL_TIMER_TO_FILE, readIniValue(pt, "config.log_kill_timer_to_file")); // logKillTimerToFile
	
	registerHotKeyWrapper(LOG_HITS, readIniValue(pt, "config.log_hits")); // logHits
	registerHotKeyWrapper(LOG_HITS_DETAILS, readIniValue(pt, "config.log_hits_details")); // logHitsDetails
	registerHotKeyWrapper(LOG_HITS_TO_FILE, readIniValue(pt, "config.log_hits_to_file")); // logHitsToFile
	registerHotKeyWrapper(LOG_ATTACK_RATE, readIniValue(pt, "config.log_attack_rate")); // logAttackRate
	registerHotKeyWrapper(LOG_ATTACK_RATE_DETAILS, readIniValue(pt, "config.log_attack_rate_details")); // logAttackRateDetails
	registerHotKeyWrapper(LOG_ATTACK_RATE_TO_FILE, readIniValue(pt, "config.log_attack_rate_to_file")); // logAttackRateToFile
	registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_MORE, readIniValue(pt, "config.attackrate_chain_hits_more")); // AttackRateChainHits +
	registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_LESS, readIniValue(pt, "config.attackrate_chain_hits_less")); // AttackRateChainHits -

	registerHotKeyWrapper(ALLIES_LIST, readIniValue(pt, "config.allies_list")); // alliesList
	registerHotKeyWrapper(WVW_BONUS_MORE, readIniValue(pt, "config.wvw_bonus_more")); // wvwBonus +
	registerHotKeyWrapper(WVW_BONUS_LESS, readIniValue(pt, "config.wvw_bonus_less")); // wvwBonus -

	registerHotKeyWrapper(FLOAT_CIRCLES, readIniValue(pt, "config.float_circles")); // floatCircles
	registerHotKeyWrapper(FLOAT_TYPE, readIniValue(pt, "config.float_type")); // floatType
	registerHotKeyWrapper(FLOAT_RADIUS_MORE, readIniValue(pt, "config.float_radius_more")); // floatRadius +
	registerHotKeyWrapper(FLOAT_RADIUS_LESS, readIniValue(pt, "config.float_radius_less")); // floatRadius -
	registerHotKeyWrapper(FLOAT_ALLY_NPC, readIniValue(pt, "config.float_ally_npc")); // floatAllyNpc
	registerHotKeyWrapper(FLOAT_ENEMY_NPC, readIniValue(pt, "config.float_enemy_npc")); // floatEnemyNpc
	registerHotKeyWrapper(FLOAT_ALLY_PLAYER, readIniValue(pt, "config.float_ally_player")); // floatAllyPlayer
	registerHotKeyWrapper(FLOAT_ALLY_PLAYER_PROF, readIniValue(pt, "config.float_ally_player_prof")); // floatAllyPlayerProf
	registerHotKeyWrapper(FLOAT_ENEMY_PLAYER, readIniValue(pt, "config.float_enemy_player")); // floatEnemyPlayer
	registerHotKeyWrapper(FLOAT_SIEGE, readIniValue(pt, "config.float_siege")); // floatSiege
	
	registerHotKeyWrapper(LOG_SPEEDOMETER, readIniValue(pt, "config.log_speedometer")); // logSpeedometer
	registerHotKeyWrapper(LOG_SPEEDOMETER_ENEMY, readIniValue(pt, "config.log_speedometer_enemy")); // logSpeedometerEnemy
	registerHotKeyWrapper(LOG_DISPLACEMENT, readIniValue(pt, "config.log_displacement")); // logDisplacement
	registerHotKeyWrapper(LOG_DISPLACEMENT_ENEMY, readIniValue(pt, "config.log_displacement_enemy")); // logDisplacementEnemy

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
	return pt.get<string>(key);
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
		switch(key[i])
		{
			case ' ':
				break;
			case 'a':
				if (key.compare(i, alt.size(), alt) == 0)
				{
					i += alt.size();
					modifier |= MOD_ALT;
				}
				break;
			case 's':
				if (key.compare(i, shift.size(), shift) == 0)
				{
					i += shift.size();
					modifier |= MOD_SHIFT;
				}
				break;
			case 'c':
				if (key.compare(i, ctrl.size(), ctrl) == 0)
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