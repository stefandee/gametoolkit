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
//---------------------------------------------------------------------------
#pragma hdrstop

#include <algorithm>
#include "BitOp.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// Descriere   : numara bitii cu valoare de 1 dintr'un dword
// Param       : what - numarul
// Rezultat    : numarul de biti cu valoare 1
// Comentarii  : se foloseste pentru masti (mask)
__int8 CountBits32(unsigned __int32 what)
{
  __int8 count = 0;

  for(int i = 0; i < 32; i++)
  {
    if (what & 1) count++;
    what = what >> 1;
  }

  return count;
}
//----------------------------------------------------------------------------

// Descriere   : offset'ul mastii in dword (cite rotiri spre dreapta trebuie facute)
// Param       : what - numarul
// Rezultat    : offset'ul
// Comentarii  : se foloseste pentru masti (mask)
__int8 MaskOffset32(unsigned __int32 what)
{
  __int8 offset = 0;

  for(int i = 0; i < 32; i++)
  {
    if (what & 1) break;
    what = what >> 1;
    offset++;
  }

  return offset;
}
//----------------------------------------------------------------------------

// Descriere   : transforma long'uri din big endian in little endian si invers
// Param       : what - numar
// Rezultat    : numarul in formatul complementar
// Comentarii  : Motorola e naspa ;)
unsigned __int32 Swap32(unsigned __int32 what)
{
  int a, b, c, d;
  a = (what & 0x000000FF);
  b = (what & 0x0000FF00) >> 8;
  c = (what & 0x00FF0000) >> 16;
  d = (what & 0xFF000000) >> 24;
  return (a << 24) | (b << 16) | (c << 8) | d;
}
//----------------------------------------------------------------------------

// Descriere   : transforma short'uri din big endian in little endian si invers
// Param       : what - numar
// Rezultat    : numarul in formatul complementar
// Comentarii  : Motorola e naspa ;)
unsigned __int16 Swap16(unsigned __int16 what)
{
  int a, b;
  a = (what & 0x00FF);
  b = (what & 0xFF00) >> 8;
  return (a << 8) | b;
}
//----------------------------------------------------------------------------

void SwapBytes(unsigned char * b, int _size)
{
   int i = 0;
   int j = _size-1;

   while (i<j)
   {
      std::swap(b[i], b[j]);
      i++, j--;
   }
}
//----------------------------------------------------------------------------

