#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

void load_preferences();
void save_preferences();

string Bool2Str(bool b);
bool Str2Bool(string str, bool def=false);

string Int2Str(int i);
int Str2Int(string str, int def=0);

string strToLower(string str);


#endif