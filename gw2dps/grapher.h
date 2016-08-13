#ifndef GRAPHER_H
#define GRAPHER_H

class Grapher {
public:
    Grapher() {}
    Grapher(int size, string n, int pad = 0, bool ltr = false, bool show_avg = false);
    Grapher(int size, int pad = 0, bool ltr = false, bool show_avg = false);

    bool Init();
    void Draw(float x, float y, float h);
    void Push(float);
    void Clear();

protected:
    boost::circular_buffer<float> buff;
    GW2LIB::Font fName;
    std::string name;
    int pad;
    bool ltr;
    bool show_avg;
};


#endif
