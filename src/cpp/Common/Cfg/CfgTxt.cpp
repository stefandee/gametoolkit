//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
#include "Platform.h"

#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

#if defined(PPT_PLATFORM_LINUX) || defined(PPT_PLATFORM_SOLARIS) || defined (PPT_PLATFORM_MACOSX)
#include <unistd.h>
#endif

#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "CfgTxt.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// Descriere   : constructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : face initializarea tabelei de simboluri
CCfgTxt::CCfgTxt()
{
  av.value   = (char*)new char[CFG_RESNAME_SIZE + 1];
  av.lexCode = CFGA_NOTHING;

  // initializarea tabelei de symboluri
  symbols = 0;
  for(int i = 0; i < CFG_SYMBOLS; i++)
  {
    symbolTable[i].resType = RES_NOTHING;
    symbolTable[i].value   = NULL;
    memset(symbolTable[i].resName, '\0', CFG_RESNAME_SIZE + 1);
  }

  // construiesc keyword list
  keywordsCount = 2;
  strcpy(cfgKeywords[0], "POINT");
  strcpy(cfgKeywords[1], "RECT");
}
//---------------------------------------------------------------------------

// Descriere   : returneaza urmatorul character din fisier
// Param       : n/a
// Rezultat    : caracterul citit
// Comentarii  : nu e error proof (nu se poate afla cind anume e eroare de citire din fisier), dar deocamdata e ok
//               nu e bine sa fie apelata cu random
int CCfgTxt::NextChar()
{
  char ch;

  int result = read(fileHandle, &ch, sizeof(char));

  if (result == 0)
  {
    return 256;
  }

  if (result == -1)
  {
    return -1;
  }

  return ch;
}
//---------------------------------------------------------------------------

int CCfgTxt::BackChar()
{
  lseek(fileHandle, -1, SEEK_CUR);
  return 1;
}
//---------------------------------------------------------------------------

