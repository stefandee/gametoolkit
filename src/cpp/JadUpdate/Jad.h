//---------------------------------------------------------------------------

#ifndef JadH
#define JadH
//---------------------------------------------------------------------------

#include <vector>
#include <System.hpp>

class Property
{
  public:
    AnsiString mName, mValue;
};

class Jad
{
  public:
    Jad();

  public:
    bool Open(AnsiString jadFile);
    bool Update(AnsiString property, AnsiString value);
    void Flush();  

  private:
    bool Parse();
    void AddProperty(AnsiString line);

    AnsiString mJadFile;

    std::vector<Property> mProperties;
};

#endif
