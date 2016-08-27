#include "preferences.h"

string Bool2Str(bool b) {
    return b ? "true" : "false";
}

bool Str2Bool(string str, bool def) {
    if (str == "") return def;
    return strToLower(str) == "true" || strToLower(str) == "on" || str == "1" ? true : false;
}

string Int2Str(int i) {
    char tmp[20];
    sprintf_s(tmp, sizeof(tmp) - 1, "%i", i);
    return string(tmp);
}

int Str2Int(string str, int def) {
    if (str == "") return def;
    return atoi(str.c_str());
}

string strToLower(string str) {
    string tmp = str;
    transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    return tmp;
}
