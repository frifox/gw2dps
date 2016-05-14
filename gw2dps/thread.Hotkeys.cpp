#include "config.h"
#include "hotkey.h"
#include "preferences.h"

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

#define COMP_OVERLAY 94
#define COMP_OVERLAY_ZFADE 95
#define SHOW_PING 96
#define FLOAT_SNAP 97
#define FLOAT_OBJECT 98
#define PLAYER_LIST_FILTER 99
#define FLOAT_MOUSE_MEASURE 100

void registerHotKeyWrapper(int id, string key);
void registerHotKeyWrapper(int id, string key, bool repeat);
void unregisterHotkeys();

void threadHotKeys()
{
    thread_id_hotkey = GetCurrentThreadId();

    registerHotKeyWrapper(KILL_APP, read_config_value("Hotkeys.KILL_APP")); // killApp

    registerHotKeyWrapper(HELP, read_config_value("Hotkeys.HELP")); // help
    registerHotKeyWrapper(EXP_MODE, read_config_value("Hotkeys.EXP_MODE")); // expMode
    registerHotKeyWrapper(SELF_FLOAT, read_config_value("Hotkeys.SELF_FLOAT")); // selfFloat
    registerHotKeyWrapper(LOOP_LIMITER, read_config_value("Hotkeys.LOOP_LIMITER")); // loopLimiter
    registerHotKeyWrapper(SELF_HEALTH_PERCENT, read_config_value("Hotkeys.SELF_HEALTH_PERCENT")); // selfHealthPercent

    registerHotKeyWrapper(TARGET_SELECTED, read_config_value("Hotkeys.TARGET_SELECTED")); // targetSelected
    registerHotKeyWrapper(TARGET_INFO, read_config_value("Hotkeys.TARGET_INFO")); // targetInfo
    registerHotKeyWrapper(TARGET_LOCK, read_config_value("Hotkeys.TARGET_LOCK")); // targetLock
    registerHotKeyWrapper(DPS_ALLOW_NEGATIVE, read_config_value("Hotkeys.DPS_ALLOW_NEGATIVE")); // dpsAllowNegative

    registerHotKeyWrapper(LOG_DPS, read_config_value("Hotkeys.LOG_DPS")); // logDps
    registerHotKeyWrapper(LOG_DPS_DETAILS, read_config_value("Hotkeys.LOG_DPS_DETAILS")); // logDpsDetails

    registerHotKeyWrapper(LOG_KILL_TIMER, read_config_value("Hotkeys.LOG_KILL_TIMER")); // logKillTimer
    registerHotKeyWrapper(LOG_KILL_TIMER_DETAILS, read_config_value("Hotkeys.LOG_KILL_TIMER_DETAILS")); // logKillTimerDetails
    registerHotKeyWrapper(LOG_KILL_TIMER_TO_FILE, read_config_value("Hotkeys.LOG_KILL_TIMER_TO_FILE")); // logKillTimerToFile

    registerHotKeyWrapper(LOG_HITS, read_config_value("Hotkeys.LOG_HITS")); // logHits
    registerHotKeyWrapper(LOG_HITS_DETAILS, read_config_value("Hotkeys.LOG_HITS_DETAILS")); // logHitsDetails
    registerHotKeyWrapper(LOG_HITS_TO_FILE, read_config_value("Hotkeys.LOG_HITS_TO_FILE")); // logHitsToFile
    registerHotKeyWrapper(LOG_ATTACK_RATE, read_config_value("Hotkeys.LOG_ATTACK_RATE")); // logAttackRate
    registerHotKeyWrapper(LOG_ATTACK_RATE_DETAILS, read_config_value("Hotkeys.LOG_ATTACK_RATE_DETAILS")); // logAttackRateDetails
    registerHotKeyWrapper(LOG_ATTACK_RATE_TO_FILE, read_config_value("Hotkeys.LOG_ATTACK_RATE_TO_FILE")); // logAttackRateToFile
    registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_MORE, read_config_value("Hotkeys.ATTACKRATE_CHAIN_HITS_MORE"), true); // AttackRateChainHits +
    registerHotKeyWrapper(ATTACKRATE_CHAIN_HITS_LESS, read_config_value("Hotkeys.ATTACKRATE_CHAIN_HITS_LESS"), true); // AttackRateChainHits -

    registerHotKeyWrapper(LOG_CRITS, read_config_value("Hotkeys.LOG_CRITS")); // logCrits
    registerHotKeyWrapper(LOG_CRITS_SAMPLE_MORE, read_config_value("Hotkeys.LOG_CRITS_SAMPLE_MORE"), true); // logCritsSample +
    registerHotKeyWrapper(LOG_CRITS_SAMPLE_LESS, read_config_value("Hotkeys.LOG_CRITS_SAMPLE_LESS"), true); // logCritsSample -

    registerHotKeyWrapper(ALLIES_LIST, read_config_value("Hotkeys.ALLIES_LIST")); // alliesList
    registerHotKeyWrapper(WVW_BONUS_MORE, read_config_value("Hotkeys.WVW_BONUS_MORE")); // wvwBonus +
    registerHotKeyWrapper(WVW_BONUS_LESS, read_config_value("Hotkeys.WVW_BONUS_LESS")); // wvwBonus -

    registerHotKeyWrapper(FLOAT_CIRCLES, read_config_value("Hotkeys.FLOAT_CIRCLES")); // floatCircles
    registerHotKeyWrapper(FLOAT_TYPE, read_config_value("Hotkeys.FLOAT_TYPE")); // floatType
    registerHotKeyWrapper(FLOAT_RADIUS_MORE, read_config_value("Hotkeys.FLOAT_RADIUS_MORE"), true); // floatRadius +
    registerHotKeyWrapper(FLOAT_RADIUS_LESS, read_config_value("Hotkeys.FLOAT_RADIUS_LESS"), true); // floatRadius -
    registerHotKeyWrapper(FLOAT_ALLY_NPC, read_config_value("Hotkeys.FLOAT_ALLY_NPC")); // floatAllyNpc
    registerHotKeyWrapper(FLOAT_ENEMY_NPC, read_config_value("Hotkeys.FLOAT_ENEMY_NPC")); // floatEnemyNpc
    registerHotKeyWrapper(FLOAT_ALLY_PLAYER, read_config_value("Hotkeys.FLOAT_ALLY_PLAYER")); // floatAllyPlayer
    registerHotKeyWrapper(FLOAT_ALLY_PLAYER_PROF, read_config_value("Hotkeys.FLOAT_ALLY_PLAYER_PROF")); // floatAllyPlayerProf
    registerHotKeyWrapper(FLOAT_ENEMY_PLAYER, read_config_value("Hotkeys.FLOAT_ENEMY_PLAYER")); // floatEnemyPlayer
    registerHotKeyWrapper(FLOAT_SIEGE, read_config_value("Hotkeys.FLOAT_SIEGE")); // floatSiege
    registerHotKeyWrapper(FLOAT_OBJECT, read_config_value("Hotkeys.FLOAT_OBJECT")); // floatObject
    registerHotKeyWrapper(FLOAT_MOUSE_MEASURE, read_config_value("Hotkeys.FLOAT_MOUSE_MEASURE")); // floatMouse

    registerHotKeyWrapper(LOG_SPEEDOMETER, read_config_value("Hotkeys.LOG_SPEEDOMETER")); // logSpeedometer
    registerHotKeyWrapper(LOG_SPEEDOMETER_ENEMY, read_config_value("Hotkeys.LOG_SPEEDOMETER_ENEMY")); // logSpeedometerEnemy
    registerHotKeyWrapper(LOG_DISPLACEMENT, read_config_value("Hotkeys.LOG_DISPLACEMENT")); // logDisplacement
    registerHotKeyWrapper(LOG_DISPLACEMENT_ENEMY, read_config_value("Hotkeys.LOG_DISPLACEMENT_ENEMY")); // logDisplacementEnemy

    registerHotKeyWrapper(COMP_OVERLAY, read_config_value("Hotkeys.COMP_OVERLAY"));
    registerHotKeyWrapper(COMP_OVERLAY_ZFADE, read_config_value("Hotkeys.COMP_OVERLAY_ZFADE"));
    registerHotKeyWrapper(SHOW_PING, read_config_value("Hotkeys.SHOW_PING"));
    registerHotKeyWrapper(FLOAT_SNAP, read_config_value("Hotkeys.FLOAT_SNAP"));
    registerHotKeyWrapper(PLAYER_LIST_FILTER, read_config_value("Hotkeys.PLAYER_LIST_FILTER"));

    // special
    registerHotKeyWrapper(TARGET_INFO_ALT, read_config_value("Hotkeys.TARGET_INFO_ALT")); // targetInfoAlt
    RegisterHotKey(NULL, WORLD_BOSSES, MOD_ALT | MOD_SHIFT | MOD_NOREPEAT, 0x57); // worldBosses

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        if (msg.message == WM_USER) unregisterHotkeys();

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
            if (msg.wParam == FLOAT_RADIUS_MORE) if (floatRadius < floatRadiusMax) floatRadius += 1000;
            if (msg.wParam == FLOAT_RADIUS_LESS) if (floatRadius > 1000) floatRadius -= 1000;
            if (msg.wParam == FLOAT_ALLY_NPC) floatAllyNpc = !floatAllyNpc;
            if (msg.wParam == FLOAT_ENEMY_NPC) floatEnemyNpc = !floatEnemyNpc;
            if (msg.wParam == FLOAT_ALLY_PLAYER) floatAllyPlayer = !floatAllyPlayer;
            if (msg.wParam == FLOAT_ALLY_PLAYER_PROF) floatAllyPlayerProf = !floatAllyPlayerProf;
            if (msg.wParam == FLOAT_ENEMY_PLAYER) floatEnemyPlayer = !floatEnemyPlayer;
            if (msg.wParam == FLOAT_SIEGE) floatSiege = !floatSiege;
            if (msg.wParam == FLOAT_OBJECT) floatObject = !floatObject;
            if (msg.wParam == FLOAT_MOUSE_MEASURE) floatMouse = !floatMouse;

            if (msg.wParam == LOG_SPEEDOMETER) logSpeedometer = !logSpeedometer;
            if (msg.wParam == LOG_SPEEDOMETER_ENEMY) logSpeedometerEnemy = !logSpeedometerEnemy;
            if (msg.wParam == LOG_DISPLACEMENT) logDisplacement = !logDisplacement;
            if (msg.wParam == LOG_DISPLACEMENT_ENEMY) logDisplacementEnemy = !logDisplacementEnemy;

            if (msg.wParam == COMP_OVERLAY) compDots = !compDots;
            if (msg.wParam == COMP_OVERLAY_ZFADE) compDotsFade = !compDotsFade;
            if (msg.wParam == SHOW_PING) showPing = !showPing;
            if (msg.wParam == FLOAT_SNAP) floatSnap = !floatSnap;
            if (msg.wParam == PLAYER_LIST_FILTER && alliesList) {
                playerListFilter = (playerListFilter >= GW2::PROFESSION_END - 1) ? 0 : playerListFilter + 1;
            }

        }

        save_preferences();
    }
}

