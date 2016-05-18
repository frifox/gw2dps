#include "gw2lib.h"
#include "gw2dps.h"
#include "config.h"
#include "keymap.h"
#include "hotkey.h"
#include "preferences.h"

#include "hacklib/Main.h"

// Threads //
#include "thread.Hotkeys.cpp"
#include "thread.Dps.cpp"
#include "thread.KillTimer.cpp"
#include "thread.Hits.cpp"
#include "thread.AttackRate.cpp"
#include "thread.Crits.cpp"
#include "thread.Speedometer.cpp"


FloatColor GetFloatColor(const Agent &ag) {
    FloatColor ret = COLOR_NONE;
    if (!ag.IsValid()) return ret;

    if (ag.GetCategory() == GW2::AGENT_CATEGORY_KEYFRAMED) {
        ret = COLOR_OBJECT;
    } else {
        Character ch = ag.GetCharacter();
        if (!ch.IsValid()) return ret;

        GW2::Attitude att = ch.GetAttitude();

        if (ch.IsPlayer()) {
            switch (att) {
            case GW2::ATTITUDE_FRIENDLY: ret = COLOR_PLAYER_ALLY; break;
            case GW2::ATTITUDE_HOSTILE:  ret = COLOR_PLAYER_FOE; break;
            }
        } else {
            switch (att) {
            case GW2::ATTITUDE_FRIENDLY:    ret = COLOR_NPC_ALLY; break;
            case GW2::ATTITUDE_HOSTILE:     ret = COLOR_NPC_FOE; break;
            case GW2::ATTITUDE_INDIFFERENT: ret = COLOR_NPC_INDIFF; break;
            case GW2::ATTITUDE_NEUTRAL:     ret = COLOR_NPC_NEUTRAL; break;
            }
        }
    }

    return ret;
}

void DrawAgentPath(const Agent &ag) {
    GW2::AgentCategory agcat = ag.GetCategory();
    if (agcat != GW2::AGENT_CATEGORY_CHAR) return;

    Character ch = ag.GetCharacter();
    if (!ch.IsValid() || !ch.IsAlive()) return;

    int agid = ag.GetAgentId();
    if (agPaths.find(agid) == agPaths.end()) {
        agPaths[agid] = circular_buffer<Vector3>(150);
    }

    size_t pathSize = agPaths[agid].size();
    Vector3 agpos = ag.GetPos();

    if (pathSize >= 2) {
        Vector3 prev = agPaths[agid][pathSize - 1];
        if (prev != agpos) {
            agPaths[agid].push_back(agpos);
        }

        for (size_t i = 0; i < pathSize - 1; i++) {
            Vector3 pos1 = agPaths[agid][i];
            Vector3 pos2 = agPaths[agid][i + 1];
            FloatColor color = GetFloatColor(ag);
            DrawLineProjected(pos1, pos2, color | 0xff000000);
        }
    } else {
        agPaths[agid].push_front(agpos);
    }
}

