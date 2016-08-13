#ifndef GRAPHER_H
#define GRAPHER_H

class Grapher {
public:
    Grapher(int size, int pad = 0) : buff(size), pad(pad) {}
    void Draw(float x, float y, float h);
    void Push(float);
    void Clear();

    int pad = 0;

protected:
    boost::circular_buffer<float> buff;
};


#endif