void unregisterHotkeys()
{
    UnregisterHotKey(NULL, KILL_APP);
    UnregisterHotKey(NULL, HELP);
    UnregisterHotKey(NULL, EXP_MODE);
    UnregisterHotKey(NULL, SELF_FLOAT);
    UnregisterHotKey(NULL, LOOP_LIMITER);
    UnregisterHotKey(NULL, SELF_HEALTH_PERCENT);
    UnregisterHotKey(NULL, WORLD_BOSSES);
    UnregisterHotKey(NULL, TARGET_SELECTED);
    UnregisterHotKey(NULL, TARGET_INFO);
    UnregisterHotKey(NULL, TARGET_INFO_ALT);
    UnregisterHotKey(NULL, TARGET_LOCK);
    UnregisterHotKey(NULL, DPS_ALLOW_NEGATIVE);
    UnregisterHotKey(NULL, LOG_DPS);
    UnregisterHotKey(NULL, LOG_DPS_DETAILS);
    UnregisterHotKey(NULL, LOG_KILL_TIMER);
    UnregisterHotKey(NULL, LOG_KILL_TIMER_DETAILS);
    UnregisterHotKey(NULL, LOG_KILL_TIMER_TO_FILE);
    UnregisterHotKey(NULL, LOG_HITS);
    UnregisterHotKey(NULL, LOG_ATTACK_RATE);
    UnregisterHotKey(NULL, LOG_ATTACK_RATE_DETAILS);
    UnregisterHotKey(NULL, LOG_ATTACK_RATE_TO_FILE);
    UnregisterHotKey(NULL, ATTACKRATE_CHAIN_HITS_MORE);
    UnregisterHotKey(NULL, ATTACKRATE_CHAIN_HITS_LESS);
    UnregisterHotKey(NULL, LOG_CRITS);
    UnregisterHotKey(NULL, LOG_CRITS_DETAILS);
    UnregisterHotKey(NULL, LOG_CRITS_SAMPLE_MORE);
    UnregisterHotKey(NULL, LOG_CRITS_SAMPLE_LESS);
    UnregisterHotKey(NULL, ALLIES_LIST);
    UnregisterHotKey(NULL, WVW_BONUS_MORE);
    UnregisterHotKey(NULL, WVW_BONUS_LESS);
    UnregisterHotKey(NULL, FLOAT_CIRCLES);
    UnregisterHotKey(NULL, FLOAT_TYPE);
    UnregisterHotKey(NULL, FLOAT_RADIUS_MORE);
    UnregisterHotKey(NULL, FLOAT_RADIUS_LESS);
    UnregisterHotKey(NULL, FLOAT_ALLY_NPC);
    UnregisterHotKey(NULL, FLOAT_ENEMY_NPC);
    UnregisterHotKey(NULL, FLOAT_ALLY_PLAYER);
    UnregisterHotKey(NULL, FLOAT_ALLY_PLAYER_PROF);
    UnregisterHotKey(NULL, FLOAT_ENEMY_PLAYER);
    UnregisterHotKey(NULL, FLOAT_SIEGE);
    UnregisterHotKey(NULL, FLOAT_OBJECT);
    UnregisterHotKey(NULL, LOG_SPEEDOMETER);
    UnregisterHotKey(NULL, LOG_SPEEDOMETER_ENEMY);
    UnregisterHotKey(NULL, LOG_DISPLACEMENT);
    UnregisterHotKey(NULL, LOG_DISPLACEMENT_ENEMY);
    UnregisterHotKey(NULL, COMP_OVERLAY);
    UnregisterHotKey(NULL, COMP_OVERLAY_ZFADE);
    UnregisterHotKey(NULL, SHOW_PING);
    UnregisterHotKey(NULL, FLOAT_SNAP);
    UnregisterHotKey(NULL, PLAYER_LIST_FILTER);
}

void registerHotKeyWrapper(int id, string key)
{
    registerHotKeyWrapper(id, key, false);
}

void registerHotKeyWrapper(int id, string key, bool repeat)
{
    string alt = "alt";
    string shift = "shift";
    string ctrl = "ctrl";
    UINT modifier = repeat ? 0 : MOD_NOREPEAT;
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
