#include "stdafx.h"
#include "gw2dps.h"

// Switches //
bool help = false;
bool targetSelected = true;
bool targetLock = false;
bool dpsAllowNegative = false; // for threadDps/threadKillTimer only
bool logDps = true;
bool logDpsDetails = false;
bool logKillTimer = false;
bool logKillTimerDetails = false;
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
	// Element Anchors
	Anchor aTopRight, aRight, aTopLeft, aCenter, aBottom;

	aTopLeft.x = round((GetWindowWidth() / 2 - 316 - 179) / 2 + 316);
	aTopLeft.y = 8;

	 aTopRight.x = round((GetWindowWidth() / 2 - 294 - 179)/2 + GetWindowWidth() / 2 + 179);
	aTopRight.y = 8;

	aRight.x = GetWindowWidth() - 10;
	aRight.y = 8;

	aCenter.x = round(GetWindowWidth() * float(0.5));
	aCenter.y = round(GetWindowHeight() * float(0.25));

	aBottom.x = round(GetWindowWidth() * float(0.5));
	aBottom.y = round(GetWindowHeight() - 85);

	
	if (help){
		stringstream ss;
		ss << format("[%i] targetSelected/Locked (Alt S)\n") % targetSelected;
		ss << format("[%i] targetLock (Alt L)\n") % targetLock;
		ss << format("[%i] dpsAllowNegative (Alt N)\n") % dpsAllowNegative;
		ss << format("\n");
		ss << format("[%i] logDps (Alt D)\n") % logDps;
		ss << format("[%i] logDpsDetails (Alt Shift D)\n") % logDpsDetails;
		ss << format("\n");
		ss << format("[%i] logKillTimer (Alt Num7)\n") % logKillTimer;
		ss << format("[%i] logKillTimerDetails (Alt Num1)\n") % logKillTimerDetails;
		ss << format("[%i] logKillTimerToFile (Alt Num4)\n") % logKillTimerToFile;
		ss << format("\n");
		ss << format("[%i] logHits (Alt Num8)\n") % logHits;
		ss << format("[%i] logHitsToFile (Alt Num5)\n") % logHitsToFile;
		ss << format("\n");
		ss << format("[%i] logAttackRate (Alt Num9)\n") % logAttackRate;
		ss << format("[%i] logAttackRateToFile (Alt Num6)\n") % logAttackRateToFile;
		ss << format("[%i] AttackRateChainHits (Alt PgUp/PgDown)\n") % AttackRateChainHits;

		StrInfo strInfo;
		strInfo = StringInfo(ss.str());
		float x = round(aCenter.x - strInfo.x / 2);
		float y = round(aCenter.y - strInfo.y / 2);

		DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
		DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
		font.Draw(x, y, fontColor, ss.str());
	}

	// Font Draw Debug
	if (0) {
		stringstream ss;
		ss << format("Selected: 18,140 / 18,140 [100%s]") % "%%";
		ss << format("Locked: 18,140 / 18,140 [100%s]") % "%%";

		StrInfo strInfo;
		strInfo = StringInfo(ss.str());
		float x = 0;
		float y = float(strInfo.lineCount * lineHeight + 1);
		padX = 0;
		padY = 0;

		DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, 0xffffffff);
		font.Draw(x, y, 0xff000000, ss.str());

		return;
	}

	// Targets & Agents //
	Character me = GetOwnCharacter();
	if (me.IsValid()){
		self.cHealth = int(me.GetCurrentHealth());
		self.mHealth = int(me.GetMaxHealth());
		if (self.mHealth > 0)
			self.pHealth = int(100.f * float(self.cHealth) / float(self.mHealth));
		else
			self.pHealth = 0;

		self.lvl = me.GetScaledLevel();
		self.lvlActual = me.GetLevel();
		self.alive = me.IsAlive();
	}
	Agent agLocked = GetLockedSelection();
	if (agLocked.IsValid())
	{
		if (agLocked.GetAgentId() != selected.id)
			selected = {};

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
				selected.pHealth = int(100.f * float(selected.cHealth) / float(selected.mHealth));
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
				selected.pHealth = int(100.f * float(selected.cHealth) / float(selected.mHealth));
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
				selected.pHealth = int(100.f * float(selected.cHealth) / float(selected.mHealth));
			else
				selected.pHealth = 0;

			//selected.lvl = chLocked.GetScaledLevel();
			//selected.lvlActual = chLocked.GetLevel();
		}
		else
			selected = {};

		if (selected.mHealth == 0)
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
					locked.pHealth = int(100.f * float(locked.cHealth) / float(locked.mHealth));
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
					locked.pHealth = int(100.f * float(locked.cHealth) / float(locked.mHealth));
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
					locked.pHealth = int(100.f * float(locked.cHealth) / float(locked.mHealth));
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

	// Bottom Element //
	{
		stringstream ss;
		StrInfo strInfo;

		if (self.alive)
		{
			ss << format("%i") % self.pHealth;

			strInfo = StringInfo(ss.str());
			float x = round(aBottom.x - strInfo.x / 2);
			float y = round(aBottom.y - lineHeight);

			//DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
			//DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor, ss.str());
		}
	}

	// TopLeft Element //
	{
		stringstream ss;
		StrInfo strInfo;

		if (targetSelected)
		{
			if (selected.valid)
			{
				Character chLocked = agLocked.GetCharacter();

				ss << format("Selected: %i / %i [%i%s]") % selected.cHealth % selected.mHealth % selected.pHealth % "%%";

				strInfo = StringInfo(ss.str());
				float x = round(aTopLeft.x - strInfo.x / 2);
				float y = round(aTopLeft.y);

				DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
				DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
				font.Draw(x, y, fontColor, ss.str());

				// Prepare for Next Element
				ss.str("");
				aTopLeft.y += strInfo.lineCount * lineHeight + padY * 2;
			}

			if (targetLock && locked.valid)
			{
				ss << format("Locked: %i / %i [%i%s]") % locked.cHealth % locked.mHealth % locked.pHealth % "%%";

				strInfo = StringInfo(ss.str());
				float x = round(aTopLeft.x - strInfo.x / 2);
				float y = round(aTopLeft.y);

				DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
				DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
				font.Draw(x, y, fontColor, ss.str());
			}

			// Prepare for Next Element
			ss.str("");
			aTopLeft.y += strInfo.lineCount * lineHeight + padY * 2;
		}
	}

	// TopRight Elements //
	{
		if (logDps)
		{
			// separate ss vars
			stringstream ss;
			StrInfo strInfo;

			float aAdjustX = 120; // adjust anchor -120

			if (!bufferDps.empty())
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

				// Prepare String
				ss << format("DP1s: %0.0f\n") % average[1];
				ss << format("DP5s: %0.0f\n") % average[5];
				if (logDpsDetails)
				{
					for (size_t i = 0; i < bufferDps.size(); i++)
						ss << format("\nDP250ms: %i") % bufferDps[i];
				}
			}
			else
			{
				ss << format("DP1s: ...\n");
				ss << format("DP5s: ...");
			}

			strInfo = StringInfo(ss.str());
			if (logDpsDetails && !bufferDps.empty() && strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // box min-width with history stream
			float x = round(aTopRight.x - aAdjustX / 2); // perma anchor offset
			float y = round(aTopRight.y);

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor, ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
			aTopRight.x -= aAdjustX / 2 + padX + 2;
		}

		if (logKillTimer)
		{
			// separate ss vars
			stringstream ss;
			StrInfo strInfo;

			// Prepare String
			if (bufferKillTimer.time > 0)
			{
				ss << format("Timer: %s\n") % SecondsToString(bufferKillTimer.time);
				if (logKillTimerDetails)
					ss << format("DPS: %0.2f") % bufferKillTimer.dps;
			}
			else
			{
				ss << format("Timer: 0.0s");
			}

			strInfo = StringInfo(ss.str());
			float x = 0;
			float y = round(aTopRight.y);
			if (logDps)
				x = round(aTopRight.x - strInfo.x - padX); // perma anchor offset with logDps
			else
				x = round(aTopRight.x - strInfo.x / 2); // center otherwise
			

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor, ss.str());

			// Prepare for Next Element
			//ss.str("");
			aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
			//aTopRight.x -= 0;
		}
		
	}

	// Right Elements //
	{
		if (logAttackRate)
		{
			stringstream ss;
			StrInfo strInfo;

			if (logAttackRateToFile)
				ss << format("« Recording »\n");
			else
				ss << format("« Monitoring »\n");
			ss << format("« Attack Rate »\n");
			ss << format("\n");
			ss << format("Threshold: %i hits\n") % AttackRateChainHits;

			if (!bufferAttackRate.empty())
			{
				double min = *min_element(bufferAttackRate.begin(), bufferAttackRate.end());
				double max = *max_element(bufferAttackRate.begin(), bufferAttackRate.end());
				double average = 0;
				for (size_t i = 0; i < bufferAttackRate.size(); i++)
					average += bufferAttackRate[i];
				average = average / bufferAttackRate.size();

				ss << format("Min: %0.3fs\n") % min;
				ss << format("Avg: %0.3fs\n") % average;
				ss << format("Max: %0.3fs\n") % max;

				ss << format("\n");
				ss << format("History");
				for (size_t i = 0; i < bufferAttackRate.size(); i++)
					ss << format("\n• %0.3fs") % bufferAttackRate[i];
			}
			else
			{
				ss << format("Minimum: ...\n");
				ss << format("Average: ...\n");
				ss << format("Maximum: ...\n");
			}

			strInfo = StringInfo(ss.str());

			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor, ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
			aRight.x = x - padX * 2 - 5;
		}

		if (logHits)
		{
			stringstream ss;
			StrInfo strInfo;

			if (logHitsToFile)
				ss << format("« Recording »\n");
			else
				ss << format("« Monitoring »\n");

			ss << format("« Damage Hits »\n");
			ss << format("\n");
			ss << format("Counter: %i\n") % threadHitsCounter;
			
			if (!bufferHits.empty())
			{
				//double min = *min_element(bufferHits.begin(), bufferHits.end());
				//double max = *max_element(bufferHits.begin(), bufferHits.end());
				double average = 0;
				for (size_t i = 0; i < bufferHits.size(); i++)
					average += bufferHits[i];
				average = average / bufferHits.size();

				ss << format("Avg: %0.1f\n") % average;
				
				ss << format("\n");
				ss << format("History");
				for (size_t i = 0; i < bufferHits.size(); i++)
					ss << format("\n• %i") % bufferHits[i];
			}
			else
			{
				ss << format("Counter: ...\n");
				ss << format("Average: ...");
			}

			strInfo = StringInfo(ss.str());
			
			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x44000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor, ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
			aRight.x = x - padX * 2 - 5;
		}	
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
		
		if (!font.Init(lineHeight, "Verdana"))
		{
			DbgOut("Could not create Font");
			return false;
		}

		return true;
	}
};
TestDll g_testDll;