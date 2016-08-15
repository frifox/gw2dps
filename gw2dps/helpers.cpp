
#include "globals.h"

float Dist(Vector3 p1, Vector3 p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

string SecondsToString(double input) {
    int hours = int(input) / 60 / 60;
    int minutes = (int(input) - hours * 60 * 60) / 60;
    double seconds = (input - hours * 60 * 60 - minutes * 60);

    stringstream ss;
    if (hours > 0) ss << format("%ihr ") % hours;
    if (minutes > 0) ss << format("%im ") % minutes;
    if (seconds > 0) ss << format("%0.2fs") % seconds;
    if (input == 0) ss << "0.00s";

    return ss.str();
}

baseHpReturn baseHp(int lvl, int profession) {
    // base stats
    float hp = 0;
    float vit = 0;

    // calc base vit for the lvl
    int cake = 0;
    while (cake <= lvl) {

        if (cake == 2)
            vit += 44;

        if (cake > 2 && cake < 11) {
            vit += 7;
        } else {
            if (cake % 2 == 0) {
                if (11 < cake && cake < 21)
                    vit += 10;
                if (21 < cake && cake < 25)
                    vit += 14;
                if (25 < cake && cake < 27)
                    vit += 15;
                if (27 < cake && cake < 31)
                    vit += 16;
                if (31 < cake && cake < 41)
                    vit += 20;
                if (41 < cake && cake < 45)
                    vit += 24;
                if (45 < cake && cake < 47)
                    vit += 25;
                if (47 < cake && cake < 51)
                    vit += 26;
                if (51 < cake && cake < 61)
                    vit += 30;
                if (61 < cake && cake < 65)
                    vit += 34;
                if (65 < cake && cake < 67)
                    vit += 35;
                if (67 < cake && cake < 71)
                    vit += 36;
                if (71 < cake && cake < 75)
                    vit += 44;
                if (75 < cake && cake < 77)
                    vit += 45;
                if (77 < cake && cake <= 80)
                    vit += 46;
            }
        }

        cake++;
    }

    // calc base hp
    switch (profession) {
    case GW2::PROFESSION_WARRIOR:
    case GW2::PROFESSION_NECROMANCER:
        hp = lvl * 28.f;
        if (lvl > 19) hp += (lvl - 19) * float(42);
        if (lvl > 39) hp += (lvl - 39) * float(70);
        if (lvl > 59) hp += (lvl - 59) * float(70);
        if (lvl > 79) hp += (lvl - 79) * float(70);
        hp += vit * 10;
        break;
    case GW2::PROFESSION_ENGINEER:
    case GW2::PROFESSION_RANGER:
    case GW2::PROFESSION_MESMER:
    case GW2::PROFESSION_REVENANT:
        hp = lvl * 18.f;
        if (lvl > 19) hp += (lvl - 19) * float(27);
        if (lvl > 39) hp += (lvl - 39) * float(45);
        if (lvl > 59) hp += (lvl - 59) * float(45);
        if (lvl > 79) hp += (lvl - 79) * float(45);
        hp += vit * 10;
        break;
    case GW2::PROFESSION_GUARDIAN:
    case GW2::PROFESSION_ELEMENTALIST:
    case GW2::PROFESSION_THIEF:
        hp = lvl * 5.f;
        if (lvl > 19) hp += (lvl - 19) * float(7.5);
        if (lvl > 39) hp += (lvl - 39) * float(12.5);
        if (lvl > 59) hp += (lvl - 59) * float(12.5);
        if (lvl > 79) hp += (lvl - 79) * float(12.5);
        hp += vit * 10;
        break;
    }

    baseHpReturn out;
    out.health = hp;
    out.vitality = vit;
    return out;
}

string dpsBufferToString(boost::circular_buffer<float> &buffer, size_t samples) {
    if (samples > buffer.size())
        samples = buffer.size();

    double avg = 0;
    for (size_t i = 0; i < samples; i++)
        avg += buffer[i];
    if (samples > 0)
        avg = avg / samples * (1000 / 250);

    stringstream dps;
    if (samples > 0)
        dps << format("%0.0f") % avg;
    else
        dps << (string) "...";

    return dps.str();
}


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
    if (!floatCircles && ag.GetAgentId() != me.GetAgent().GetAgentId()) return;

    GW2::AgentCategory agcat = ag.GetCategory();
    if (agcat != GW2::AGENT_CATEGORY_CHAR) return;

    Character ch = ag.GetCharacter();
    if (!ch.IsValid() || !ch.IsAlive()) return;

    GW2::Attitude att = ch.GetAttitude();

    switch (att) {
    case GW2::ATTITUDE_FRIENDLY:
        if (ch.IsControlled()) {
            if (!selfFloat) return;
        } else {
            if (ch.IsPlayer() && !floatAllyPlayer) return;
            if (!ch.IsPlayer() && !floatAllyNpc) return;
        }
        break;
    case GW2::ATTITUDE_HOSTILE:
        if (ch.IsPlayer() && !floatEnemyPlayer) return;
        if (!ch.IsPlayer() && !floatEnemyNpc) return;
        break;
    case GW2::ATTITUDE_NEUTRAL:
        if (!ch.IsPlayer() && !floatAllyNpc) return;
        break;
    case GW2::ATTITUDE_INDIFFERENT:
        if (!ch.IsPlayer() && !floatNeutEnemyNpc) return;
        break;
    default: return;
    }

    int agid = ag.GetAgentId();
    if (agPaths.find(agid) == agPaths.end()) {
        agPaths[agid] = circular_buffer<Vector3>(200);
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


void DrawFloater(const Float &floater, DWORD color, bool drawArrow, bool drawText, bool drawName, bool drawProfIcon) {
    float x, y;
    if (WorldToScreen(floater.pos, &x, &y)) {
        if (floater.isPlayer && floatSnap) {
            float ww = GetWindowWidth() - 25;
            float wh = GetWindowHeight() - 10;
            if (x < 50) x = 50;
            if (x > ww) x = ww;
            if (y < 33) y = 33;
            if (y > wh) y = wh;
        }

        if (drawArrow) {
            Vector3 rotArrow = {
                floater.pos.x + cos(floater.rot) * 50.0f,
                floater.pos.y + sin(floater.rot) * 50.0f,
                floater.pos.z
            };

            float w = floater.cHealth / floater.mHealth * 20;
            DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
            DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
        }

        DrawCircleProjected(floater.pos, 20.0f, color);
        DrawCircleFilledProjected(floater.pos, 20.0f, color - floatMask);

        if (drawText) {
            stringstream fs;
            if (floatType)
                fs << format("%i") % int(Dist(self.pos, floater.pos));
            else
                fs << format("%i") % floater.mHealth;

            Vector2 fsInfo = font.TextInfo(fs.str());
            if (floater.isPlayer && drawProfIcon)
                (floater.eliteSpec ? eliteIcon[floater.prof] : profIcon[floater.prof]).Draw(x - fsInfo.x / 2 - 25, y - lineHeight - 1, icon_w, icon_h);
            font.Draw(x - fsInfo.x / 2, y - 15, fontColor, "%s", fs.str().c_str());

            if (drawName) {
                Vector2 fsInfo2 = font2.TextInfo(floater.name);
                if (floater.name.size()) font2.Draw(x - fsInfo2.x / 2, y - 30, fontColor, "%s", floater.name.c_str());
            }
        }
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
    floatNeutEnemyNpc = Str2Bool(read_config_value("Preferences.FLOAT_NEUT_ENEMY_NPC"));
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
    write_config_value("Preferences.FLOAT_NEUT_ENEMY_NPC", Bool2Str(floatNeutEnemyNpc));
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