void ESP()
{
    if (IsInterfaceHidden() || IsMapOpen() || IsInCutscene()) return;

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

    // Font Draw Debug
    if (0) {
        stringstream ss;
        ss << format("Selected: 18,140 / 18,140 [100%%]");
        ss << format("Locked: 18,140 / 18,140 [100%%]");

        Vector2 strInfo;
        strInfo = font.TextInfo(ss.str());
        float x = 0;
        float y = float(strInfo.y + 1);
        padX = 0;
        padY = 0;

        DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, 0xffffffff);
        font.Draw(x, y, 0xff000000, "%s", ss.str().c_str());

        return;
    }

    // Targets & Agents //
    me = GetOwnCharacter();
    if (me.IsValid()){
        Agent ag = GetOwnAgent();
        self.id = ag.GetAgentId();
        self.pos = ag.GetPos();
        self.speed = ag.GetSpeed();
        self.maxSpeed = ag.GetMaxSpeed();

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

    if (selfFloat && GetOwnAgent().IsValid())
    {
        float rot = GetOwnAgent().GetRot();
        auto Char = GetOwnCharacter();
        Vector3 rotArrow = {
            self.pos.x + cos(rot) * 50.0f,
            self.pos.y + sin(rot) * 50.0f,
            self.pos.z
        };

        DWORD color = COLOR_PLAYER_ALLY;
        float w = Char.GetCurrentHealth() / Char.GetMaxHealth() * 20;
        DrawRectProjected(rotArrow, 20, 5, rot, color);
        DrawRectFilledProjected(rotArrow, w, 5, rot, color);
        DrawCircleProjected(self.pos, 20.0f, color);
        DrawCircleFilledProjected(self.pos, 20.0f, color - floatMask);
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

            Gadget gd = agLocked.GetGadget();
            selected.cHealth = gd.GetCurrentHealth();
            selected.mHealth = gd.GetMaxHealth();
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

            AttackTarget tgt = agLocked.GetAttackTarget();
            selected.cHealth = tgt.GetCurrentHealth();
            selected.mHealth = tgt.GetMaxHealth();
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

    if (floatMouse) {
        Vector3 mcoords = GetMouseInWorld();
        if (!isinf(mcoords.x)) {
            float x, y;
            if (WorldToScreen(mcoords, &x, &y)) {
                DWORD color = 0x44fca5f4;
                DrawCircleProjected(mcoords, 30.0f, color);
                DrawCircleFilledProjected(mcoords, 30.0f, color);
                DrawLineProjected(mcoords, self.pos, color);

                stringstream fs;
                fs << format("%i") % int(Dist(self.pos, mcoords));

                Vector2 fsInfo = font2.TextInfo(fs.str());
                font2.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());
            }
        }
    }


    // compile all agent data
    Floaters floaters;
    Allies allies;
    Agent ag;
    WBosses wbosses;
    while (ag.BeNext())
    {
        if (agentLines) {
            DrawAgentPath(ag);
        } else if (!agPaths.empty()) {
            agPaths.clear();
        }

        // Locked Target (Data)
        if (targetLockID == ag.GetAgentId())
        {
            int agType = ag.GetType();
            if (agType == GW2::AGENT_CATEGORY_CHAR) // char
            {
                locked.valid = true;
                locked.id = ag.GetAgentId();
                locked.type = agType;
                locked.speed = ag.GetSpeed();
                locked.maxSpeed = ag.GetMaxSpeed();
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

                Gadget gd = ag.GetGadget();
                locked.cHealth = gd.GetCurrentHealth();
                locked.mHealth = gd.GetMaxHealth();

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

                AttackTarget tgt = ag.GetAttackTarget();
                locked.cHealth = tgt.GetCurrentHealth();
                locked.mHealth = tgt.GetMaxHealth();

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

            AttackTarget tgt = ag.GetAttackTarget();
            wboss.cHealth = tgt.GetCurrentHealth();
            wboss.mHealth = tgt.GetMaxHealth();

            if (wboss.mHealth > 0)
                wboss.pHealth = 100.f * (wboss.cHealth / wboss.mHealth);
            else
                wboss.pHealth = 0;

            wbosses.list.push_back(wboss);
        }

        // Floaters
        if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege || floatObject)
        {
            int agType = ag.GetType();

            // object floaters
            if (agType == GW2::AGENT_TYPE_GADGET) {
                // gather data
                Gadget gd = ag.GetGadget();
                Vector3 pos = ag.GetPos();
                float rot = ag.GetRot();
                float cHealth = gd.GetCurrentHealth();
                float mHealth = gd.GetMaxHealth();

                // Filter those out of range
                if (Dist(self.pos, pos) <= floatRadius)
                {
                    Object floater;
                    floater.pos = pos;
                    floater.rot = rot;
                    floater.mHealth = mHealth;
                    floater.cHealth = cHealth;

                    floaters.object.push_back(floater);
                }

            } else 
            // NPC & Player
            if (agType == GW2::AGENT_TYPE_CHAR)
            {
                Character ch = ag.GetCharacter();

                // gather data
                Vector3 pos = ag.GetPos();
                float rot = ag.GetRot();
                float cHealth = ch.GetCurrentHealth();
                float mHealth = ch.GetMaxHealth();
                int attitude = ch.GetAttitude();
                int prof = ch.GetProfession();
                string name = ch.GetName();
                GW2::Attitude att = ch.GetAttitude();

                // Filter the dead
                if (cHealth > 0 && mHealth > 1)
                {
                    // Filter those out of range
                    if (Dist(self.pos, pos) <= floatRadius)
                    {
                        Float floater;
                        floater.pos = pos;
                        floater.rot = rot;
                        floater.mHealth = mHealth;
                        floater.cHealth = cHealth;
                        floater.prof = prof;
                        floater.name = name;
                        floater.att = att;

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
            Player player = ag.GetPlayer();

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
                ally.pos = ag.GetPos();

                ally.profession = ch.GetProfession();
                ally.mHealth = round(ch.GetMaxHealth() / (100 + wvwBonus) * 100);
                //ally.cHealth = int(ch.GetCurrentHealth());
                //if (ally.mHealth > 0)
                //ally.pHealth = 100.f * (ally.cHealth / ally.mHealth);
                //else
                //ally.pHealth = 0;
                ally.lvl = ch.GetScaledLevel();
                ally.lvlActual = ch.GetLevel();
                ally.name = player.GetName();

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

    if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege || floatObject)
    {
        stringstream ss;
        Vector2 strInfo;

        if (floatCircles)
        {
            float x, y;
            if (floatAllyNpc && floaters.allyNpc.size() > 0)
            {
                for (auto & floater : floaters.allyNpc) {
                    if (WorldToScreen(floater.pos, &x, &y))
                    {
                        stringstream fs;
                        //fs << floater.name << "\n";
                        if (floatType)
                            fs << format("%i") % int(Dist(self.pos, floater.pos));
                        else
                            fs << format("%i") % floater.mHealth;

                        Vector2 fsInfo = font.TextInfo(fs.str());
                        font.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());

                        Vector3 rotArrow = {
                            floater.pos.x + cos(floater.rot) * 50.0f,
                            floater.pos.y + sin(floater.rot) * 50.0f,
                            floater.pos.z
                        };

                        DWORD color = floater.att == GW2::ATTITUDE_NEUTRAL ? COLOR_NPC_NEUTRAL : COLOR_NPC_ALLY;
                        float w = floater.cHealth / floater.mHealth * 20;
                        DrawCircleProjected(floater.pos, 20.0f, color);
                        DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                        DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                        DrawCircleFilledProjected(floater.pos, 20.0f, color - floatMask);
                    }
                }
            }

            if (floatEnemyNpc && floaters.enemyNpc.size() > 0)
            {
                for (auto & floater : floaters.enemyNpc) {
                    if (WorldToScreen(floater.pos, &x, &y))
                    {
                        stringstream fs;
                        //fs << floater.name << "\n";
                        if (floatType)
                            fs << format("%i") % int(Dist(self.pos, floater.pos));
                        else
                            fs << format("%i") % floater.mHealth;

                        Vector2 fsInfo = font.TextInfo(fs.str());
                        font.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());

                        Vector3 rotArrow = {
                            floater.pos.x + cos(floater.rot) * 50.0f,
                            floater.pos.y + sin(floater.rot) * 50.0f,
                            floater.pos.z
                        };

                        DWORD color = floater.att == GW2::ATTITUDE_INDIFFERENT ? COLOR_NPC_INDIFF : COLOR_NPC_FOE;
                        float w = floater.cHealth / floater.mHealth * 20;
                        DrawCircleProjected(floater.pos, 20.0f, color);
                        DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                        DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                        DrawCircleFilledProjected(floater.pos, 20.0f, color - floatMask);
                    }
                }
            }

            if (floatAllyPlayer && floaters.allyPlayer.size() > 0)
            {
                for (auto & floater : floaters.allyPlayer) {
                    if (WorldToScreen(floater.pos, &x, &y))
                    {
                        if (floatSnap) {
                            float ww = GetWindowWidth() - 25;
                            float wh = GetWindowHeight() - 10;
                            if (x < 50) x = 50;
                            if (x > ww) x = ww;
                            if (y < 20) y = 20;
                            if (y > wh) y = wh;
                        }

                        stringstream fs;
                        //fs << floater.name << "\n";
                        if (floatType)
                            fs << format("%i") % int(Dist(self.pos, floater.pos));
                        else
                            fs << format("%i") % floater.mHealth;

                        Vector2 fsInfo = font.TextInfo(fs.str());
                        font.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());

                        Vector2 fsInfo2 = font2.TextInfo(floater.name);
                        font2.Draw(x - fsInfo2.x / 2, y - 30, fontColor, "%s", floater.name.c_str());

                        Vector3 rotArrow = {
                            floater.pos.x + cos(floater.rot) * 50.0f,
                            floater.pos.y + sin(floater.rot) * 50.0f,
                            floater.pos.z
                        };

                        DWORD color = COLOR_PLAYER_ALLY;
                        float w = floater.cHealth / floater.mHealth * 20;
                        DrawCircleProjected(floater.pos, 20.0f, color);
                        DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                        DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                        DrawCircleFilledProjected(floater.pos, 20.0f, color - floatMask);
                        profIcon[floater.prof].Draw(x - fsInfo.x / 2 - 25, y - lineHeight-1, icon_w, icon_h);
                    }
                }
            }

            if (floatEnemyPlayer && floaters.enemyPlayer.size() > 0)
            {
                for (auto & floater : floaters.enemyPlayer) {
                    if (WorldToScreen(floater.pos, &x, &y))
                    {
                        if (floatSnap) {
                            float ww = GetWindowWidth() - 25;
                            float wh = GetWindowHeight() - 10;
                            if (x < 50) x = 50;
                            if (x > ww) x = ww;
                            if (y < 20) y = 20;
                            if (y > wh) y = wh;
                        }

                        stringstream fs;
                        //fs << floater.name << "\n";
                        if (floatType)
                            fs << format("%i") % int(Dist(self.pos, floater.pos));
                        else
                            fs << format("%i") % floater.mHealth;

                        Vector2 fsInfo = font.TextInfo(fs.str());
                        font.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());

                        Vector3 rotArrow = {
                            floater.pos.x + cos(floater.rot) * 50.0f,
                            floater.pos.y + sin(floater.rot) * 50.0f,
                            floater.pos.z
                        };

                        DWORD color = COLOR_PLAYER_FOE;
                        float w = floater.cHealth / floater.mHealth * 20;
                        DrawCircleProjected(floater.pos, 20.0f, color);
                        DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                        DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                        DrawCircleFilledProjected(floater.pos, 20.0f, color - floatMask);
                        profIcon[floater.prof].Draw(x - fsInfo.x / 2 - 25, y - 17, icon_w, icon_h);
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

            if (floatObject && floaters.object.size() > 0)
            {
                for (auto & floater : floaters.object) {
                    if (WorldToScreen(floater.pos, &x, &y))
                    {

                        stringstream fs;
                        if (floatType)
                            fs << format("%i") % int(Dist(self.pos, floater.pos));
                        else
                            fs << format("%i") % floater.mHealth;

                        Vector2 fsInfo = font.TextInfo(fs.str());
                        font.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());

                        Vector3 rotArrow = {
                            floater.pos.x + cos(floater.rot) * 50.0f,
                            floater.pos.y + sin(floater.rot) * 50.0f,
                            floater.pos.z
                        };

                        DWORD color = COLOR_OBJECT;
                        float w = floater.cHealth / floater.mHealth * 20;
                        DrawCircleProjected(floater.pos, 20.0f, color);
                        DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                        DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                        DrawCircleFilledProjected(floater.pos, 20.0f, color - floatMask);
                    }
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

        if (floatObject)
            ss << format(" | Objects: %i") % floaters.object.size();

        strInfo = font.TextInfo(ss.str());
        float x = round(aTop.x - strInfo.x / 2);
        float y = round(aTop.y);

        DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
        DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
        font.Draw(x, y, fontColor - (floatCircles ? 0x00aa0000 : 0), "%s", ss.str().c_str());

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

            strInfo = font.TextInfo(ss.str());
            float x = round(aTop.x - strInfo.x / 2);
            float y = round(aTop.y);

            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor - (floatCircles ? 0x00aa0000 : 0), "%s", ss.str().c_str());

            aTop.y += strInfo.y + padY * 2;
        }
    }

    if (compDots && compOverlay) {
        compOverlay->RenderOverlay();
    }

    if (showPing) {
        float ww = GetWindowWidth();
        float wh = GetWindowHeight();
        int ping = GetPing();
        int fps = GetFPS();
        int rows = 2;

        stringstream ss;
        Vector2 strInfo;
        ss << format("%i\n%i") % fps % ping;

        strInfo = font.TextInfo(ss.str());
        float x = round(ww - strInfo.x - 2);
        float y = round(wh - lineHeight * rows - 4);
        DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
        DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
        font.Draw(x, y, 0xffffffff, "%s", ss.str().c_str());
    }

    // Bottom Element //
    {
        stringstream ss;
        Vector2 strInfo;

        if (selfHealthPercent && self.alive)
        {
            ss << format("%i") % (int)self.pHealth;

            strInfo = font.TextInfo(ss.str());
            float x = round(aBottom.x - strInfo.x / 2);
            float y = round(aBottom.y - lineHeight);

            //DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            //DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor, "%s", ss.str().c_str());
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
            stringstream sl;
            stringstream sd;

            const char *prof = profFilterName[playerListFilter];

            ss << format("Nearby Players (%s) (WvW HP Bonus: %i%%)") % prof % wvwBonus;
            sp << format("Class");
            sn << format("Name");
            sh << format("Health");
            sv << format("Vitality");
            sl << format("Level");
            sd << format("Distance");

            bool listEmpty = true;
            if (!allies.guard.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_GUARDIAN))
            {
                for (auto & ally : allies.guard) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nGuard:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.war.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_WARRIOR))
            {
                for (auto & ally : allies.war) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nWar:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.engi.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_ENGINEER))
            {
                for (auto & ally : allies.engi) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nEngi:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.ranger.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_RANGER))
            {
                for (auto & ally : allies.ranger) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nRanger:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.thief.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_THIEF))
            {
                for (auto & ally : allies.thief) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nThief:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.ele.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_ELEMENTALIST))
            {
                for (auto & ally : allies.ele) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nEle:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.mes.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_MESMER))
            {
                for (auto & ally : allies.mes) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nMes:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.necro.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_NECROMANCER))
            {
                for (auto & ally : allies.necro) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nNecro:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (!allies.rev.empty() && (!playerListFilter || playerListFilter == GW2::PROFESSION_REVENANT))
            {
                for (auto & ally : allies.rev) {
                    int dist = int(Dist(ally.pos, self.pos));
                    sp << format("\nRev:");
                    sn << format("\n%s") % ally.name;
                    sh << format("\n%i hp") % ally.mHealth;
                    sv << format("\n%+i") % ally.vitality;
                    sl << format("\n%i") % ally.lvlActual;
                    sd << format("\n%i") % dist;
                }
                listEmpty = false;
            }

            if (listEmpty)
            {
                sp << format("\n...");
                sn << format("\n...");
                sh << format("\n...");
                sv << format("\n...");
                sl << format("\n...");
                sd << format("\n...");
            }


            // CharName max width
            stringstream sx;
            sx << "WWWWWWWWWWWWWWWWWWW";
            Vector2 strInfo;
            strInfo = font.TextInfo(sx.str());

            float spOffset = 0;
            float snOffset = spOffset + 65;
            float shOffset = snOffset + strInfo.x;
            float svOffset = shOffset + 85;
            float slOffset = svOffset + 70;
            float sdOffset = slOffset + 60;
            float sxOffset = sdOffset + 70;

            float x = round(aLeft.x);
            float y = round(aLeft.y);

            strInfo = font.TextInfo(sp.str());
            int lineCount = int(strInfo.y)/lineHeight + 2;

            // render the list
            DrawRectFilled(x - padX, y - padY, sxOffset + padX * 2, float(lineCount * lineHeight) + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, sxOffset + padX * 2, float(lineCount * lineHeight) + padY * 2, borderColor);

            int lineShiftY = 2;
            for (int i = 3; i < lineCount; i++) {
                DrawLine(x - padX, y - padY + i * lineHeight + lineShiftY, x + sxOffset + padX, y - padY + i * lineHeight + lineShiftY, borderColor);
            }
            font.Draw(x + spOffset, y, fontColor, "%s", ss.str().c_str()); y += 2 * lineHeight;
            font.Draw(x + spOffset, y, fontColor, "%s", sp.str().c_str());
            font.Draw(x + snOffset, y, fontColor, "%s", sn.str().c_str());
            font.Draw(x + shOffset, y, fontColor, "%s", sh.str().c_str());
            font.Draw(x + svOffset, y, fontColor, "%s", sv.str().c_str());
            font.Draw(x + slOffset, y, fontColor, "%s", sl.str().c_str());
            font.Draw(x + sdOffset, y, fontColor, "%s", sd.str().c_str());
        }
    }

    // TopLeft Element //
    {
        if (targetSelected)
        {
            if (targetLock && selected.valid && locked.valid && selected.id == locked.id)
            {

                stringstream ss;
                Vector2 strInfo;

                ss << format("Selected & Locked: %i / %i [%i%%]") % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth;

                strInfo = font.TextInfo(ss.str());
                float x = round(aTopLeft.x - strInfo.x / 2);
                float y = round(aTopLeft.y);

                DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
                DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
                font.Draw(x, y, fontColor, "%s", ss.str().c_str());

                // Prepare for Next Element
                aTopLeft.y += strInfo.y + padY * 2;
            }
            else
            {
                if (selected.valid)
                {
                    stringstream ss;
                    Vector2 strInfo;

                    ss << format("Selected: %i / %i [%i%%]") % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth;

                    strInfo = font.TextInfo(ss.str());
                    float x = round(aTopLeft.x - strInfo.x / 2);
                    float y = round(aTopLeft.y);

                    DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
                    DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
                    font.Draw(x, y, fontColor, "%s", ss.str().c_str());

                    // Prepare for Next Element
                    aTopLeft.y += strInfo.y + padY * 2;
                }

                if (targetLock && locked.valid)
                {
                    stringstream ss;
                    Vector2 strInfo;

                    ss << format("Locked: %i / %i [%i%%]") % (int)locked.cHealth % (int)locked.mHealth % (int)locked.pHealth;

                    strInfo = font.TextInfo(ss.str());
                    float x = round(aTopLeft.x - strInfo.x / 2);
                    float y = round(aTopLeft.y);

                    DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
                    DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
                    font.Draw(x, y, fontColor, "%s", ss.str().c_str());

                    // Prepare for Next Element
                    aTopLeft.y += strInfo.y + padY * 2;
                }
            }

            if (targetInfo && selected.valid)
            {
                //aTopLeft.y += lineHeight;

                stringstream ss;
                Vector2 strInfo;

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
                    ss << format("Distance: %i") % int(Dist(self.pos, selected.pos));
                    if (agLocked.GetCharacter().GetBreakbarState() != GW2::BREAKBAR_STATE_NONE)
                        ss << format("\nBB: %3.05f%%") % selected.breakbar;
                }

                strInfo = font.TextInfo(ss.str());
                float x = round(aTopLeft.x - strInfo.x / 2);
                float y = round(aTopLeft.y);

                DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
                DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
                font.Draw(x, y, fontColor, "%s", ss.str().c_str());

                // Prepare for Next Element
                ss.str("");
                aTopLeft.y += strInfo.y + padY * 2;
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

            Vector2 strInfo = font.TextInfo(fs.str());
            float lx = 12;
            float ly = 32;

            DrawRectFilled(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(lx, ly, fontColor, "%s", fs.str().c_str());

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

                        Vector2 strInfo = font.TextInfo(fs.str());
                        fx = round(fx - strInfo.x / 2);
                        fy = round(fy - 15);

                        DrawRectFilled(fx - padX, fy - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
                        DrawRect(fx - padX, fy - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
                        font.Draw(fx, fy, fontColor, "%s", fs.str().c_str());
                    }

                    // list
                    {
                        stringstream fs;
                        fs << format("[%i] %i / %i (%i)") % wboss.id % (int)wboss.cHealth % (int)wboss.mHealth % int(Dist(self.pos, wboss.pos));

                        Vector2 strInfo = font.TextInfo(fs.str());
                        ly = round(ly + strInfo.y + padY);

                        DrawRectFilled(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
                        DrawRect(lx - padX, ly - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
                        font.Draw(lx, ly, fontColor, "%s", fs.str().c_str());
                    }
                }
            }
        }

        if (logSpeedometer) {
            stringstream ss;
            Vector2 strInfo;

            /*ss << format("Speed: ");

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
            }*/

            ss << format("Speed: %0.0f in/s - Boost: %0.0f%%") % (speedometer * 32) % (((speedometerMax / 9.1875) - 1) * 100);

            strInfo = font.TextInfo(ss.str());
            float x = round(aTop.x - strInfo.x / 2);
            float y = round(aTop.y);

            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor - (logSpeedometerEnemy ? 0x00aa0000 : 0), "%s", ss.str().c_str());

            aTop.y += strInfo.y + padY * 2;
        }

        if (logDisplacement)
        {
            stringstream ss;
            Vector2 strInfo;

            ss << format("Distance: %i") % bufferDisplacement.dist;

            strInfo = font.TextInfo(ss.str());
            float x = round(aTop.x - strInfo.x / 2);
            float y = round(aTop.y);

            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor - (logDisplacementEnemy ? 0x00aa0000 : 0), "%s", ss.str().c_str());

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
            stringstream ssSelf;
            stringstream ssParty;
            Vector2 strInfoSelf;
            Vector2 strInfoParty;

            float aAdjustX = 120; // adjust anchor -120

            //  4 samples = 1s / 250ms
            // 20 samples = 5s / 250ms

                
            string selfHeader = "Personal";
            string self1s = dpsBufferToString(bufferSelfDps, 4);
            string self5s = dpsBufferToString(bufferSelfDps, 20);

            string partyHeader = "Combined";
            string party1s = dpsBufferToString(bufferDps, 4);
            string party5s = dpsBufferToString(bufferDps, 20);
            
            Vector2 leftHeader = font.TextInfo(selfHeader);
            Vector2 rightHeader = font.TextInfo(partyHeader);

            Vector2 left1s = font.TextInfo(self1s);
            Vector2 right1s = font.TextInfo(party1s);
            
            Vector2 left5s = font.TextInfo(self5s);
            Vector2 right5s = font.TextInfo(party5s);
            
            // calc column widths
            float leftWidth = 50;  // min-width
            float rightWidth = 50; // min-width

            float leftElements[3] = { leftHeader.x, left1s.x, left5s.x };
            float rightElements[3] = { rightHeader.x, right1s.x, right5s.x };

            if (*max_element(leftElements, leftElements + 3) > leftWidth)
                leftWidth = *max_element(leftElements, leftElements + 3);
            if (*max_element(rightElements, rightElements + 3) > rightWidth)
                rightWidth = *max_element(rightElements, rightElements + 3);
            
            // compensate for inner gutter + line
            leftWidth += padX + 1;
            rightWidth += padX + 1;

            // prepare container
            Vector2 boxSize = { leftWidth + rightWidth, 3 * float(lineHeight) };
            Vector2 boxAnchor = {aTopRight.x - leftWidth, aTopRight.y}; // left column's top/right = anchor
            
            DrawRectFilled(boxAnchor.x - padX, boxAnchor.y - padY, boxSize.x + padX * 2, boxSize.y + padY * 2, backColor - bgColorMask);
            DrawRect(boxAnchor.x - padX, boxAnchor.y - padY, boxSize.x + padX * 2, boxSize.y + padY * 2, borderColor);
            DrawLine(boxAnchor.x + leftWidth, boxAnchor.y, boxAnchor.x + leftWidth, boxAnchor.y + lineHeight * 3, borderColor);

            // left self column (right aligned)
            font.Draw(boxAnchor.x + leftWidth - leftHeader.x - padX, boxAnchor.y, fontColor, "%s", selfHeader.c_str());
            font.Draw(boxAnchor.x + leftWidth - left1s.x - padX, boxAnchor.y + lineHeight * 1, fontColor, "%s", self1s.c_str());
            font.Draw(boxAnchor.x + leftWidth - left5s.x - padX, boxAnchor.y + lineHeight * 2, fontColor, "%s", self5s.c_str());
            
            // right party column (left aligned)
            font.Draw(boxAnchor.x + leftWidth + padX + 1, boxAnchor.y, fontColor, "%s", partyHeader.c_str());
            font.Draw(boxAnchor.x + leftWidth + padX + 1, boxAnchor.y + lineHeight * 1, fontColor, "%s", party1s.c_str());
            font.Draw(boxAnchor.x + leftWidth + padX + 1, boxAnchor.y + lineHeight * 2, fontColor, "%s", party5s.c_str());


            /*
            if (logDpsDetails) {
                float sliceParty = 0;
                float sliceSelf = 0;

                ssSelf << format("\n");
                for (size_t i = 0; i < bufferSelfDps.size(); i++) {
                    ssSelf << format("\n%i") % bufferSelfDps[i];
                }

                ssParty << format("\n");
                for (size_t i = 0; i < bufferDps.size(); i++) {
                    ssSelf << format("\n%i") % bufferDps[i];
                }
            }
            */

            // Prepare for Next Element
            //aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;

            // TODO fix dis
            aTopRight.x = boxAnchor.x - padX;
        }

        if (logKillTimer)
        {
            // separate ss vars
            stringstream ss;
            Vector2 strInfo;

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

            strInfo = font.TextInfo(ss.str());
            float x = 0;
            float y = round(aTopRight.y);
            if (logDps)
                x = round(aTopRight.x - strInfo.x - padX); // perma anchor offset with logDps
            else
                x = round(aTopRight.x - strInfo.x / 2); // center otherwise


            // Draw
            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor, "%s", ss.str().c_str());

            // Prepare for Next Element
            //ss.str("");
            aTopRight.y += strInfo.y + padY * 2;
            //aTopRight.x -= 0;
        }

    }

    // Right Elements //
    {
        if (logAttackRate)
        {
            stringstream ss;
            Vector2 strInfo;

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
            ss << format("Threshold: %i hits") % AttackRateChainHits;

            strInfo = font.TextInfo(ss.str());

            float x = round(aRight.x - strInfo.x);
            float y = round(aRight.y);

            // Draw
            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor - (logAttackRateToFile ? 0x00aa0000 : 0), "%s", ss.str().c_str());

            // Prepare for Next Element
            //ss.str("");
            //aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
            aRight.x = x - padX * 2 - 5;
        }

        if (logHits)
        {
            stringstream ss;
            Vector2 strInfo;

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
                ss << format("Average: %0.1f") % average;

                if (logHitsDetails)
                {
                    ss << format("\n\n");
                    ss << format("History");
                    for (size_t i = 0; i < bufferHits.size(); i++)
                        ss << format("\n\u2022 %i") % bufferHits[i];
                }
            }
            else
            {
                ss << format("Counter: ...\n");
                ss << format("Average: ...");

                if (logHitsDetails)
                {
                    ss << format("\n\n");
                    ss << format("History\n");
                    ss << format("\u2022 ...");
                }
            }

            strInfo = font.TextInfo(ss.str());

            float x = round(aRight.x - strInfo.x);
            float y = round(aRight.y);

            // Draw
            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor - (logHitsToFile ? 0x00aa0000 : 0), "%s", ss.str().c_str());

            // Prepare for Next Element
            //ss.str("");
            //aTopRight.y += strInfo.lineCount * lineHeight + padY * 2;
            aRight.x = x - padX * 2 - 5;
        }

        if (logCrits)
        {
            stringstream ss;
            Vector2 strInfo;

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
            ss << format("Sample Hit: %i") % logCritsSample;


            strInfo = font.TextInfo(ss.str());

            float x = round(aRight.x - strInfo.x);
            float y = round(aRight.y);

            // Draw
            DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
            DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
            font.Draw(x, y, fontColor - (logCritsToFile ? 0x00aa0000 : 0), "%s", ss.str().c_str());

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
        ss << format("[%i] Self Health Percent (%s)") % selfHealthPercent % get_key_description("Hotkeys.SELF_HEALTH_PERCENT");
        ss << format("\n");
        ss << format("\n[%i] Selected/Locked Target HP (%s") % targetSelected % get_key_description("Hotkeys.TARGET_SELECTED");
        ss << format("\n[%i] Selected Target Details (%s)") % targetInfo % get_key_description("Hotkeys.TARGET_INFO");
        ss << format("\n[%i] Lock On Target (%s)") % targetLock % get_key_description("Hotkeys.TARGET_LOCK");
        ss << format("\n[%i] Allow Negative DPS (%s)") % dpsAllowNegative % get_key_description("Hotkeys.DPS_ALLOW_NEGATIVE");
        ss << format("\n");
        ss << format("\n[%i] DPS Meter (%s)") % logDps % get_key_description("Hotkeys.LOG_DPS");
        ss << format("\n[%i] DPS Meter History (%s)") % logDpsDetails % get_key_description("Hotkeys.LOG_DPS_DETAILS");
        ss << format("\n");
        ss << format("\n[%i] Kill Timer (%s)") % logKillTimer % get_key_description("Hotkeys.LOG_KILL_TIMER");
        ss << format("\n[%i] Kill Timer Details (%s)") % logKillTimerDetails % get_key_description("Hotkeys.LOG_KILL_TIMER_DETAILS");
        //ss << format("[%i] Kill Timer Writes to a File (Alt Num4)\n") % logKillTimerToFile;
        ss << format("\n");
        ss << format("\n[%i] Monitor Hits (%s)") % logHits % get_key_description("Hotkeys.LOG_HITS");
        ss << format("\n[%i] Show Hits History (%s)") % logHitsDetails % get_key_description("Hotkeys.LOG_HITS_DETAILS");
        ss << format("\n[%i] Record Hits to File (%s)") % logHitsToFile % get_key_description("Hotkeys.LOG_HITS_TO_FILE");
        ss << format("\n");
        ss << format("\n[%i] Monitor Attack Rate (%s)") % logAttackRate % get_key_description("Hotkeys.LOG_ATTACK_RATE");
        ss << format("\n[%i] Show Attack Rate History (%s)") % logAttackRateDetails % get_key_description("Hotkeys.LOG_ATTACK_RATE_DETAILS");
        ss << format("\n[%i] Record Attack Rate to File (%s)") % logAttackRateToFile % get_key_description("Hotkeys.LOG_ATTACK_RATE_TO_FILE");
        ss << format("\n[%i] Adjust Attack Rate Threshold +(%s) and -(%s)") % AttackRateChainHits % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_MORE") % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_LESS");
        ss << format("\n");
        ss << format("\n[%i] Nearby Ally Players List (%s)") % alliesList % get_key_description("Hotkeys.ALLIES_LIST");
        ss << format("\n[%i] Adjust WvW HP Bonus +(%s) and -(%s)") % wvwBonus % get_key_description("Hotkeys.WVW_BONUS_MORE") % get_key_description("Hotkeys.WVW_BONUS_LESS");
        ss << format("\n");
        ss << format("\n[%i] Count Ally NPCs (%s)") % floatAllyNpc % get_key_description("Hotkeys.FLOAT_ALLY_NPC");
        ss << format("\n[%i] Count Enemy NPCs (%s)") % floatEnemyNpc % get_key_description("Hotkeys.FLOAT_ENEMY_NPC");
        ss << format("\n[%i] Count Ally Players (%s)") % floatAllyPlayer % get_key_description("Hotkeys.FLOAT_ALLY_PLAYER");
        ss << format("\n[%i] Count Enemy Players (%s)") % floatEnemyPlayer % get_key_description("Hotkeys.FLOAT_ENEMY_PLAYER");
        //ss << format("[%i] Count Siege (Alt 5)\n") % floatSiege;
        ss << format("\n[%i] Show/Hide Floaters below counted (%s)") % floatCircles % get_key_description("Hotkeys.FLOAT_CIRCLES");
        ss << format("\n[%i] Floaters show Max HP / Distance (%s)") % floatType % get_key_description("Hotkeys.FLOAT_TYPE");
        ss << format("\n");
        ss << format("\n[%i] Speedometer (%s)") % logSpeedometer % get_key_description("Hotkeys.LOG_SPEEDOMETER");
        ss << format("\n[%i] Speedometer for Self/Enemy (%s)") % logSpeedometerEnemy % get_key_description("Hotkeys.LOG_SPEEDOMETER_ENEMY");
        ss << format("\n[%i] Measure Distance (%s)") % logDisplacement % get_key_description("Hotkeys.LOG_DISPLACEMENT");
        ss << format("\n[%i] Distance for Self/Enemy (%s)") % logDisplacementEnemy % get_key_description("Hotkeys.LOG_DISPLACEMENT_ENEMY");

        Vector2 strInfo;
        strInfo = font.TextInfo(ss.str());
        float x = round(aCenter.x - strInfo.x / 2);
        float y = round(aCenter.y - strInfo.y / 2);

        DrawRectFilled(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, backColor - bgColorMask);
        DrawRect(x - padX, y - padY, strInfo.x + padX * 2, strInfo.y + padY * 2, borderColor);
        font.Draw(x, y, fontColor - (!loopLimiter ? 0x00aa0000 : 0), "%s", ss.str().c_str());
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
    floatObject = Str2Bool(read_config_value("Preferences.FLOAT_OBJECT"));
    floatMouse = Str2Bool(read_config_value("Preferences.FLOAT_MOUSE_MEASURE"));
    logSpeedometer = Str2Bool(read_config_value("Preferences.LOG_SPEEDOMETER"));
    logSpeedometerEnemy = Str2Bool(read_config_value("Preferences.LOG_SPEEDOMETER_ENEMY"));
    logDisplacement = Str2Bool(read_config_value("Preferences.LOG_DISPLACEMENT"));
    logDisplacementEnemy = Str2Bool(read_config_value("Preferences.LOG_DISPLACEMENT_ENEMY"));
    floatRadius = Str2Int(read_config_value("Preferences.FLOAT_RADIUS"), floatRadiusMax);
    wvwBonus = Str2Int(read_config_value("Preferences.WVW_BONUS"));
    AttackRateChainHits = Str2Int(read_config_value("Preferences.ATTACKRATE_CHAIN_HITS"), 1);
    logCritsSample = Str2Int(read_config_value("Preferences.LOG_CRITS_SAMPLE"));
    compDotsFade = Str2Bool(read_config_value("Preferences.COMP_OVERLAY_ZFADE"));
    compDots = Str2Bool(read_config_value("Preferences.COMP_OVERLAY"));
    showPing = Str2Bool(read_config_value("Preferences.SHOW_PING"));
    floatSnap = Str2Bool(read_config_value("Preferences.FLOAT_SNAP"));
    playerListFilter = Str2Int(read_config_value("Preferences.PLAYER_LIST_FILTER"));
    agentLines = Str2Bool(read_config_value("Preferences.AGENT_LINES"));
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
    write_config_value("Preferences.FLOAT_OBJECT", Bool2Str(floatObject));
    write_config_value("Preferences.FLOAT_MOUSE_MEASURE", Bool2Str(floatMouse));
    write_config_value("Preferences.LOG_SPEEDOMETER", Bool2Str(logSpeedometer));
    write_config_value("Preferences.LOG_SPEEDOMETER_ENEMY", Bool2Str(logSpeedometerEnemy));
    write_config_value("Preferences.LOG_DISPLACEMENT", Bool2Str(logDisplacement));
    write_config_value("Preferences.LOG_DISPLACEMENT_ENEMY", Bool2Str(logDisplacementEnemy));
    write_config_value("Preferences.FLOAT_RADIUS", Int2Str(floatRadius));
    write_config_value("Preferences.WVW_BONUS", Int2Str(wvwBonus));
    write_config_value("Preferences.ATTACKRATE_CHAIN_HITS", Int2Str(AttackRateChainHits));
    write_config_value("Preferences.LOG_CRITS_SAMPLE", Int2Str(logCritsSample));
    write_config_value("Preferences.COMP_OVERLAY_ZFADE", Bool2Str(compDotsFade));
    write_config_value("Preferences.COMP_OVERLAY", Bool2Str(compDots));
    write_config_value("Preferences.SHOW_PING", Bool2Str(showPing));
    write_config_value("Preferences.FLOAT_SNAP", Bool2Str(floatSnap));
    write_config_value("Preferences.PLAYER_LIST_FILTER", Int2Str(playerListFilter));
    write_config_value("Preferences.AGENT_LINES", Bool2Str(agentLines));
    save_config();
}


