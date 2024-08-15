#include <iostream>
#include <string>
#pragma hdrstop

using namespace std;
#include "PP_Stdlib.h"
#include <codecvt>

int gfRandom(int _v)
{
  if (_v <= 0)
  {
    return 0;
  }

  return rand()%_v;
}
//---------------------------------------------------------------------------

float gfRandom48()
{
  return (float)rand() / RAND_MAX;
}
//---------------------------------------------------------------------------

CPString padZero(CPString _string, int size)
{
  int strLen = _string.Length();

  for(int i = 0; i < size - strLen; i++)
  {
    _string = CPString("0") + _string;
  }

  return _string;
}
//---------------------------------------------------------------------------

wstring StringToWString(const string& str)
{
	/*wstring wstr;
	size_t size;
	wstr.resize(str.length());
	mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
	return wstr;*/
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}
//---------------------------------------------------------------------------

string WStringToString(const wstring& wstr)
{
/*
	string str;
	size_t size;
	str.resize(wstr.length());
	wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
	return str;
	*/
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}
//---------------------------------------------------------------------------

std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		 subject.replace(pos, search.length(), replace);
		 pos += replace.length();
	}
	return subject;
}
//---------------------------------------------------------------------------

