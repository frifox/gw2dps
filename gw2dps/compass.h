#ifndef COMPASS_H
#define COMPASS_H

class CompassOverlay {
    const float PI = 3.141592653f;

    const DWORD BackgroundColor = 0xBB62B3E2;
    const DWORD BorderColor = 0xFFFFFFFF;
    const DWORD SelfDotColor = 0xff78eaff;

    const DWORD Color_FRIENDLY = 0x00FF30;
    const DWORD Color_HOSTILE = 0xFF0000;
    const DWORD Color_INDIFFERENT = 0xFFDE00;
    const DWORD Color_NEUTRAL = 0x665E5E;
    const DWORD Color_PLAYER = 0x0000FF; //0x60D7FE;
    const DWORD Color_NPC_FOE = 0xe76d00;
    const DWORD Color_OBJECT = 0xffffff;
    const float fadeMark = 1800.0f;
    const int alphaMask = 0xFF000000;

    float dist2D(GW2LIB::Vector3 p1, GW2LIB::Vector3 p2) {
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }

public:
    void RenderOverlay();
    bool FilterDot(GW2LIB::Agent &ag);
    int CalcFade(GW2LIB::Vector3 self, GW2LIB::Vector3 ag);
    int GetColor(GW2LIB::Character curChar, int initialColor);
};

#endif;