void CompassOverlay::RenderOverlay() {
    using namespace GW2LIB;
    GW2LIB::Agent curAg, agSelf;
    GW2LIB::Character curChar;
    GW2LIB::Vector3 camVec, agPos, agSelfPos;
    GW2LIB::Compass comp;

    GW2LIB::GetLockedSelection();

    float curDist, Rotation, CoordX, CoordY;
    float CenterX, CenterY;

    camVec = GetViewVector();
    agSelf = GetOwnAgent();
    agSelfPos = agSelf.GetPos();
    comp = GetCompass();

    float map_width = 0.0f;
    float map_height = 0.0f;
    float map_zoom = 0.0f;
    float wComp = 1.0f;
    float hComp = 1.0f;
    float zoomDiff = 0.0f;
    float zoomComp = 0.0f;
    float map_right = 0.0f;
    float map_bottom = 0.0f;
    float map_top = GetWindowHeight();
    int ifSize = GetUiInterfaceSize();

    switch (ifSize) {
    case GW2::UI_IF_SMALL:
        map_bottom = 33.0f;
        map_right = 0.0f;
        wComp = 0.898f;
        hComp = 0.898f;
        zoomComp = 26.7f;
        zoomDiff = 19.9f;
        break;
    case GW2::UI_IF_NORMAL:
        map_bottom = 37.0f;
        map_right = 0.0f;
        wComp = 1.0f;
        hComp = 1.0f;
        zoomComp = 23.9f;
        zoomDiff = 18.0f;
        break;
    case GW2::UI_IF_LARGE:
        map_bottom = 40.0f;
        map_right = -2.0f;
        wComp = 1.117647058823529f;
        hComp = 1.117647058823529f;
        zoomComp = 21.5f;
        zoomDiff = 16.2f;
        break;
    case GW2::UI_IF_LARGER:
        map_bottom = 44.0f;
        map_right = -2.0f;
        wComp = 1.229411764705882f;
        hComp = 1.229411764705882f;
        zoomComp = 19.5f;
        zoomDiff = 14.7f;
        break;
    }

    map_width = (comp.GetWidth() / 2) * wComp;
    map_height = (comp.GetHeight() / 2) * hComp;
    map_zoom = (comp.GetZoom() * zoomDiff) + zoomComp;
    if (comp.GetPosition()) map_top = (map_height * 2) + 35.5f;

    CenterX = GetWindowWidth() - (map_width + map_right);
    CenterY = GetWindowHeight() - (map_height + map_bottom);
    DrawCircleFilled(CenterX, CenterY, 2, SelfDotColor);

    while (curAg.BeNext()){
        GW2::AgentType type = curAg.GetType();
        if (type != GW2::AGENT_TYPE_CHAR && type != GW2::AGENT_TYPE_GADGET) continue;
        if (curAg.GetAgentId() == GetOwnAgent().GetAgentId()) continue;
        if (!curAg.IsValid()) continue;

        curChar = curAg.GetCharacter();
        if (type == GW2::AGENT_TYPE_CHAR && !curChar.IsAlive() && !curChar.IsDowned()) continue;
        if (type == GW2::AGENT_TYPE_GADGET && !floatObject) continue;
        if (type == GW2::AGENT_TYPE_CHAR && FilterDot(curAg)) continue;
        agPos = curAg.GetPos();
        curDist = dist2D(agSelfPos, agPos);
        if (curDist > (float)floatRadius) continue;

        Rotation = !comp.GetRotation() ?
            ((atan2(agPos.y - agSelfPos.y, agPos.x - agSelfPos.x)) + 2 * PI) :
            (atan2(agPos.y - agSelfPos.y, agPos.x - agSelfPos.x) - atan2(camVec.y, camVec.x)) + (PI / 2);

        CoordX = CenterX + (curDist * cos(Rotation)) / map_zoom;
        CoordY = CenterY - (curDist * sin(Rotation)) / map_zoom;

        if (CoordX > CenterX + map_width) CoordX = CenterX + map_width;
        if (CoordX < CenterX - map_width) CoordX = CenterX - map_width;
        if (CoordX > CenterX + map_width) CoordX = CenterX + map_width;
        if (CoordY > CenterY + map_height) CoordY = CenterY + map_height;
        if (CoordY < CenterY - map_height) CoordY = CenterY - map_height;

        DWORD alpha = CalcFade(agSelfPos, agPos);
        DWORD color = type == GW2::AGENT_TYPE_GADGET ? (alpha | Color_OBJECT) : GetColor(curChar, alpha);

        DrawCircleFilled(CoordX, CoordY, 4.0f, alpha);
        DrawCircleFilled(CoordX, CoordY, 3.0f, color);
    }

}

