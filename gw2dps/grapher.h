#ifndef GRAPHER_H
#define GRAPHER_H

class Grapher {
public:
    Grapher() {}
    Grapher::Grapher(int size);

    bool Grapher::Init(int font_size = 16, string font_family = "Verdana", bool bold = true);
    void Draw(float x, float y, float h);
    void Push(float);
    void Clear();

    std::string name;
    int pad;
    bool ltr;
    bool show_avg;

protected:
    boost::circular_buffer<float> buff;
    GW2LIB::Font fName;
};


#endif
