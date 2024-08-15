#ifndef RandomH
#define RandomH

#include <algorithm>
#include <sstream>
#include "PP_String.h"

extern int gfRandom(int _v);
extern float gfRandom48();
extern std::wstring StringToWString(const std::string& str);
extern std::string WStringToString(const std::wstring& wstr);
extern std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace);

template <typename T>T clamp(T in, T low, T high)
{
    return std::min(std::max(in, low), high);
}

extern CPString padZero(CPString _string, int size);

template <typename T> std::string toString(T v)
{
    std::ostringstream s;
    s << v;
    return s.str();
}

#endif