bool CompassOverlay::FilterDot(GW2LIB::Agent ag) {
    GW2LIB::Character ch = ag.GetCharacter();
    GW2::Attitude att = ch.GetAttitude();
    bool isPlayer = ch.IsPlayer();
    if (isPlayer && (!floatAllyPlayer && att == GW2::ATTITUDE_FRIENDLY)) return true;
    if (isPlayer && (!floatEnemyPlayer && att == GW2::ATTITUDE_HOSTILE)) return true;
    if (!isPlayer && (!floatAllyNpc && att == GW2::ATTITUDE_NEUTRAL)) return true;
    if (!isPlayer && (!floatAllyNpc && att == GW2::ATTITUDE_FRIENDLY)) return true;
    if (!isPlayer && (!floatEnemyNpc && att == GW2::ATTITUDE_HOSTILE)) return true;
    if (!isPlayer && (!floatEnemyNpc && att == GW2::ATTITUDE_INDIFFERENT)) return true;
    return false;
}



int CompassOverlay::CalcFade(GW2LIB::Vector3 self, GW2LIB::Vector3 ag) {
    int alpha = alphaMask;
    if (compDotsFade) {
        float fr = fadeMark;
        float tmp = abs(self.z - ag.z);
        if (tmp > fr) tmp = fr;
        tmp /= fr;
        alpha = int(roundf(255.0f - tmp * 255.0f)) << 24;
    }
    return alpha;
}

