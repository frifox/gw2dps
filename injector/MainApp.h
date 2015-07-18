#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <wx/wxprec.h>

class MainApp : public wxApp
{
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(MainApp)
#endif