bool CCfgTxt::IsKeyWord(char* name)
{
  for(int i = 0; i < keywordsCount; i++)
  {
    if (strcmp(cfgKeywords[i], name) == 0)
    {
      return true;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

bool CCfgTxt::CfgALex()
{
  int           ch, fractionarPart;
  int           resNameIndex = 0;

  // prelucrare initiala
  av.lexCode = CFGA_NOTHING;
  memset((char*)av.value, '\0', CFG_RESNAME_SIZE);

  do
  {
    ch = NextChar();

    // CARACTER ALfANUMERIC
    if (isalpha(ch))
    {
      // prelucrez identificator (eventual keyword)
      while(isalpha(ch) || isdigit(ch))
      {
        ((char*)av.value)[resNameIndex] = (char)ch;
        resNameIndex++;
        if (resNameIndex > CFG_RESNAME_SIZE)
        {
          return false;
        }
        ch = NextChar();
      }

      if (ch != 256)
      {
        BackChar();
      }

      // identificarea keywordului
      if (IsKeyWord((char*)av.value))
      {
        av.lexCode = CFGA_KEYWORD;
      }
      else
      {
        av.lexCode = CFGA_IDENT;
      }

      return true;
    }

    // DIGIT ?
    if (isdigit(ch))
    {
      // prelucrez cifra
      //((char*)av.value)[resNameIndex] = (char)ch;

      while(isdigit(ch))
      {
        ((char*)av.value)[resNameIndex] = (char)ch;
        resNameIndex++;
        if (resNameIndex > CFG_RESNAME_SIZE)
        {
          return false;
        }
        ch = NextChar();
      }

      // a fost punct ?
      if (ch == '.')
      {
        // avem numar real, adaug punctul
        ((char*)av.value)[resNameIndex++] = (char)ch;

        // citesc in continuare
        fractionarPart = 0;
        ch = NextChar();
        while(isdigit(ch))
        {
          ((char*)av.value)[resNameIndex] = (char)ch;
          resNameIndex++;
          if (resNameIndex > CFG_RESNAME_SIZE)
          {
            return false;
          }
          fractionarPart++;
          ch = NextChar();
        }


        if (fractionarPart == 0)
        {
          return false;
        }

        if (ch != 256)
        {
          BackChar();
        }

        av.lexCode = CFGA_REAL;
        return true;
      }

      if (ch != 256)
      {
        BackChar();
      }

      av.lexCode = CFGA_INT;
      return true;
    }

    // SEMNE SPECIALE
    switch(ch)
    {

      case ';' :
        // comentariu, deci skipez pina la sfirsit de linie
        do
        {
          ch = NextChar();
        }
        while ((ch != '\n') && (ch != 256));

        if (ch == 256)
        {
          av.lexCode = CFGA_EOF;
          return true;
        }

        BackChar();
        break;

      case '[' :
        av.lexCode = CFGA_BRACKET_LEFT;
        return true;

      case ']' :
        av.lexCode = CFGA_BRACKET_RIGHT;
        return true;

      case '=' :
        av.lexCode = CFGA_EQUAL;
        return true;

      case '"' :
        // sir de caractere, citesc pina la urmatorul "
        av.lexCode = CFGA_STRING;

        ch = NextChar();
        while(ch != '"')
        {
          ((char*)av.value)[resNameIndex] = (char)ch;
          resNameIndex++;
          if (resNameIndex > CFG_RESNAME_SIZE)
          {
            return false;
          }
          ch = NextChar();
        }

        // BackChar();

        // elimin "
        //resNameIndex--;
        //((char*)av.value)[resNameIndex] = '\0';

        return true;
      case 256 :
        av.lexCode = CFGA_EOF;
        return true;
    }
  }
  while(ch == 13 || ch == 10 || ch == 32);

  return false;
}
//---------------------------------------------------------------------------

bool CCfgTxt::Groups()
{
  if (!CfgALex())
  {
    return false;
  }

  while(av.lexCode != CFGA_EOF)
  {
    if (!Group())
    {
      return false;
    }
  }

  return true;
}

bool CCfgTxt::Group()
{
  // test the group header
  if (av.lexCode != CFGA_BRACKET_LEFT)
  {
    return false;
  }

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_IDENT)
  {
     return false;
  }

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_BRACKET_RIGHT)
  {
    return false;
  }

  if (!CfgALex())
  {
    return false;
  }

  return Resources();
}
//---------------------------------------------------------------------------

bool CCfgTxt::Resources()
{
  char resName[CFG_RESNAME_SIZE + 1];

  while(av.lexCode == CFGA_IDENT)
  {
    // iau numele resursei
    memset(resName, '\0', CFG_RESNAME_SIZE + 1);
    strncpy(resName, (char*)av.value, CFG_RESNAME_SIZE + 1);

    /*
    if (av.lexCode != CFGA_IDENT)
    {
      return false;
    }
    */

    // pun identificatorul in symbol table

    if (!CfgALex())
    {
      return false;
    }

    if (av.lexCode != CFGA_EQUAL)
    {
      return false;
    }

    if (!CfgALex())
    {
      return false;
    }

    switch(av.lexCode)
    {
      case CFGA_INT    :
        if (!AddInt(resName, (char*)av.value))
        {
          return false;
        }
        break;

      case CFGA_REAL   :
        if (!AddDouble(resName, (char*)av.value))
        {
          return false;
        }
        break;

      case CFGA_STRING :
        if (!AddString(resName, (char*)av.value))
        {
          return false;
        }
        break;

      case CFGA_KEYWORD :
        // e POINT ?
        if (strcmp("POINT", (char*)av.value) == 0)
        {
          if (!PointDecl(resName))
          {
            return false;
          }
          break;
        }

        // e RECT ?
        if (strcmp("RECT", (char*)av.value) == 0)
        {
          if (!RectDecl(resName))
          {
            return false;
          }
          break;
        }

        return false;

      default:
        return false;
    }

    if (!CfgALex())
    {
      return false;
    }
  }

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::PointDecl(const char* resName)
{
  int  x, y;
  //char resName[CFG_RESNAME_SIZE + 1];

  // iau numele resursei
  //memset(resName, '\0', CFG_RESNAME_SIZE + 1);
  //strncpy(resName, (char*)av.value, CFG_RESNAME_SIZE + 1);

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_INT)
  {
    return false;
  }

  x = atoi((char*)(av.value));

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_INT)
  {
    return false;
  }

  y = atoi((char*)(av.value));

  AddPoint(const_cast<char*>(resName), x, y);

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::RectDecl(const char* resName)
{
  int  left, right, top, bottom;
  //char resName[CFG_RESNAME_SIZE + 1];

  // iau numele resursei
  //memset(resName, '\0', CFG_RESNAME_SIZE + 1);
  //strncpy(resName, (char*)av.value, CFG_RESNAME_SIZE + 1);

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_INT)
  {
    return false;
  }

  left = atoi((char*)(av.value));

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_INT)
  {
    return false;
  }

  top = atoi((char*)(av.value));

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_INT)
  {
    return false;
  }

  right = atoi((char*)(av.value));

  if (!CfgALex())
  {
    return false;
  }

  if (av.lexCode != CFGA_INT)
  {
    return false;
  }

  bottom = atoi((char*)(av.value));

  AddRect(const_cast<char*>(resName), left, top, right, bottom);

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::CfgASin()
{
  // verificari
  if (fileHandle == -1)
  {
    return false;
  }

  if (lseek(fileHandle, 0, SEEK_SET) == -1)
  {
    return false;
  }

  return Groups();
}
//---------------------------------------------------------------------------

