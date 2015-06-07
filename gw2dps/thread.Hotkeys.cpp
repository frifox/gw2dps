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

void threadHotKeys()
{
	RegisterHotKey(NULL, KILL_APP, MOD_ALT | MOD_NOREPEAT, 0x4B); // killApp

	RegisterHotKey(NULL, HELP, MOD_ALT | MOD_NOREPEAT, VK_OEM_2); // help
	RegisterHotKey(NULL, EXP_MODE, MOD_ALT | MOD_NOREPEAT, 0x48); // expMode
	RegisterHotKey(NULL, SELF_FLOAT, MOD_ALT | MOD_SHIFT | MOD_NOREPEAT, 0x48); // selfFloat
	RegisterHotKey(NULL, LOOP_LIMITER, MOD_ALT | MOD_NOREPEAT, 0x54); // loopLimiter
	RegisterHotKey(NULL, SELF_HEALTH_PERCENT, MOD_ALT | MOD_NOREPEAT, 0x50); // selfHealthPercent
	RegisterHotKey(NULL, WORLD_BOSSES, MOD_ALT | MOD_NOREPEAT, 0x42); // worldBoss

	RegisterHotKey(NULL, TARGET_SELECTED, MOD_ALT | MOD_NOREPEAT, 0x53); // targetSelected
	RegisterHotKey(NULL, TARGET_INFO, MOD_ALT | MOD_NOREPEAT, 0x49); // targetInfo
	RegisterHotKey(NULL, TARGET_INFO_ALT, MOD_ALT | MOD_SHIFT| MOD_NOREPEAT, 0x49); // targetInfoAlt
	RegisterHotKey(NULL, TARGET_LOCK, MOD_ALT | MOD_NOREPEAT, 0x4C); // targetLock
	RegisterHotKey(NULL, DPS_ALLOW_NEGATIVE, MOD_ALT | MOD_NOREPEAT, 0x4E); // dpsAllowNegative

	RegisterHotKey(NULL, LOG_DPS, MOD_ALT | MOD_NOREPEAT, 0x44); // logDps
	RegisterHotKey(NULL, LOG_DPS_DETAILS, MOD_ALT | MOD_SHIFT | MOD_NOREPEAT, 0x44); // logDpsDetails

	RegisterHotKey(NULL, LOG_KILL_TIMER, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD7); // logKillTimer
	RegisterHotKey(NULL, LOG_KILL_TIMER_DETAILS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD1); // logKillTimerDetails
	RegisterHotKey(NULL, LOG_KILL_TIMER_TO_FILE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD4); // logKillTimerToFile
	
	RegisterHotKey(NULL, LOG_HITS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD8); // logHits
	RegisterHotKey(NULL, LOG_HITS_DETAILS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD2); // logHitsDetails
	RegisterHotKey(NULL, LOG_HITS_TO_FILE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD5); // logHitsToFile

	RegisterHotKey(NULL, LOG_ATTACK_RATE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD9); // logAttackRate
	RegisterHotKey(NULL, LOG_ATTACK_RATE_DETAILS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD3); // logAttackRateDetails
	RegisterHotKey(NULL, LOG_ATTACK_RATE_TO_FILE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD6); // logAttackRateToFile
	RegisterHotKey(NULL, ATTACKRATE_CHAIN_HITS_MORE, MOD_ALT, VK_PRIOR); // AttackRateChainHits +
	RegisterHotKey(NULL, ATTACKRATE_CHAIN_HITS_LESS, MOD_ALT, VK_NEXT); // AttackRateChainHits -

	RegisterHotKey(NULL, LOG_CRITS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD0); // logCrits
	RegisterHotKey(NULL, LOG_CRITS_SAMPLE_MORE, MOD_ALT, VK_INSERT); // logCritsSample +
	RegisterHotKey(NULL, LOG_CRITS_SAMPLE_LESS, MOD_ALT, VK_DELETE); // logCritsSample -

	RegisterHotKey(NULL, ALLIES_LIST, MOD_ALT | MOD_NOREPEAT, 0x43); // alliesList
	RegisterHotKey(NULL, WVW_BONUS_MORE, MOD_ALT, VK_HOME); // wvwBonus +
	RegisterHotKey(NULL, WVW_BONUS_LESS, MOD_ALT, VK_END); // wvwBonus -

	RegisterHotKey(NULL, FLOAT_CIRCLES, MOD_ALT | MOD_NOREPEAT, 0x46); // floatCircles
	RegisterHotKey(NULL, FLOAT_TYPE, MOD_ALT | MOD_SHIFT |  MOD_NOREPEAT, 0x46); // floatType
	RegisterHotKey(NULL, FLOAT_RADIUS_MORE, MOD_ALT, VK_ADD); // floatRadius +
	RegisterHotKey(NULL, FLOAT_RADIUS_LESS, MOD_ALT, VK_SUBTRACT); // floatRadius -
	RegisterHotKey(NULL, FLOAT_ALLY_NPC, MOD_ALT, 0x31); // floatAllyNpc
	RegisterHotKey(NULL, FLOAT_ENEMY_NPC, MOD_ALT, 0x32); // floatEnemyNpc
	RegisterHotKey(NULL, FLOAT_ALLY_PLAYER, MOD_ALT, 0x33); // floatAllyPlayer
	RegisterHotKey(NULL, FLOAT_ALLY_PLAYER_PROF, MOD_ALT | MOD_SHIFT, 0x33); // floatAllyPlayerProf
	RegisterHotKey(NULL, FLOAT_ENEMY_PLAYER, MOD_ALT, 0x34); // floatEnemyPlayer
	RegisterHotKey(NULL, FLOAT_SIEGE, MOD_ALT, 0x35); // floatSiege
	
	RegisterHotKey(NULL, LOG_SPEEDOMETER, MOD_ALT, 0x39); // logSpeedometer
	RegisterHotKey(NULL, LOG_SPEEDOMETER_ENEMY, MOD_ALT | MOD_SHIFT, 0x39); // logSpeedometerEnemy
	RegisterHotKey(NULL, LOG_DISPLACEMENT, MOD_ALT, 0x30); // logDisplacement
	RegisterHotKey(NULL, LOG_DISPLACEMENT_ENEMY, MOD_ALT | MOD_SHIFT, 0x30); // logDisplacementEnemy

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
