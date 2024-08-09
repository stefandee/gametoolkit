//---------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#pragma hdrstop

#include "Jad.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Jad::Jad()
{
}
//---------------------------------------------------------------------------

bool Jad::Open(AnsiString jadFile)
{
  mJadFile = jadFile;

  return Parse();
}
//---------------------------------------------------------------------------

bool Jad::Update(AnsiString property, AnsiString value)
{
  for(unsigned int i = 0; i < mProperties.size(); i++)
  {
    if (mProperties[i].mName == property)
    {
      mProperties[i].mValue = value;

      return true;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

void Jad::Flush()
{
  FILE* file;

  file = fopen(mJadFile.c_str(), "w+t");

  for(unsigned int i = 0; i < mProperties.size(); i++)
  {
    fprintf(file, "%s: %s\n", mProperties[i].mName.c_str(), mProperties[i].mValue.c_str());
  }

  fclose(file);
}
//---------------------------------------------------------------------------

bool Jad::Parse()
{
  FILE* file;

  file = fopen(mJadFile.c_str(), "rt");

  if (!file)
  {
    return false;
  }

  // read until end of file
  bool eof = false;

  while(!eof)
  {
    AnsiString line = "";

    while(!feof(file))
    {
      int c = fgetc(file);

      if (c == '\n')
      {
        break;
      }

      if (c != -1)
      {
        line += (char)c;
      }
    }

    if (line != "")
    {
      AddProperty(line);
    }
    else
    {
      break;
    }
  }

  fclose(file);

  return true;
}
//---------------------------------------------------------------------------

void Jad::AddProperty(AnsiString line)
{
  if (line.Trim() == "")
  {
    //cout << "null line" << endl;
    return;
  }

  //cout << "|" << line << "|" << endl;

  // find the first occurence of ":"
  int delimiterIndex = line.Pos(":");

  Property property;

  property.mName  = line.SubString(0, delimiterIndex - 1).Trim();
  property.mValue = line.SubString(delimiterIndex + 1, line.Length() - delimiterIndex).Trim();

  mProperties.push_back(property);
}
//---------------------------------------------------------------------------
