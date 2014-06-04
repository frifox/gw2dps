#include "stdafx.h"
#include "gw2dps.h"

// Switches //
bool help = false;
bool targetLock = false;
bool dpsAllowNegative = false; // for threadDps/threadKillTimer only
bool logDps = false;
bool logKillTimer = false;
bool logKillTimerToFile = false;
bool logHits = false;
bool logHitsToFile = false;
bool logAttackRate = false;
bool logAttackRateToFile = false;

// Settings //
int AttackRateChainHits = 1;
int AttackRateChainTime = 0; // not used atm
double dpsPollingRate = 250; // ms
string logDpsFile = "gw2dpsLog-Dps.txt";
string logHitsFile = "gw2dpsLog-Hits.txt";
string logAttackRateFile = "gw2dpsLog-AttackRate.txt";

// Threads //
#include "gw2dps.threadHotkeys.cpp"
#include "gw2dps.threadDps.cpp"
#include "gw2dps.threadKillTimer.cpp"
#include "gw2dps.threadHits.cpp"
#include "gw2dps.threadAttackRate.cpp"

void ESP()
{
	if (help){
		stringstream ss;
		ss << format("[%i] targetLock (L)") % targetLock;
		ss << format("\n[%i] dpsAllowNegative (N)") % dpsAllowNegative;
		ss << format("\n[%i] logDps (D)") % logDps;
		ss << format("\n[%i] logKillTimer (Num7)") % logKillTimer;
		ss << format("\n[%i] logKillTimnerToFile (Num4)") % logKillTimerToFile;
		ss << format("\n[%i] logHits (Num8)") % logHits;
		ss << format("\n[%i] logHitsToFile (Num5)") % logHitsToFile;
		ss << format("\n[%i] logAttackRate (Num9)") % logAttackRate;
		ss << format("\n[%i] logAttackRateToFile (Num6)") % logAttackRateToFile;
		ss << format("\n[%i] AttackRateChainHits (PgUp/PgDown)") % AttackRateChainHits;

		float x = 1200; float y = 200;
		font.Draw(x, y, fontColor, ss.str());
	}

	// Targets & Agents //
	Agent agLocked = GetLockedSelection();
	if (agLocked.IsValid())
	{
		int agType = agLocked.GetType();
		if (agType == GW2::AGENT_CATEGORY_CHAR) // char
		{
			selected.valid = true;
			selected.id = agLocked.GetAgentId();
			selected.type = agType;

			Character chLocked = agLocked.GetCharacter();
			selected.cHealth = int(chLocked.GetCurrentHealth());
			selected.mHealth = int(chLocked.GetMaxHealth());
			if (selected.mHealth > 0)
				selected.pHealth = int(100 * selected.cHealth / selected.mHealth);
			else
				selected.pHealth = 0;
			selected.lvl = chLocked.GetScaledLevel();
			selected.lvlActual = chLocked.GetLevel();
		}
		else if (agType == GW2::AGENT_TYPE_GADGET) // object
		{
			selected.valid = true;
			selected.id = agLocked.GetAgentId();
			selected.type = agType;

			unsigned long shift = *(unsigned long*)agLocked.m_ptr;
			shift = *(unsigned long*)(shift + 0x30);
			shift = *(unsigned long*)(shift + 0x164);
			if (shift)
			{
				selected.cHealth = int(*(float*)(shift + 0x8));
				selected.mHealth = int(*(float*)(shift + 0xC));
			}
			if (selected.mHealth > 0)
				selected.pHealth = int(100 * selected.cHealth / selected.mHealth);
			else
				selected.pHealth = 0;
			//selected.lvl = chLocked.GetScaledLevel();
			//selected.lvlActual = chLocked.GetLevel();
		}
		else if (agType == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) // world boss
		{
			selected.valid = true;
			selected.id = agLocked.GetAgentId();
			selected.type = agType;

			unsigned long shift = *(unsigned long*)agLocked.m_ptr;
			shift = *(unsigned long*)(shift + 0x30);
			shift = *(unsigned long*)(shift + 0x28);
			shift = *(unsigned long*)(shift + 0x178);
			if (shift)
			{
				selected.cHealth = int(*(float*)(shift + 0x8));
				selected.mHealth = int(*(float*)(shift + 0xC));
			}
			if (selected.mHealth > 0)
				selected.pHealth = int(100 * selected.cHealth / selected.mHealth);
			else
				selected.pHealth = 0;
			//selected.lvl = chLocked.GetScaledLevel();
			//selected.lvlActual = chLocked.GetLevel();
		}
		else
			selected = {};
	}
	else
		selected = {};

	// Locked Target (ID)
	if (targetLock)
	{
		if (!locked.valid && selected.valid)
			targetLockID = selected.id;
	}
	else
	{
		if (!selected.valid)
		{
			locked = {};
			targetLockID = 0;
		}
		else
			targetLockID = selected.id;
	}

	Agent ag;
	while (ag.BeNext())
	{
		// Locked Target (Data)
		if (targetLockID == ag.GetAgentId())
		{
			int agType = ag.GetType();
			if (agType == GW2::AGENT_CATEGORY_CHAR) // char
			{
				locked.valid = true;
				locked.id = ag.GetAgentId();
				locked.type = agType;

				Character ch = ag.GetCharacter();
				locked.cHealth = int(ch.GetCurrentHealth());
				locked.mHealth = int(ch.GetMaxHealth());
				if (locked.mHealth > 0)
					locked.pHealth = int(100 * locked.cHealth / locked.mHealth);
				else
					locked.pHealth = 0;
				locked.lvl = ch.GetScaledLevel();
				locked.lvlActual = ch.GetLevel();
			}
			else if (agType == GW2::AGENT_TYPE_GADGET) // struct
			{
				locked.valid = true;
				locked.id = ag.GetAgentId();
				locked.type = agType;

				unsigned long shift = *(unsigned long*)ag.m_ptr;
				shift = *(unsigned long*)(shift + 0x30);
				shift = *(unsigned long*)(shift + 0x164);
				if (shift)
				{
					locked.cHealth = int(*(float*)(shift + 0x8));
					locked.mHealth = int(*(float*)(shift + 0xC));
				}
				if (locked.mHealth > 0)
					locked.pHealth = int(100 * locked.cHealth / locked.mHealth);
				else
					locked.pHealth = 0;
				//locked.lvl = ch.GetScaledLevel();
				//locked.lvlActual = ch.GetLevel();
			}
			else if (agType == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) // world boss
			{
				locked.valid = true;
				locked.id = ag.GetAgentId();
				locked.type = agType;

				unsigned long shift = *(unsigned long*)ag.m_ptr;
				shift = *(unsigned long*)(shift + 0x30);
				shift = *(unsigned long*)(shift + 0x28);
				shift = *(unsigned long*)(shift + 0x178);
				if (shift)
				{
					locked.cHealth = int(*(float*)(shift + 0x8));
					locked.mHealth = int(*(float*)(shift + 0xC));
				}
				if (locked.mHealth > 0)
					locked.pHealth = int(100 * locked.cHealth / locked.mHealth);
				else
					locked.pHealth = 0;
				//locked.lvl = ch.GetScaledLevel();
				//locked.lvlActual = ch.GetLevel();
			}

			if (locked.cHealth == 0 && locked.mHealth != 78870) // don't clear if 78870 (indestructible golem) or targetLocked
			{
				if (targetLock)
					locked.alive = false;
				else
					locked = {};
			}
		}

		// Allies list
	}

	// ESP Elements // 
	if (selected.valid)
	{
		stringstream ss;
		ss << format("Selected: %i / %i [%i%s]") % selected.cHealth % selected.mHealth % selected.pHealth % "%%";
		font.Draw(15, 40, fontColor, ss.str());
	}
	
	if (targetLock && locked.valid)
	{
		stringstream ss;
		ss << format("Locked: %i / %i [%i%s]") % locked.cHealth % locked.mHealth % locked.pHealth % "%%";
		font.Draw(15, 40 + 14, fontColor, ss.str());
	}
	
	if (logDps && !bufferDps.empty())
	{
		double average[6] {}; // for 1s & 5s
		size_t samples = 0;

		// DP1s
		samples = 4; // 1s/250ms=4
		if (samples > bufferDps.size())
			samples = bufferDps.size();
		average[1] = 0;
		for (size_t i = 0; i < samples; i++)
			average[1] += bufferDps[i];
		average[1] = average[1] / samples * (1000 / dpsPollingRate);

		// DP5s
		samples = 20; // 5s/250ms=20
		if (samples > bufferDps.size())
			samples = bufferDps.size();
		average[5] = 0;
		for (size_t i = 0; i < samples; i++)
			average[5] += bufferDps[i];
		average[5] = average[5] / samples * (1000 / dpsPollingRate);

		// Draw
		stringstream ss;
		ss << format("DP1s: %0.0f\n") % average[1];
		ss << format("DP5s: %0.0f\n") % average[5];
		for (size_t i = 0; i < bufferDps.size(); i++)
			ss << format("\nDP250ms: %i") % bufferDps[i];
		
		font.Draw(15, 100, fontColor, ss.str());
	}

	if (logHits && !bufferHits.empty())
	{
		//double min = *min_element(bufferHits.begin(), bufferHits.end());
		//double max = *max_element(bufferHits.begin(), bufferHits.end());
		double average = 0;
		for (size_t i = 0; i < bufferHits.size(); i++)
			average += bufferHits[i];
		average = average / bufferHits.size();

		// Draw
		stringstream ss;
		ss << format("Counter: %i\n") % threadHitsCounter;
		ss << format("Average: %0.3f\n") % average;
		for (size_t i = 0; i < bufferHits.size(); i++)
			ss << format("\n%i") % bufferHits[i];
		
		font.Draw(15, 100, fontColor, ss.str());
	}

	if (logAttackRate && !bufferAttackRate.empty())
	{
		double min = *min_element(bufferAttackRate.begin(), bufferAttackRate.end());
		double max = *max_element(bufferAttackRate.begin(), bufferAttackRate.end());
		double average = 0;
		for (size_t i = 0; i < bufferAttackRate.size(); i++)
			average += bufferAttackRate[i];
		average = average / bufferAttackRate.size();

		// Draw
		stringstream ss;
		ss << format("Minimum: %0.3fs\n") % min;
		ss << format("Average: %0.3fs\n") % average;
		ss << format("Maximum: %0.3fs\n") % max;
		
		for (size_t i = 0; i < bufferAttackRate.size(); i++)
			ss << format("\n%02i: %0.3fs") % (bufferAttackRate.size() - i) % bufferAttackRate[i];

		font.Draw(15, 100, fontColor, ss.str());
	}

	if (logKillTimer)
	{
		// Draw
		stringstream ss;
		ss << format("Timer: %s\n") % secondsToString(bufferKillTimer.time);
		ss << format("DPS: %0.2f") % bufferKillTimer.dps;

		font.Draw(15, 100, fontColor, ss.str());
	}
}

class TestDll : public GW2LIB::Main
{
public:
	bool init() override
	{
		locale::global(locale("en-US"));

		EnableEsp(ESP);
		NewThread(threadHotKeys);
		NewThread(threadDps);
		NewThread(threadKillTimer);
		NewThread(threadHits);
		NewThread(threadAttackRate);
		
		if (!font.Init(14, "Verdana"))
		{
			DbgOut("Could not create Font");
			return false;
		}

		return true;
	}
};
TestDll g_testDll;