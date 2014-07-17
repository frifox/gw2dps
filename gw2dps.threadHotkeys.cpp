#include "stdafx.h"
#include "gw2dps.h"

#define HELP 0

#define TARGET_SELECTED 1
#define TARGET_LOCK 2
#define DPS_ALLOW_NEGATIVE 3

#define LOG_DPS 4
#define LOG_DPS_DETAILS 5

#define LOG_KILL_TIMER 6
#define LOG_KILL_TIMER_DETAILS 7
#define LOG_KILL_TIMER_TO_FILE 8

#define LOG_HITS 9
#define LOG_HITS_TO_FILE 10

#define LOG_ATTACK_RATE 11
#define LOG_ATTACK_RATE_TO_FILE 12
#define ATTACKRATE_CHAIN_HITS_MORE 13
#define ATTACKRATE_CHAIN_HITS_LESS 14

#define ALLIES_LIST 15
#define WVW_BONUS_MORE 16
#define WVW_BONUS_LESS 17

void threadHotKeys()
{
	RegisterHotKey(NULL, HELP, MOD_ALT | MOD_NOREPEAT, VK_OEM_2); // help

	RegisterHotKey(NULL, TARGET_SELECTED, MOD_ALT | MOD_NOREPEAT, 0x53); // targetSelected
	RegisterHotKey(NULL, TARGET_LOCK, MOD_ALT | MOD_NOREPEAT, 0x4C); // targetLock
	RegisterHotKey(NULL, DPS_ALLOW_NEGATIVE, MOD_ALT | MOD_NOREPEAT, 0x4E); // dpsAllowNegative

	RegisterHotKey(NULL, LOG_DPS, MOD_ALT | MOD_NOREPEAT, 0x44); // logDps
	RegisterHotKey(NULL, LOG_DPS_DETAILS, MOD_ALT | MOD_SHIFT | MOD_NOREPEAT, 0x44); // logDpsDetails

	RegisterHotKey(NULL, LOG_KILL_TIMER, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD7); // logKillTimer
	RegisterHotKey(NULL, LOG_KILL_TIMER_DETAILS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD1); // logKillTimerDetails
	RegisterHotKey(NULL, LOG_KILL_TIMER_TO_FILE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD4); // logKillTimerToFile
	
	RegisterHotKey(NULL, LOG_HITS, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD8); // logHits
	RegisterHotKey(NULL, LOG_HITS_TO_FILE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD5); // logHitsToFile
	RegisterHotKey(NULL, LOG_ATTACK_RATE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD9); // logAttackRate
	RegisterHotKey(NULL, LOG_ATTACK_RATE_TO_FILE, MOD_ALT | MOD_NOREPEAT, VK_NUMPAD6); // logAttackRateToFile
	RegisterHotKey(NULL, ATTACKRATE_CHAIN_HITS_MORE, MOD_ALT, VK_PRIOR); // AttackRateChainHits +
	RegisterHotKey(NULL, ATTACKRATE_CHAIN_HITS_LESS, MOD_ALT, VK_NEXT); // AttackRateChainHits -

	RegisterHotKey(NULL, ALLIES_LIST, MOD_ALT | MOD_NOREPEAT, 0x43); // alliesList
	RegisterHotKey(NULL, WVW_BONUS_MORE, MOD_ALT, VK_HOME); // wvwBonus +
	RegisterHotKey(NULL, WVW_BONUS_LESS, MOD_ALT, VK_END); // wvwBonus -

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		PeekMessage(&msg, 0, 0, 0, 0x0001);
		switch (msg.message)
		{
		case WM_HOTKEY:
			if (msg.wParam == HELP) help = !help;

			if (msg.wParam == TARGET_SELECTED) targetSelected = !targetSelected;
			if (msg.wParam == TARGET_LOCK) targetLock = !targetLock;
			if (msg.wParam == DPS_ALLOW_NEGATIVE) dpsAllowNegative = !dpsAllowNegative;
			
			if (msg.wParam == LOG_DPS) logDps = !logDps;
			if (msg.wParam == LOG_DPS_DETAILS) logDpsDetails = !logDpsDetails;

			if (msg.wParam == LOG_KILL_TIMER) logKillTimer = !logKillTimer;
			if (msg.wParam == LOG_KILL_TIMER_DETAILS) logKillTimerDetails = !logKillTimerDetails;
			if (msg.wParam == LOG_KILL_TIMER_TO_FILE) logKillTimerToFile = !logKillTimerToFile;
			
			if (msg.wParam == LOG_HITS) logHits = !logHits;
			if (msg.wParam == LOG_HITS_TO_FILE) logHitsToFile = !logHitsToFile;
			
			if (msg.wParam == LOG_ATTACK_RATE) logAttackRate = !logAttackRate;
			if (msg.wParam == LOG_ATTACK_RATE_TO_FILE) logAttackRateToFile = !logAttackRateToFile;
			if (msg.wParam == ATTACKRATE_CHAIN_HITS_MORE) if (AttackRateChainHits < 50) AttackRateChainHits += 1;
			if (msg.wParam == ATTACKRATE_CHAIN_HITS_LESS) if (AttackRateChainHits > 1) AttackRateChainHits -= 1;


			if (msg.wParam == ALLIES_LIST) alliesList = !alliesList;
			if (msg.wParam == WVW_BONUS_MORE) if (wvwBonus < 10) wvwBonus += 1;
			if (msg.wParam == WVW_BONUS_LESS) if (wvwBonus > 0) wvwBonus -= 1;
		}
	}
}
