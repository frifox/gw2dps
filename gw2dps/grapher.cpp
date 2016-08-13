
#include "globals.h"

Grapher::Grapher(int size, string n, int pad, bool ltr) :
    buff(size), name(n), pad(pad), ltr(ltr) {
}

Grapher::Grapher(int size, int pad, bool ltr) :
    buff(size), name(""), pad(pad), ltr(ltr) {
}

bool Grapher::Init() {
    return fName.Init(16, "Verdana");
}

// graph width is determined by buffer capacity
void Grapher::Draw(float x, float y, float h) {
    size_t size = buff.size();
    float w = (float)buff.capacity();
    float border = 1;
    float max = 0;

    float pw = w + pad * 2;
    float ph = h + pad * 2;
    float bw = pw + border * 2;
    float bh = ph + border * 2;
    float cx = bw / 2 + x;
    float cy = bh / 2 + y;

    float l = x + border + pad;
    float b = y + border + pad + h - 1;

    // get max val first
    for (float val : buff) {
        if (val > max) max = val;
    }

    Vector2 nInfo = fName.TextInfo(name);

    // draw boxes and name
    DrawRectFilled(x, y, bw, bh, backColor - bgColorMask);
    DrawRect(x, y, bw-1, bh-1, 0xff0060ff);
    if(name.size()) fName.Draw(cx-nInfo.x/2, cy-nInfo.y/2, 0xcc999999, "%s", name.c_str());

    if (!size || !max) return;

    // draw graph lines
    for (size_t i = 0; i < size; i++) {
        float val = buff[i];
        if (!val) continue;
        float pct = val / max;
        float px = ltr ? l + w - i - 1 : i + l;
        float py = b - h * pct;

        DrawLine(px, b, px, py, 0xffffffff);
    }

}

void Grapher::Push(float val) {
    buff.push_front(val);
}

void Grapher::Clear() {
    buff.clear();
}