int CompassOverlay::GetColor(GW2LIB::Character curChar, int initialColor = 0xFF000000) {
    int color = initialColor;
    switch (curChar.GetAttitude()){
    case GW2::ATTITUDE_FRIENDLY:
        color |= curChar.IsPlayer() ? Color_PLAYER : Color_FRIENDLY;
        break;
    case GW2::ATTITUDE_HOSTILE:
        color |= curChar.IsPlayer() ? Color_HOSTILE : Color_NPC_FOE;
        break;
    case GW2::ATTITUDE_INDIFFERENT:
        color |= Color_INDIFFERENT;
        break;
    case GW2::ATTITUDE_NEUTRAL:
        color |= Color_NEUTRAL;
        break;
    }

    return color;
}


void chat_log(wchar_t *wtxt) {
    size_t len = wcslen(wtxt) + 1;
    char *txt = new char[len];
    memset(txt, 0, len);
    wcstombs(txt, wtxt, len);
    chat = txt;
    delete txt;
}


bool mouse_move(int x, int y, int modkeys) {
    mouse_x = x;
    mouse_y = y;
    mouse_keys = modkeys;
    return true;
}

bool mouse_click(bool down, int button, int x, int y, int modkeys) {
    mouse_down = down;
    mouse_btn = button;
    mouse_x = x;
    mouse_y = y;
    mouse_keys = modkeys;
    return true;
}

