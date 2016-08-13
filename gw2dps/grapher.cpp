
#include "globals.h"

// graph width is determined by buffer capacity
void Grapher::Draw(float x, float y, float h) {
    size_t size = buff.size();
    float w = (float)buff.capacity();
    float b = y + h;
    float r = w + pad * 2 + 1;
    float max = 0;

    // get max val first
    for (float val : buff) {
        if (val > max) max = val;
    }

    // draw boxes
    DrawRectFilled(x, y, r, h, backColor - bgColorMask);
    DrawRect(x, y, r, h, 0xff0060ff);

    if (!size || !max) return;

    // draw graph lines
    for (size_t i = 0; i < size; i++) {
        float val = buff[i];
        if (!val) continue;
        float pct = val / max;
        float px = i + pad + 1 + x;
        float py = b - h * pct;

        float x1 = px;
        float y1 = b - pad - 1;
        float x2 = px;
        float y2 = py + pad;

        if (y2 > y1) y2 = y1;

        DrawLine(x1, y1, x2, y2, 0xffffffff);
    }

}


void Grapher::Push(float val) {
    buff.push_front(val);
}

void Grapher::Clear() {
    buff.clear();
}

