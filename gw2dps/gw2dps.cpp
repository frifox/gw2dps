#include "gw2lib.h"
#include "gw2dps.h"
#include "config.h"
#include "keymap.h"
#include "hotkey.h"
#include "preferences.h"

// Settings //
bool killApp = false;

bool help = false;
bool expMode = false;
bool selfFloat = false;
bool selfHealthPercent = true;
bool loopLimiter = true;

bool woldBosses = false;

bool targetSelected = true;
bool targetInfo = false;
bool targetInfoAlt = false;
bool targetLock = false;

bool dpsAllowNegative = false;

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
int wvwBonus = 0;

bool floatAllyNpc = false;
bool floatEnemyNpc = false;
bool floatAllyPlayer = false;
bool floatAllyPlayerProf = false;
bool floatEnemyPlayer = false;
bool floatSiege = false;
int floatRadius = 7000;
bool floatCircles = false;
bool floatType = true; // false = HP; true = Dist;

bool logSpeedometer = false;
bool logSpeedometerEnemy = false;
int logDisplacementValue = 0;
bool logDisplacement = false;
bool logDisplacementEnemy = false;
Vector3 logDisplacementStart = Vector3(0, 0, 0);

#ifdef ARCH_64BIT
uintptr_t hp_shift1 = 0x58;
uintptr_t hp_shift2 = 0x1c0;
uintptr_t hp_shift3 = 0x50;
uintptr_t hp_shift4 = 0x1e8;
uintptr_t hp_shift_cur = 0xc;
uintptr_t hp_shift_max = 0x10;
#else
uintptr_t hp_shift1 = 0x34;
uintptr_t hp_shift2 = 0x178;
uintptr_t hp_shift3 = 0x2c;
uintptr_t hp_shift4 = 0x18c;
uintptr_t hp_shift_cur = 0x8;
uintptr_t hp_shift_max = 0xc;
#endif

DWORD thread_id_hotkey = 0;

// Threads //
#include "thread.Hotkeys.cpp"
#include "thread.Dps.cpp"
#include "thread.KillTimer.cpp"
#include "thread.Hits.cpp"
#include "thread.AttackRate.cpp"
#include "thread.Crits.cpp"
#include "thread.Speedometer.cpp"

// Self
Character me;
Agent meAg = me.GetAgent();

