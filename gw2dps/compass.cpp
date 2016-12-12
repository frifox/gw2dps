
#include "globals.h"


void CompassOverlay::RenderOverlay() {
    using namespace GW2LIB;
    GW2LIB::Agent curAg, agSelf;
    GW2LIB::Character curChar;
    GW2LIB::Vector3 camVec, agPos, agSelfPos;
    GW2LIB::Compass comp;

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

    while (curAg.BeNext()) {
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

bool CompassOverlay::FilterDot(GW2LIB::Agent &ag) {
    GW2LIB::Character ch = ag.GetCharacter();
    GW2::Attitude att = ch.GetAttitude();
    bool isPlayer = ch.IsPlayer();
    if (isPlayer && !(!playerListFilter || playerListFilter == ch.GetProfession())) return true;
    if (isPlayer && (!floatAllyPlayer && att == GW2::ATTITUDE_FRIENDLY)) return true;
    if (isPlayer && (!floatEnemyPlayer && att == GW2::ATTITUDE_HOSTILE)) return true;
    if (!isPlayer && (!floatAllyNpc && att == GW2::ATTITUDE_NEUTRAL)) return true;
    if (!isPlayer && (!floatAllyNpc && att == GW2::ATTITUDE_FRIENDLY)) return true;
    if (!isPlayer && (!floatEnemyNpc && att == GW2::ATTITUDE_HOSTILE)) return true;
    if (!isPlayer && (!floatNeutEnemyNpc && att == GW2::ATTITUDE_INDIFFERENT)) return true;
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
    switch (curChar.GetAttitude()) {
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

