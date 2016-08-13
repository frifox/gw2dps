#ifndef GRAPHER_H
#define GRAPHER_H

class Grapher {
public:
    Grapher() {}
    Grapher(int size, string n, int pad = 0);
    Grapher(int size, int pad = 0);

    bool Init();
    void Draw(float x, float y, float h);
    void Push(float);
    void Clear();

protected:
    boost::circular_buffer<float> buff;
    GW2LIB::Font fName;
    std::string name;
    int pad;
};


#endif