void ESP()
{
	// Element Anchors
	Anchor aLeft, aTopLeft, aTop, aTopRight, aRight, aCenter, aBottom;

	aLeft.x = 100;
	aLeft.y = 75;

	aTopLeft.x = round((GetWindowWidth() / 2 - 316 - 179) / 2 + 316);
	aTopLeft.y = 8;

	aTop.x = round(GetWindowWidth() / 2);
	aTop.y = 1;

	aTopRight.x = round((GetWindowWidth() / 2 - 294 - 179) / 2 + GetWindowWidth() / 2 + 179);
	aTopRight.y = 8;

	aRight.x = GetWindowWidth() - 10;
	aRight.y = 8;

	aCenter.x = round(GetWindowWidth() * float(0.5));
	aCenter.y = round(GetWindowHeight() * float(0.5));

	aBottom.x = round(GetWindowWidth() * float(0.5));
	aBottom.y = round(GetWindowHeight() - float(85));


	// JP Skills
	if (expMode)
	{
		float x = aCenter.x;
		float y = aCenter.y + 28;

		float box = 2;
		float line = 10;

		DrawLine(x - line, y, x + line, y, borderColor);
		DrawLine(x, y - line, x, y + line, borderColor);

		DrawRectFilled(x - box, y - box, box * 2, box * 2, 0xccFF0000);
		DrawRect(x - box, y - box, box * 2, box * 2, borderColor);
	}

	if (selfFloat)
	{
		DWORD color = 0x4433ff00;
		DrawCircleProjected(self.pos, 20.0f, color);
		DrawCircleFilledProjected(self.pos, 20.0f, color - 0x30000000);
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
	me = GetOwnCharacter();
	if (me.IsValid()){
		self.id = GetOwnAgent().GetAgentId();
		self.pos = GetOwnAgent().GetPos();

		self.cHealth = me.GetCurrentHealth();
		self.mHealth = me.GetMaxHealth();
		if (self.mHealth > 0)
			self.pHealth = 100.f * (self.cHealth / self.mHealth);
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

			selected.pos = agLocked.GetPos();

			Character chLocked = agLocked.GetCharacter();
			selected.cHealth = chLocked.GetCurrentHealth();
			selected.mHealth = chLocked.GetMaxHealth();
			if (selected.mHealth > 0)
				selected.pHealth = 100.f * (selected.cHealth / selected.mHealth);
			else
				selected.pHealth = 0;
			selected.lvl = chLocked.GetScaledLevel();
			selected.lvlActual = chLocked.GetLevel();
			selected.breakbar = chLocked.GetBreakbarPercent() * 100;
		}
		else if (agType == GW2::AGENT_TYPE_GADGET) // structure
		{
			selected.valid = true;
			selected.id = agLocked.GetAgentId();
			selected.type = agType;

			selected.pos = agLocked.GetPos();

			unsigned long shift = *(unsigned long*)agLocked.m_ptr;
			shift = *(unsigned long*)(shift + hp_shift1);
			shift = *(unsigned long*)(shift + hp_shift2);
			if (shift)
			{
				selected.cHealth = *(float*)(shift + hp_shift_cur);
				selected.mHealth = *(float*)(shift + hp_shift_max);
			}
			if (selected.mHealth > 0)
				selected.pHealth = 100.f * (selected.cHealth / selected.mHealth);
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

			selected.pos = agLocked.GetPos();

			unsigned long shift = *(unsigned long*)agLocked.m_ptr;
			shift = *(unsigned long*)(shift + hp_shift1);
			shift = *(unsigned long*)(shift + hp_shift3);
			shift = *(unsigned long*)(shift + hp_shift4);
			if (shift)
			{
				selected.cHealth = *(float*)(shift + hp_shift_cur);
				selected.mHealth = *(float*)(shift + hp_shift_max);
			}
			if (selected.mHealth > 0)
				selected.pHealth = 100.f * (selected.cHealth / selected.mHealth);
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

	// compile all agent data
	Floaters floaters;
	Allies allies;
	Agent ag;
	WBosses wbosses;
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

				locked.pos = ag.GetPos();

				Character ch = ag.GetCharacter();
				locked.cHealth = ch.GetCurrentHealth();
				locked.mHealth = ch.GetMaxHealth();
				if (locked.mHealth > 0)
					locked.pHealth = 100.f * (locked.cHealth / locked.mHealth);
				else
					locked.pHealth = 0;
				locked.lvl = ch.GetScaledLevel();
				locked.lvlActual = ch.GetLevel();
			}
			else if (agType == GW2::AGENT_TYPE_GADGET) // structure
			{
				locked.valid = true;
				locked.id = ag.GetAgentId();
				locked.type = agType;

				locked.pos = ag.GetPos();

				unsigned long shift = *(unsigned long*)ag.m_ptr;
				shift = *(unsigned long*)(shift + hp_shift1);
				shift = *(unsigned long*)(shift + hp_shift2);
				if (shift)
				{
					locked.cHealth = *(float*)(shift + hp_shift_cur);
					locked.mHealth = *(float*)(shift + hp_shift_max);
				}
				if (locked.mHealth > 0)
					locked.pHealth = 100.f * (locked.cHealth / locked.mHealth);
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

				locked.pos = ag.GetPos();

				unsigned long shift = *(unsigned long*)ag.m_ptr;
				shift = *(unsigned long*)(shift + hp_shift1);
				shift = *(unsigned long*)(shift + hp_shift3);
				shift = *(unsigned long*)(shift + hp_shift4);
				if (shift)
				{
					locked.cHealth = *(float*)(shift + hp_shift_cur);
					locked.mHealth = *(float*)(shift + hp_shift_max);
				}
				if (locked.mHealth > 0)
					locked.pHealth = 100.f * (locked.cHealth / locked.mHealth);
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

		// Wold Bosses
		if (woldBosses && ag.GetType() == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) {
			WBoss wboss;

			wboss.id = ag.GetAgentId();
			wboss.pos = ag.GetPos();

			unsigned long shift = *(unsigned long*)ag.m_ptr;
			shift = *(unsigned long*)(shift + hp_shift1);
			shift = *(unsigned long*)(shift + hp_shift3);
			shift = *(unsigned long*)(shift + hp_shift4);
			if (shift)
			{
				wboss.cHealth = *(float*)(shift + hp_shift_cur);
				wboss.mHealth = *(float*)(shift + hp_shift_max);
			}
			if (wboss.mHealth > 0)
				wboss.pHealth = 100.f * (wboss.cHealth / wboss.mHealth);
			else
				wboss.pHealth = 0;

			wbosses.list.push_back(wboss);
		}

		// Floaters
		if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege)
		{
			int agType = ag.GetType();

			// NPC & Player
			if (agType == GW2::AGENT_TYPE_CHAR)
			{
				Character ch = ag.GetCharacter();

				// gather data
				Vector3 pos = ag.GetPos();
				float cHealth = ch.GetCurrentHealth();
				float mHealth = ch.GetMaxHealth();
				int attitude = ch.GetAttitude();
				int prof = ch.GetProfession();

				// Filter the dead
				if (cHealth > 0 && mHealth > 1)
				{
					// Filter those out of range
					if (Dist(self.pos, pos) <= floatRadius)
					{
						Float floater;
						floater.pos = pos;
						floater.mHealth = mHealth;
						floater.prof = prof;

						// player vs npc
						if (ch.IsPlayer() && !ch.IsControlled()) // (ignore self)
						{
							// allyPlayer
							if (floatAllyPlayer && attitude == GW2::ATTITUDE_FRIENDLY)
								floaters.allyPlayer.push_back(floater);

							// enemyPlayer
							if (floatEnemyPlayer && attitude == GW2::ATTITUDE_HOSTILE)
								floaters.enemyPlayer.push_back(floater);
						}

						if (!ch.IsPlayer()){
							// allyNpc
							if (floatAllyNpc && (attitude == GW2::ATTITUDE_FRIENDLY || attitude == GW2::ATTITUDE_NEUTRAL))
								floaters.allyNpc.push_back(floater);

							// enemyNpc
							if (floatEnemyNpc && (attitude == GW2::ATTITUDE_HOSTILE || attitude == GW2::ATTITUDE_INDIFFERENT))
								floaters.enemyNpc.push_back(floater);
						}
					}
				}
			}
		}

		// Allies list
		if (alliesList) {
			Character ch = ag.GetCharacter();

			// collect only valid allies (and yourself)
			bool chValid = true;

			if (!ch.IsValid())
				chValid = false;

			//if (ch.IsControlled())
			//chValid = false;

			if (!ch.IsPlayer() || ch.GetAttitude() != GW2::ATTITUDE_FRIENDLY)
				chValid = false;

			// gather char data
			if (chValid){
				Ally ally;
				ally.id = ag.GetAgentId();

				ally.profession = ch.GetProfession();
				ally.mHealth = round(ch.GetMaxHealth() / (100 + wvwBonus) * 100);
				//ally.cHealth = int(ch.GetCurrentHealth());
				//if (ally.mHealth > 0)
				//ally.pHealth = 100.f * (ally.cHealth / ally.mHealth);
				//else
				//ally.pHealth = 0;
				ally.lvl = ch.GetScaledLevel();
				ally.lvlActual = ch.GetLevel();
				ally.name = ch.GetName();

				baseHpReturn base = baseHp(ally.lvl, ally.profession);
				ally.vitality = int(round((ally.mHealth - base.health) / 10));

				switch (ally.profession)
				{
				case GW2::PROFESSION_WARRIOR:
					allies.war.push_back(ally);
					break;
				case GW2::PROFESSION_NECROMANCER:
					allies.necro.push_back(ally);
					break;

				case GW2::PROFESSION_ENGINEER:
					allies.engi.push_back(ally);
					break;
				case GW2::PROFESSION_RANGER:
					allies.ranger.push_back(ally);
					break;
				case GW2::PROFESSION_MESMER:
					allies.mes.push_back(ally);
					break;

				case GW2::PROFESSION_GUARDIAN:
					allies.guard.push_back(ally);
					break;
				case GW2::PROFESSION_THIEF:
					allies.thief.push_back(ally);
					break;
				case GW2::PROFESSION_ELEMENTALIST:
					allies.ele.push_back(ally);
					break;
				case GW2::PROFESSION_REVENANT:
					allies.rev.push_back(ally);
					break;
				}
			}
		}

		// Displacement
		if (logDisplacement)
		{
			if (logDisplacementEnemy)
			{
				if (locked.valid) {
					if (locked.id != bufferDisplacement.id || (bufferDisplacement.start.x == 0 && bufferDisplacement.start.y == 0 && bufferDisplacement.start.z == 0))
					{
						bufferDisplacement.start = locked.pos;
						bufferDisplacement.id = locked.id;
					}
					else
					{
						float displacement = Dist(bufferDisplacement.start, locked.pos);
						bufferDisplacement.dist = int(displacement);
					}
				}
				else
				{
					// reset
					bufferDisplacement = {};
				}
			}
			else
			{
				if (bufferDisplacement.start.x == 0 && bufferDisplacement.start.y == 0 && bufferDisplacement.start.z == 0)
				{
					bufferDisplacement.start = self.pos;
				}
				else
				{
					float displacement = Dist(bufferDisplacement.start, self.pos);
					bufferDisplacement.dist = int(displacement);
				}
			}
		}
		else
		{
			// reset
			bufferDisplacement = {};
		}
	}

	// floaters //

	if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege)
	{
		stringstream ss;
		StrInfo strInfo;

		if (floatCircles)
		{
			float x, y;
			if (floatAllyNpc && floaters.allyNpc.size() > 0)
			{
				for (auto & floater : floaters.allyNpc) {
					if (WorldToScreen(floater.pos, &x, &y))
					{
						stringstream fs;
						if (floatType)
							fs << format("%i") % int(Dist(self.pos, floater.pos));
						else
							fs << format("%i") % floater.mHealth;

						StrInfo fsInfo = StringInfo(fs.str());
						font.Draw(x - fsInfo.x / 2, y - 15, fontColor, fs.str());

						DWORD color = 0x4433ff00;
						DrawCircleProjected(floater.pos, 20.0f, color);
						DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
					}
				}
			}

			if (floatEnemyNpc && floaters.enemyNpc.size() > 0)
			{
				for (auto & floater : floaters.enemyNpc) {
					if (WorldToScreen(floater.pos, &x, &y))
					{
						stringstream fs;
						if (floatType)
							fs << format("%i") % int(Dist(self.pos, floater.pos));
						else
							fs << format("%i") % floater.mHealth;

						StrInfo fsInfo = StringInfo(fs.str());
						font.Draw(x - fsInfo.x / 2, y - 15, fontColor, fs.str());

						DWORD color = 0x44ff3300;
						DrawCircleProjected(floater.pos, 20.0f, color);
						DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
					}
				}
			}

			if (floatAllyPlayer && floaters.allyPlayer.size() > 0)
			{
				for (auto & floater : floaters.allyPlayer) {
					if (WorldToScreen(floater.pos, &x, &y))
					{
						stringstream fs;
						if (floatType)
							fs << format("%i") % int(Dist(self.pos, floater.pos));
						else
							fs << format("%i") % floater.mHealth;

						StrInfo fsInfo = StringInfo(fs.str());
						font.Draw(x - fsInfo.x / 2, y - 15, fontColor, fs.str());

						DWORD color = 0x4433ff00;
						DrawCircleProjected(floater.pos, 20.0f, color);
						DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
					}
				}
			}

			if (floatEnemyPlayer && floaters.enemyPlayer.size() > 0)
			{
				for (auto & floater : floaters.enemyPlayer) {
					if (WorldToScreen(floater.pos, &x, &y))
					{
						stringstream fs;
						if (floatType)
							fs << format("%i") % int(Dist(self.pos, floater.pos));
						else
							fs << format("%i") % floater.mHealth;

						StrInfo fsInfo = StringInfo(fs.str());
						font.Draw(x - fsInfo.x / 2, y - 15, fontColor, fs.str());

						DWORD color = 0x44ff3300;
						DrawCircleProjected(floater.pos, 20.0f, color);
						DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
					}
				}
			}

			if (floatSiege && floaters.siege.size() > 0)
			{
				for (auto & floater : floaters.siege) {
					//DWORD color = 0x44ff3300;
					//DrawCircleProjected(floater.pos, 20.0f, color);
					//DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
				}
			}
		}

		ss << format("R: %i") % floatRadius;

		if (floatAllyNpc)
			ss << format(" | AllyNPCs: %i") % floaters.allyNpc.size();

		if (floatEnemyNpc)
			ss << format(" | FoeNPCs: %i") % floaters.enemyNpc.size();

		if (floatAllyPlayer)
			ss << format(" | Allies: %i") % floaters.allyPlayer.size();

		if (floatEnemyPlayer)
			ss << format(" | Foes: %i") % floaters.enemyPlayer.size();

		if (floatSiege)
			ss << format(" | Siege: %i") % floaters.siege.size();

		strInfo = StringInfo(ss.str());
		float x = round(aTop.x - strInfo.x / 2);
		float y = round(aTop.y);

		DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
		DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
		font.Draw(x, y, fontColor - (floatCircles ? 0x00aa0000 : 0), ss.str());

		aTop.y += strInfo.y + padY * 2;

		if (floatAllyPlayerProf)
		{
			int prof[10] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			for (auto & ally : floaters.allyPlayer) {
				prof[ally.prof]++;
			}

			ss.str("");
			ss << format("War: %i") % prof[2];
			ss << format(" | Guard: %i") % prof[1];
			ss << format(" | Ele: %i") % prof[6];
			ss << format(" | Thief: %i") % prof[5];
			ss << format(" | Mes: %i") % prof[7];
			ss << format(" | Engi: %i") % prof[3];
			ss << format(" | Ranger: %i") % prof[4];
			ss << format(" | Necro: %i") % prof[8];
			ss << format(" | Rev: %i") % prof[9];

			strInfo = StringInfo(ss.str());
			float x = round(aTop.x - strInfo.x / 2);
			float y = round(aTop.y);

			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor - (floatCircles ? 0x00aa0000 : 0), ss.str());

			aTop.y += strInfo.y + padY * 2;
		}
	}

	// Bottom Element //
	{
		stringstream ss;
		StrInfo strInfo;

		if (selfHealthPercent && self.alive)
		{
			ss << format("%i") % (int)self.pHealth;

			strInfo = StringInfo(ss.str());
			float x = round(aBottom.x - strInfo.x / 2);
			float y = round(aBottom.y - lineHeight);

			//DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			//DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor, ss.str());
		}
	}

	// Left Element //
	{
		if (alliesList)
		{
			stringstream ss;
			stringstream sp;
			stringstream sn;
			stringstream sh;
			stringstream sv;

			ss << format("Nearby Ally Players (WvW HP Bonus: %i%s)") % wvwBonus % "%%";
			sp << format("Class");
			sn << format("Name");
			sh << format("Health");
			sv << format("Vitality");

			bool listEmpty = true;
			if (!allies.war.empty())
			{
				for (auto & ally : allies.war) {
					sp << format("\nWar:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (!allies.guard.empty())
			{
				for (auto & ally : allies.guard) {
					sp << format("\nGuard:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}

			if (!allies.ele.empty())
			{
				for (auto & ally : allies.ele) {
					sp << format("\nEle:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (!allies.mes.empty())
			{
				for (auto & ally : allies.mes) {
					sp << format("\nMes:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}

			if (!allies.thief.empty())
			{
				for (auto & ally : allies.thief) {
					sp << format("\nThief:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (!allies.ranger.empty())
			{
				for (auto & ally : allies.ranger) {
					sp << format("\nRanger:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (!allies.engi.empty())
			{
				for (auto & ally : allies.engi) {
					sp << format("\nEngi:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (!allies.necro.empty())
			{
				for (auto & ally : allies.necro) {
					sp << format("\nNecro:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (!allies.rev.empty())
			{
				for (auto & ally : allies.rev) {
					sp << format("\nRev:");
					sn << format("\n%s") % ally.name;
					sh << format("\n%i hp") % ally.mHealth;
					sv << format("\n%+i") % ally.vitality;
				}
				listEmpty = false;
			}
			if (listEmpty)
			{
				sp << format("\n...");
				sn << format("\n...");
				sh << format("\n...");
				sv << format("\n...");
			}


			// CharName max width
			stringstream sx;
			sx << "WWWWWWWWWWWWWWWWWWW";
			StrInfo strInfo;
			strInfo = StringInfo(sx.str());

			float spOffset = 0;
			float snOffset = spOffset + 65;
			float shOffset = snOffset + strInfo.x;
			float svOffset = shOffset + 85;
			float sxOffset = svOffset + 60;

			float x = round(aLeft.x);
			float y = round(aLeft.y);

			strInfo = StringInfo(sp.str());
			int lineCount = int(strInfo.lineCount) + 2;

			// render the list
			DrawRectFilled(x - padX, y - padY, sxOffset + padX * 2, float(lineCount * lineHeight) + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, sxOffset + padX * 2, float(lineCount * lineHeight) + padY * 2, borderColor);

			int lineShiftY = 2;
			for (int i = 3; i < lineCount; i++) {
				DrawLine(x - padX, y - padY + i * lineHeight + lineShiftY, x + sxOffset + padX, y - padY + i * lineHeight + lineShiftY, borderColor);
			}
			font.Draw(x + spOffset, y, fontColor, ss.str()); y += 2 * lineHeight;
			font.Draw(x + spOffset, y, fontColor, sp.str());
			font.Draw(x + snOffset, y, fontColor, sn.str());
			font.Draw(x + shOffset, y, fontColor, sh.str());
			font.Draw(x + svOffset, y, fontColor, sv.str());
		}
	}

	// TopLeft Element //
	{
		if (targetSelected)
		{
			if (targetLock && selected.valid && locked.valid && selected.id == locked.id)
			{

				stringstream ss;
				StrInfo strInfo;

				ss << format("Selected & Locked: %i / %i [%i%s]") % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth % "%%";

				strInfo = StringInfo(ss.str());
				float x = round(aTopLeft.x - strInfo.x / 2);
				float y = round(aTopLeft.y);

				DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
				DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
				font.Draw(x, y, fontColor, ss.str());

				// Prepare for Next Element
				aTopLeft.y += strInfo.lineCount * lineHeight + padY * 2;
			}
			else
			{
				if (selected.valid)
				{
					stringstream ss;
					StrInfo strInfo;

					ss << format("Selected: %i / %i [%i%s]") % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth % "%%";

					strInfo = StringInfo(ss.str());
					float x = round(aTopLeft.x - strInfo.x / 2);
					float y = round(aTopLeft.y);

					DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
					DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
					font.Draw(x, y, fontColor, ss.str());

					// Prepare for Next Element
					aTopLeft.y += strInfo.lineCount * lineHeight + padY * 2;
				}

				if (targetLock && locked.valid)
				{
					stringstream ss;
					StrInfo strInfo;

					ss << format("Selected & Locked: %i / %i [%i%s]") % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth % "%%";

					strInfo = StringInfo(ss.str());
					float x = round(aTopLeft.x - strInfo.x / 2);
					float y = round(aTopLeft.y);

					DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
					DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
					font.Draw(x, y, fontColor, ss.str());

					// Prepare for Next Element
					aTopLeft.y += strInfo.lineCount * lineHeight + padY * 2;
				}
			}

			if (targetInfo && selected.valid)
			{
				//aTopLeft.y += lineHeight;

				stringstream ss;
				StrInfo strInfo;

				if (targetInfoAlt)
				{
					if (agLocked.GetType() == GW2::AGENT_TYPE_CHAR)
					{
						GW2::CharacterStats stats = agLocked.GetCharacter().GetStats();

						ss << format("Power - %i") % stats.power;
						ss << format("\nPrecision - %i") % stats.precision;
						ss << format("\nToughness - %i") % stats.toughness;
						ss << format("\nVitality - %i") % stats.vitality;
						ss << format("\nFerocity - %i") % stats.ferocity;
						ss << format("\nHealing - %i") % stats.healing;
						ss << format("\nCondition - %i") % stats.condition;

						ss << format("\n");
						ss << format("\n(Agent: %p)") % (void**)agLocked.m_ptr;
					}
				}
				else
				{
					ss << format("Distance: %i\n") % int(Dist(self.pos, selected.pos));
					if (agLocked.GetCharacter().GetBreakbarState() != GW2::BREAKBAR_STATE_NONE)
						ss << format("BB: %3.05f%s") % selected.breakbar % "%%";
				}

				strInfo = StringInfo(ss.str());
				float x = round(aTopLeft.x - strInfo.x / 2);
				float y = round(aTopLeft.y);

				DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
				DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
				font.Draw(x, y, fontColor, ss.str());

				// Prepare for Next Element
				ss.str("");
				aTopLeft.y += strInfo.lineCount * lineHeight + padY * 2;
			}
		}
	}

	// Top Elements //
	{

		// World Boss
		if (woldBosses)
		{
			stringstream fs;
			fs << format("Wold Bosses: %i") % wbosses.list.size();

			StrInfo strInfo = StringInfo(fs.str());
			float lx = 12;
			float ly = 32;

			DrawRectFilled(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(lx, ly, fontColor, fs.str());

			ly = ly + 12;

			if (wbosses.list.size() > 0) {
				float fx, fy;
				for (auto & wboss : wbosses.list) {
					// floater
					if (WorldToScreen(wboss.pos, &fx, &fy))
					{
						stringstream fs;
						//fs << format("%i / %i") % wboss.cHealth % wboss.mHealth;
						fs << format("[%i] %i") % wboss.id % int(Dist(self.pos, wboss.pos));

						StrInfo strInfo = StringInfo(fs.str());
						fx = round(fx - strInfo.x / 2);
						fy = round(fy - 15);

						DrawRectFilled(fx - padX, fy - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
						DrawRect(fx - padX, fy - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
						font.Draw(fx, fy, fontColor, fs.str());
					}

					// list
					{
						stringstream fs;
						fs << format("[%i] %i / %i (%i)") % wboss.id % (int)wboss.cHealth % (int)wboss.mHealth % int(Dist(self.pos, wboss.pos));

						StrInfo strInfo = StringInfo(fs.str());
						ly = round(ly + strInfo.y + padY);

						DrawRectFilled(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
						DrawRect(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
						font.Draw(lx, ly, fontColor, fs.str());
					}
				}
			}
		}

		if (logSpeedometer) {
			stringstream ss;
			StrInfo strInfo;

			ss << format("Speed: ");

			if (!bufferSpeedometer.empty())
			{
				double average[2] {};
				size_t samples = 0;

				// Speed ~ .5s
				samples = 5;
				if (samples > bufferSpeedometer.size())
					samples = bufferSpeedometer.size();
				average[0] = 0;
				for (size_t i = 0; i < samples; i++)
					average[0] += bufferSpeedometer[i];
				average[0] = average[0] / samples * (1000 / 100);

				// Speed ~ 3s
				samples = 30;
				if (samples > bufferSpeedometer.size())
					samples = bufferSpeedometer.size();
				average[1] = 0;
				for (size_t i = 0; i < samples; i++)
					average[1] += bufferSpeedometer[i];
				average[1] = average[1] / samples * (1000 / 100);

				// Prepare String
				ss << format("%i in/s, %i in/s") % int(average[0]) % int(average[1]);
			}
			else
			{
				ss << format("0 in/s, 0 in/s");
			}

			strInfo = StringInfo(ss.str());
			float x = round(aTop.x - strInfo.x / 2);
			float y = round(aTop.y);

			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor - (logSpeedometerEnemy ? 0x00aa0000 : 0), ss.str());

			aTop.y += strInfo.y + padY * 2;
		}

		if (logDisplacement)
		{
			stringstream ss;
			StrInfo strInfo;

			ss << format("Distance: %i") % bufferDisplacement.dist;

			strInfo = StringInfo(ss.str());
			float x = round(aTop.x - strInfo.x / 2);
			float y = round(aTop.y);

			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor - (logDisplacementEnemy ? 0x00aa0000 : 0), ss.str());

			aTop.y += strInfo.y + padY * 2;

		}
		else
		{
			if (logDisplacementStart.x != 0 || logDisplacementStart.y != 0 || logDisplacementStart.z != 0)
				logDisplacementStart = Vector3(0, 0, 0);
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
				average[1] = average[1] / samples * (1000 / 250);

				// DP5s
				samples = 20; // 5s/250ms=20
				if (samples > bufferDps.size())
					samples = bufferDps.size();
				average[5] = 0;
				for (size_t i = 0; i < samples; i++)
					average[5] += bufferDps[i];
				average[5] = average[5] / samples * (1000 / 250);

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
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
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
				ss << format("Timer: %s") % SecondsToString(bufferKillTimer.time);
				if (logKillTimerDetails)
					ss << format("\nDPS: %0.2f") % bufferKillTimer.dps;
			}
			else
			{
				ss << format("Timer: 0.0s");
				if (logKillTimerDetails)
					ss << format("\nDPS: 0.0");
			}

			strInfo = StringInfo(ss.str());
			float x = 0;
			float y = round(aTopRight.y);
			if (logDps)
				x = round(aTopRight.x - strInfo.x - padX); // perma anchor offset with logDps
			else
				x = round(aTopRight.x - strInfo.x / 2); // center otherwise


			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
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
				ss << format(".: Recording :.\n");
			else
				ss << format(".: Monitoring :.\n");
			ss << format(".: Attack Rate :.\n");
			ss << format("\n");

			if (!bufferAttackRate.empty())
			{
				//double min = *min_element(bufferAttackRate.begin(), bufferAttackRate.end());
				//double max = *max_element(bufferAttackRate.begin(), bufferAttackRate.end());
				double average = 0;
				for (size_t i = 0; i < bufferAttackRate.size(); i++)
					average += bufferAttackRate[i];
				average = average / bufferAttackRate.size();

				ss << format("Counter: %i\n") % threadAttackRateCounter;
				//ss << format("Min: %0.3fs\n") % min;
				ss << format("Average: %0.3fs\n") % average;
				//ss << format("Max: %0.3fs\n") % max;

				if (logAttackRateDetails)
				{
					ss << format("\n");
					ss << format("History\n");
					for (size_t i = 0; i < bufferAttackRate.size(); i++)
						ss << format("\u2022 %0.3fs\n") % bufferAttackRate[i];
				}
			}
			else
			{
				ss << format("Counter: ...\n");
				//ss << format("Minimum: ...\n");
				ss << format("Average: ...\n");
				//ss << format("Maximum: ...\n");

				if (logAttackRateDetails)
				{
					ss << format("\n");
					ss << format("History\n");
					ss << format("\u2022 ...\n");
				}
			}
			ss << format("\n");
			ss << format("Threshold: %i hits\n") % AttackRateChainHits;

			strInfo = StringInfo(ss.str());

			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor - (logAttackRateToFile ? 0x00aa0000 : 0), ss.str());

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
				ss << format(".: Recording :.\n");
			else
				ss << format(".: Monitoring :.\n");

			ss << format(".: Damage Hits :.\n");
			ss << format("\n");

			if (!bufferHits.empty())
			{
				//double min = *min_element(bufferHits.begin(), bufferHits.end());
				//double max = *max_element(bufferHits.begin(), bufferHits.end());
				double average = 0;
				for (size_t i = 0; i < bufferHits.size(); i++)
					average += bufferHits[i];
				average = average / bufferHits.size();

				ss << format("Counter: %i\n") % threadHitsCounter;
				ss << format("Average: %0.1f\n") % average;

				if (logHitsDetails)
				{
					ss << format("\n");
					ss << format("History\n");
					for (size_t i = 0; i < bufferHits.size(); i++)
						ss << format("\u2022 %i\n") % bufferHits[i];
				}
			}
			else
			{
				ss << format("Counter: ...\n");
				ss << format("Average: ...\n");

				if (logHitsDetails)
				{
					ss << format("\n");
					ss << format("History\n");
					ss << format("\u2022 ...\n");
				}
			}

			strInfo = StringInfo(ss.str());


			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor - (logHitsToFile ? 0x00aa0000 : 0), ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
			aRight.x = x - padX * 2 - 5;
		}

		if (logCrits)
		{
			stringstream ss;
			StrInfo strInfo;

			if (logCritsToFile)
				ss << format(".: Recording :.\n");
			else
				ss << format(".: Monitoring :.\n");

			ss << format(".: Crit Chance :.\n");
			ss << format("\n");

			ss << format("Samples: %i\n") % (logCritsGlances + logCritsNormals + logCritsCrits);
			ss << format("Glances: %i\n") % logCritsGlances;
			ss << format("Normals: %i\n") % logCritsNormals;
			ss << format("Crits: %i\n") % logCritsCrits;
			ss << format("\n");


			float critA = 0;
			if ((logCritsGlances + logCritsNormals + logCritsCrits) > 0)
				critA = logCritsCrits / (logCritsGlances + logCritsNormals + logCritsCrits + 0.f) * 100;

			float critB = 0;
			if ((logCritsNormals + logCritsCrits) > 0)
				critB = logCritsCrits / (logCritsNormals + logCritsCrits + 0.f) * 100;

			ss << format(".: Crit Chance :.\n");
			ss << format("GlIncl: %0.4f\n") % critA;
			ss << format("GlExcl: %0.4f\n") % critB;

			ss << format("\n");
			ss << format("Sample Hit: %i\n") % logCritsSample;


			strInfo = StringInfo(ss.str());


			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			// Draw
			DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
			DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
			font.Draw(x, y, fontColor - (logCritsToFile ? 0x00aa0000 : 0), ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
			aRight.x = x - padX * 2 - 5;
		}
	}

	if (help)
	{
		stringstream ss;

		//ss << format("[%i] Exp Mode Crosshair (Alt H)\n") % expMode;
		//ss << format("\n");
		ss << format("[%i] Self Health Percent (%s)\n") % selfHealthPercent % get_key_description("Hotkeys.SELF_HEALTH_PERCENT");
		ss << format("\n");
		ss << format("[%i] Selected/Locked Target HP (%s)\n") % targetSelected % get_key_description("Hotkeys.TARGET_SELECTED");
		ss << format("[%i] Selected Target Details (%s)\n") % targetInfo % get_key_description("Hotkeys.TARGET_INFO");
		ss << format("[%i] Lock On Target (%s)\n") % targetLock % get_key_description("Hotkeys.TARGET_LOCK");
		ss << format("[%i] Allow Negative DPS (%s)\n") % dpsAllowNegative % get_key_description("Hotkeys.DPS_ALLOW_NEGATIVE");
		ss << format("\n");
		ss << format("[%i] DPS Meter (%s)\n") % logDps % get_key_description("Hotkeys.LOG_DPS");
		ss << format("[%i] DPS Meter History (%s)\n") % logDpsDetails % get_key_description("Hotkeys.LOG_DPS_DETAILS");
		ss << format("\n");
		ss << format("[%i] Kill Timer (%s)\n") % logKillTimer % get_key_description("Hotkeys.LOG_KILL_TIMER");
		ss << format("[%i] Kill Timer Details (%s)\n") % logKillTimerDetails % get_key_description("Hotkeys.LOG_KILL_TIMER_DETAILS");
		//ss << format("[%i] Kill Timer Writes to a File (Alt Num4)\n") % logKillTimerToFile;
		ss << format("\n");
		ss << format("[%i] Monitor Hits (%s)\n") % logHits % get_key_description("Hotkeys.LOG_HITS");
		ss << format("[%i] Show Hits History (%s)\n") % logHitsDetails % get_key_description("Hotkeys.LOG_HITS_DETAILS");
		ss << format("[%i] Record Hits to File (%s)\n") % logHitsToFile % get_key_description("Hotkeys.LOG_HITS_TO_FILE");
		ss << format("\n");
		ss << format("[%i] Monitor Attack Rate (%s)\n") % logAttackRate % get_key_description("Hotkeys.LOG_ATTACK_RATE");
		ss << format("[%i] Show Attack Rate History (%s)\n") % logAttackRateDetails % get_key_description("Hotkeys.LOG_ATTACK_RATE_DETAILS");
		ss << format("[%i] Record Attack Rate to File (%s)\n") % logAttackRateToFile % get_key_description("Hotkeys.LOG_ATTACK_RATE_TO_FILE");
		ss << format("[%i] Adjust Attack Rate Threshold +(%s) and -(%s)\n") % AttackRateChainHits % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_MORE") % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_LESS");
		ss << format("\n");
		ss << format("[%i] Nearby Ally Players List (%s)\n") % alliesList % get_key_description("Hotkeys.ALLIES_LIST");
		ss << format("[%i] Adjust WvW HP Bonus +(%s) and -(%s)\n") % wvwBonus % get_key_description("Hotkeys.WVW_BONUS_MORE") % get_key_description("Hotkeys.WVW_BONUS_LESS");
		ss << format("\n");
		ss << format("[%i] Count Ally NPCs (%s)\n") % floatAllyNpc % get_key_description("Hotkeys.FLOAT_ALLY_NPC");
		ss << format("[%i] Count Enemy NPCs (%s)\n") % floatEnemyNpc % get_key_description("Hotkeys.FLOAT_ENEMY_NPC");
		ss << format("[%i] Count Ally Players (%s)\n") % floatAllyPlayer % get_key_description("Hotkeys.FLOAT_ALLY_PLAYER");
		ss << format("[%i] Count Enemy Players (%s)\n") % floatEnemyPlayer % get_key_description("Hotkeys.FLOAT_ENEMY_PLAYER");
		//ss << format("[%i] Count Siege (Alt 5)\n") % floatSiege;
		ss << format("[%i] Show/Hide Floaters below counted (%s)\n") % floatCircles % get_key_description("Hotkeys.FLOAT_CIRCLES");
		ss << format("[%i] Floaters show Max HP / Distance (%s)\n") % floatType % get_key_description("Hotkeys.FLOAT_TYPE");
		ss << format("\n");
		ss << format("[%i] Speedometer (%s)\n") % logSpeedometer % get_key_description("Hotkeys.LOG_SPEEDOMETER");
		ss << format("[%i] Speedometer for Self/Enemy (%s)\n") % logSpeedometerEnemy % get_key_description("Hotkeys.LOG_SPEEDOMETER_ENEMY");
		ss << format("[%i] Measure Distance (%s)\n") % logDisplacement % get_key_description("Hotkeys.LOG_DISPLACEMENT");
		ss << format("[%i] Distance for Self/Enemy (%s)\n") % logDisplacementEnemy % get_key_description("Hotkeys.LOG_DISPLACEMENT_ENEMY");

		StrInfo strInfo;
		strInfo = StringInfo(ss.str());
		float x = round(aCenter.x - strInfo.x / 2);
		float y = round(aCenter.y - strInfo.y / 2);

		DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - 0x22000000);
		DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
		font.Draw(x, y, fontColor - (!loopLimiter ? 0x00aa0000 : 0), ss.str());
	}
}


void load_preferences() {
	expMode = Str2Bool(read_config_value("Preferences.EXP_MODE"));
	selfFloat = Str2Bool(read_config_value("Preferences.SELF_FLOAT"));
	loopLimiter = Str2Bool(read_config_value("Preferences.LOOP_LIMITER"), true);
	selfHealthPercent = Str2Bool(read_config_value("Preferences.SELF_HEALTH_PERCENT"), true);
	woldBosses = Str2Bool(read_config_value("Preferences.WORLD_BOSSES"));
	targetSelected = Str2Bool(read_config_value("Preferences.TARGET_SELECTED"), true);
	targetInfo = Str2Bool(read_config_value("Preferences.TARGET_INFO"));
	targetInfoAlt = Str2Bool(read_config_value("Preferences.TARGET_INFO_ALT"));
	targetLock = Str2Bool(read_config_value("Preferences.TARGET_LOCK"));
	dpsAllowNegative = Str2Bool(read_config_value("Preferences.DPS_ALLOW_NEGATIVE"));
	logDps = Str2Bool(read_config_value("Preferences.LOG_DPS"), true);
	logDpsDetails = Str2Bool(read_config_value("Preferences.LOG_DPS_DETAILS"));
	logKillTimer = Str2Bool(read_config_value("Preferences.LOG_KILL_TIMER"));
	logKillTimerDetails = Str2Bool(read_config_value("Preferences.LOG_KILL_TIMER_DETAILS"));
	logKillTimerToFile = Str2Bool(read_config_value("Preferences.LOG_KILL_TIMER_TO_FILE"));
	logHits = Str2Bool(read_config_value("Preferences.LOG_HITS"));
	logHitsDetails = Str2Bool(read_config_value("Preferences.LOG_HITS_DETAILS"));
	logHitsToFile = Str2Bool(read_config_value("Preferences.LOG_HITS_TO_FILE"));
	logAttackRate = Str2Bool(read_config_value("Preferences.LOG_ATTACK_RATE"));
	logAttackRateDetails = Str2Bool(read_config_value("Preferences.LOG_ATTACK_RATE_DETAILS"));
	logAttackRateToFile = Str2Bool(read_config_value("Preferences.LOG_ATTACK_RATE_TO_FILE"));
	logCrits = Str2Bool(read_config_value("Preferences.LOG_CRITS"));
	alliesList = Str2Bool(read_config_value("Preferences.ALLIES_LIST"));
	floatCircles = Str2Bool(read_config_value("Preferences.FLOAT_CIRCLES"));
	floatType = Str2Bool(read_config_value("Preferences.FLOAT_TYPE"), true);
	floatAllyNpc = Str2Bool(read_config_value("Preferences.FLOAT_ALLY_NPC"));
	floatEnemyNpc = Str2Bool(read_config_value("Preferences.FLOAT_ENEMY_NPC"));
	floatAllyPlayer = Str2Bool(read_config_value("Preferences.FLOAT_ALLY_PLAYER"));
	floatAllyPlayerProf = Str2Bool(read_config_value("Preferences.FLOAT_ALLY_PLAYER_PROF"));
	floatEnemyPlayer = Str2Bool(read_config_value("Preferences.FLOAT_ENEMY_PLAYER"));
	floatSiege = Str2Bool(read_config_value("Preferences.FLOAT_SIEGE"));
	logSpeedometer = Str2Bool(read_config_value("Preferences.LOG_SPEEDOMETER"));
	logSpeedometerEnemy = Str2Bool(read_config_value("Preferences.LOG_SPEEDOMETER_ENEMY"));
	logDisplacement = Str2Bool(read_config_value("Preferences.LOG_DISPLACEMENT"));
	logDisplacementEnemy = Str2Bool(read_config_value("Preferences.LOG_DISPLACEMENT_ENEMY"));
	floatRadius = Str2Int(read_config_value("Preferences.FLOAT_RADIUS"), 7000);
	wvwBonus = Str2Int(read_config_value("Preferences.WVW_BONUS"));
	AttackRateChainHits = Str2Int(read_config_value("Preferences.ATTACKRATE_CHAIN_HITS"), 1);
	logCritsSample = Str2Int(read_config_value("Preferences.LOG_CRITS_SAMPLE"));
}

void save_preferences() {
	write_config_value("Preferences.EXP_MODE", Bool2Str(expMode));
	write_config_value("Preferences.SELF_FLOAT", Bool2Str(selfFloat));
	write_config_value("Preferences.LOOP_LIMITER", Bool2Str(loopLimiter));
	write_config_value("Preferences.SELF_HEALTH_PERCENT", Bool2Str(selfHealthPercent));
	write_config_value("Preferences.WORLD_BOSSES", Bool2Str(woldBosses));
	write_config_value("Preferences.TARGET_SELECTED", Bool2Str(targetSelected));
	write_config_value("Preferences.TARGET_INFO", Bool2Str(targetInfo));
	write_config_value("Preferences.TARGET_INFO_ALT", Bool2Str(targetInfoAlt));
	write_config_value("Preferences.TARGET_LOCK", Bool2Str(targetLock));
	write_config_value("Preferences.DPS_ALLOW_NEGATIVE", Bool2Str(dpsAllowNegative));
	write_config_value("Preferences.LOG_DPS", Bool2Str(logDps));
	write_config_value("Preferences.LOG_DPS_DETAILS", Bool2Str(logDpsDetails));
	write_config_value("Preferences.LOG_KILL_TIMER", Bool2Str(logKillTimer));
	write_config_value("Preferences.LOG_KILL_TIMER_DETAILS", Bool2Str(logKillTimerDetails));
	write_config_value("Preferences.LOG_KILL_TIMER_TO_FILE", Bool2Str(logKillTimerToFile));
	write_config_value("Preferences.LOG_HITS", Bool2Str(logHits));
	write_config_value("Preferences.LOG_HITS_DETAILS", Bool2Str(logHitsDetails));
	write_config_value("Preferences.LOG_HITS_TO_FILE", Bool2Str(logHitsToFile));
	write_config_value("Preferences.LOG_ATTACK_RATE", Bool2Str(logAttackRate));
	write_config_value("Preferences.LOG_ATTACK_RATE_DETAILS", Bool2Str(logAttackRateDetails));
	write_config_value("Preferences.LOG_ATTACK_RATE_TO_FILE", Bool2Str(logAttackRateToFile));
	write_config_value("Preferences.LOG_CRITS", Bool2Str(logCrits));
	write_config_value("Preferences.ALLIES_LIST", Bool2Str(alliesList));
	write_config_value("Preferences.FLOAT_CIRCLES", Bool2Str(floatCircles));
	write_config_value("Preferences.FLOAT_TYPE", Bool2Str(floatType));
	write_config_value("Preferences.FLOAT_ALLY_NPC", Bool2Str(floatAllyNpc));
	write_config_value("Preferences.FLOAT_ENEMY_NPC", Bool2Str(floatEnemyNpc));
	write_config_value("Preferences.FLOAT_ALLY_PLAYER", Bool2Str(floatAllyPlayer));
	write_config_value("Preferences.FLOAT_ALLY_PLAYER_PROF", Bool2Str(floatAllyPlayerProf));
	write_config_value("Preferences.FLOAT_ENEMY_PLAYER", Bool2Str(floatEnemyPlayer));
	write_config_value("Preferences.FLOAT_SIEGE", Bool2Str(floatSiege));
	write_config_value("Preferences.LOG_SPEEDOMETER", Bool2Str(logSpeedometer));
	write_config_value("Preferences.LOG_SPEEDOMETER_ENEMY", Bool2Str(logSpeedometerEnemy));
	write_config_value("Preferences.LOG_DISPLACEMENT", Bool2Str(logDisplacement));
	write_config_value("Preferences.LOG_DISPLACEMENT_ENEMY", Bool2Str(logDisplacementEnemy));
	write_config_value("Preferences.FLOAT_RADIUS", Int2Str(floatRadius));
	write_config_value("Preferences.WVW_BONUS", Int2Str(wvwBonus));
	write_config_value("Preferences.ATTACKRATE_CHAIN_HITS", Int2Str(AttackRateChainHits));
	write_config_value("Preferences.LOG_CRITS_SAMPLE", Int2Str(logCritsSample));
	save_config();
}


void chat_log(wchar_t *wtxt) {
    size_t len = wcslen(wtxt) + 1;

    char *txt = new char[len];
    memset(txt, 0, len);
    wcstombs(txt, wtxt, len);

    //font.Draw(10, 10, 0xffffffff, "%s", txt);

    delete txt;
}


void GW2LIB::gw2lib_main()
{
	locale::global(locale("en-US"));

	init_keymap();
	init_config();
	load_preferences();

	EnableEsp(ESP);
    SetGameHook(ChatHook, chat_log);

	thread t1(&threadHotKeys);
	thread t2(&threadDps);
	thread t3(&threadKillTimer);
	thread t4(&threadHits);
	thread t5(&threadAttackRate);
	thread t6(&threadCrits);
	thread t7(&threadSpeedometer);

	if (!font.Init(lineHeight, "Verdana"))
	{
		//DbgOut("could not create font");
		return;
	}

	// wait for exit hotkey
	while (GetAsyncKeyState(VK_F12) >= 0)
		Sleep(1);

	close_config();

	// make threads clean-up first before interrupting
	PostThreadMessage(thread_id_hotkey, WM_USER, NULL, NULL);

	// self destruct sequence
	t1.interrupt(); //t1.join();
	t2.interrupt(); //t2.join();
	t3.interrupt(); //t3.join();
	t4.interrupt(); //t4.join();
	t5.interrupt(); //t5.join();
	t6.interrupt(); //t6.join();
	t7.interrupt(); //t7.join();

	Sleep(1000);
	return;
}