bool mouse_wheel(int delta, int modkeys) {
    mouse_delta = delta;
    mouse_keys = modkeys;
    return true;
}

void dmg_log(Agent src, Agent tgt, int hit) {
    //HL_LOG_DBG("hit: %i\n", hit);
}

void combat_log(CombatLogType type, int hit, Agent tgt) {
    //HL_LOG_DBG("agent: %i - type: %i - hit: %i\n", tgt.GetAgentId(), type, hit);
    switch (type) {
    case CL_CONDI_DMG_OUT:
    case CL_CRIT_DMG_OUT:
    case CL_GLANCE_DMG_OUT:
    case CL_PHYS_DMG_OUT:
        if (locked.valid && locked.id != tgt.GetAgentId()) {
            totaldmg = 0;
            selfDmg.total = 0;
            selfDmg.snapshot = 0; // also set in threadDps, probably not safe...
        }

        if (locked.valid && locked.id == tgt.GetAgentId()) {
            selfDmg.total += float(hit);
            totaldmg += hit;
        }
        break;
    }
}

void GW2LIB::gw2lib_main()
{
    locale::global(locale("en-US"));

    init_keymap();
    init_config();
    load_preferences();

    compOverlay = new CompassOverlay();

    EnableEsp(ESP);
    SetGameHook(HOOK_CHAT, chat_log);
    SetGameHook(HOOK_MOUSE_MOVE, mouse_move);
    SetGameHook(HOOK_MOUSE_BUTTON, mouse_click);
    SetGameHook(HOOK_MOUSE_WHEEL, mouse_wheel);
    SetGameHook(HOOK_DAMAGE_LOG, dmg_log);
    SetGameHook(HOOK_COMBAT_LOG, combat_log);

    thread t1(&threadHotKeys);
    thread t2(&threadDps);
    thread t3(&threadKillTimer);
    thread t4(&threadHits);
    thread t5(&threadAttackRate);
    thread t6(&threadCrits);
    thread t7(&threadSpeedometer);

    if (!font.Init(lineHeight, fontFamily) || !font2.Init(lineHeight, fontFamily, false))
    {
        //DbgOut("could not create font");n
        return;
    }

    HMODULE dll = hl::GetCurrentModule();

    for (int i = 1; i < GW2::PROFESSION_END; i++) {
        stringstream res_id;
        res_id << "IDB_PNG" << i;
        HRSRC ires = FindResourceA(dll, res_id.str().c_str(), "PNG");
        if (ires && !profIcon[i].Init(LockResource(LoadResource(dll, ires)), SizeofResource(dll, ires))) {
            HL_LOG_ERR("Unable to load profession icon: %i\n", res_id);
        }
    }

    // wait for exit hotkey
    while (GetAsyncKeyState(VK_F12) >= 0)
        Sleep(25);

    close_config();

    // make threads clean-up first before interrupting
    PostThreadMessage(thread_id_hotkey, WM_USER, NULL, NULL);

    delete compOverlay;
    compOverlay = nullptr;

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