bool CCfgTxt::AddString(char* resName, char *value)
{
  if ((symbols >= CFG_SYMBOLS) || (symbols < 0))
  {
    return false;
  }

  // in functie de lungimea sirului, aleg tipul resursei
  int strLength = strlen(value);

  // e un caracter ?
  if (strLength == 1)
  {
    // adaug un caracter
    symbolTable[symbols].resType = RES_CHAR;

    strncpy((char*)(symbolTable[symbols].resName), resName, strlen(resName));

    // try...catch ?
    symbolTable[symbols].value = new char[1];
    *((char*)symbolTable[symbols].value)  =  value[0];

    symbols++;
    return true;
  }

  if (strLength > 256)
  {
    return false;
  }

  // defensiva pe toate fronturile :)
  int resNameLength = strlen(resName);
  if (resNameLength > CFG_RESNAME_SIZE + 1) resNameLength = CFG_RESNAME_SIZE + 1;
  strncpy((char*)(symbolTable[symbols].resName), resName, resNameLength);

  // try...catch ?
  symbolTable[symbols].value = new char[strLength + 1];
  strncpy((char*)(symbolTable[symbols].value), value, strLength + 1);

  // e string de lungime pina in 32 ?
  if (strLength <= 32)
  {
    symbolTable[symbols].resType = RES_STRING32;
  }

  // e string de lungime pina in 64 ?
  if (strLength <= 64)
  {
    symbolTable[symbols].resType = RES_STRING64;
  }

  // e string de lungime pina in 256 ?
  if (strLength <= 256)
  {
    symbolTable[symbols].resType = RES_STRING256;
  }

  symbols++;

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::AddRect(char* resName, __int32 left, __int32 top, __int32 right, __int32 bottom)
{
  if ((symbols >= CFG_SYMBOLS) || (symbols < 0))
  {
    return false;
  }

  // defensiva pe toate fronturile :)
  int resNameLength = strlen(resName);
  if (resNameLength > CFG_RESNAME_SIZE + 1) resNameLength = CFG_RESNAME_SIZE + 1;
  strncpy((char*)(symbolTable[symbols].resName), resName, resNameLength);
  symbolTable[symbols].resType = RES_TRECT;

  // aloc memorie
  symbolTable[symbols].value = new __int32[4];

  // pun valorile
  ((__int32*)symbolTable[symbols].value)[0] = left;
  ((__int32*)symbolTable[symbols].value)[1] = top;
  ((__int32*)symbolTable[symbols].value)[2] = right;
  ((__int32*)symbolTable[symbols].value)[3] = bottom;

  symbols++;

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::AddPoint(char* resName, __int32 x, __int32 y)
{
  if ((symbols >= CFG_SYMBOLS) || (symbols < 0))
  {
    return false;
  }

  // defensiva pe toate fronturile :)
  int resNameLength = strlen(resName);
  if (resNameLength > CFG_RESNAME_SIZE + 1) resNameLength = CFG_RESNAME_SIZE + 1;
  strncpy((char*)(symbolTable[symbols].resName), resName, resNameLength);
  symbolTable[symbols].resType = RES_TPOINT;

  // aloc memorie
  symbolTable[symbols].value = new __int32[2];

  // pun valorile
  ((__int32*)symbolTable[symbols].value)[0] = x;
  ((__int32*)symbolTable[symbols].value)[1] = y;

  symbols++;

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::AddInt(char* resName, char* value)
{
  if ((symbols >= CFG_SYMBOLS) || (symbols < 0))
  {
    return false;
  }

  // defensiva pe toate fronturile :)
  int resNameLength = strlen(resName);
  if (resNameLength > CFG_RESNAME_SIZE + 1) resNameLength = CFG_RESNAME_SIZE + 1;
  strncpy((char*)(symbolTable[symbols].resName), resName, resNameLength);

  symbolTable[symbols].resType = RES_INT;
  // aloc memorie
  symbolTable[symbols].value = new __int32[1];

  // pun valorile
  *((__int32*)(symbolTable[symbols].value)) = atoi(value);

  symbols++;

  return true;
}
//---------------------------------------------------------------------------

bool CCfgTxt::AddDouble(char* resName, char* value)
{
  if ((symbols >= CFG_SYMBOLS) || (symbols < 0))
  {
    return false;
  }

  // defensiva pe toate fronturile :)
  int resNameLength = strlen(resName);
  if (resNameLength > CFG_RESNAME_SIZE + 1) resNameLength = CFG_RESNAME_SIZE + 1;
  strncpy((char*)(symbolTable[symbols].resName), resName, resNameLength);

  symbolTable[symbols].resType = RES_DOUBLE;
  // aloc memorie
  symbolTable[symbols].value = new double[1];

  // pun valorile
  *((double*)(symbolTable[symbols].value)) = atof(value);

  symbols++;

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : clasica functie de citire de headere am inlocuit'o cu verificarea sintactica
// Param       : n/a
// Rezultat    : true/false - fisierul e scris sintactic valid
// Comentarii  : n/a
bool CCfgTxt::ReadHeader()
{
  return CfgASin();
}
//---------------------------------------------------------------------------

bool CCfgTxt::GetResource(const char* resName, void* value)
{
  if (symbols <= 0)
  {
    return false;
  }

  for(int i = 0; i < symbols; i++)
  {
    if (strcmp(resName, symbolTable[i].resName) == 0)
    {
      switch(symbolTable[i].resType)
      {
        case RES_CHAR :
          ((char*)value)[0] =     ((char*)symbolTable[i].value)[0];
          return true;

        case RES_STRING32 :
          strncpy((char*)value,   (char*)symbolTable[i].value, 32);
          return true;

        case RES_STRING64 :
          strncpy((char*)value,   (char*)symbolTable[i].value, 64);
          return true;

        case RES_STRING256 :
          strncpy((char*)value,   (char*)symbolTable[i].value, 256);
          return true;

        case RES_TPOINT :
          memcpy((__int32*)value, (__int32*)symbolTable[i].value, 2 * sizeof(__int32));
          return true;

        case RES_TRECT :
          memcpy((__int32*)value, (__int32*)symbolTable[i].value, 4 * sizeof(__int32));
          return true;

        case RES_INT :
          memcpy((__int32*)value, (__int32*)symbolTable[i].value, 1 * sizeof(__int32));
          return true;

        case RES_DOUBLE :
          memcpy((double*)value,  (double*)symbolTable[i].value, 1 * sizeof(double));
          return true;

        default :
          return false;
      }
    }
  }

  return false;
}
//---------------------------------------------------------------------------

TResourceType CCfgTxt::GetResourceType(const char* resName)
{
  if (symbols <= 0)
  {
    return RES_NOTHING;
  }

  for(int i = 0; i < symbols; i++)
  {
    if (strcmp(resName, symbolTable[i].resName) == 0)
    {
      return symbolTable[i].resType;
    }
  }

  return RES_NOTHING;
}
//---------------------------------------------------------------------------

bool CCfgTxt::SetResource(const char* resName, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfgTxt::AddResource(const char* resName, TResourceType resType, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfgTxt::DeleteResource(const char* resName)
{
  return false;
}
//---------------------------------------------------------------------------

CCfgTxt::~CCfgTxt()
{
  for(int i = 0; i < symbols; i++)
  {
    delete [] symbolTable[i].value;
  }
}
//---------------------------------------------------------------------------

