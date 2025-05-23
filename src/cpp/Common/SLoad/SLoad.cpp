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
//#include <string.h>
#include <stdexcept>
#pragma hdrstop

#include "Stream_ANSI_C.h"
#include "SLoad.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

SLoad::SLoad() : mStream(0)
{
  info.channels      = 0;
  info.sampleRate    = 0;
  info.bitsPerSample = 0;
  info.blockAlign    = 0;
  info.length        = 0;
  info.time          = 0;
}
//---------------------------------------------------------------------------

// Descriere   : deschide un fisier grafic si citeste informatiile (headerele interne)
// Param       : fileName - fisierul
// Rezultat    : true/false - operatie reusita/nu
// Comentarii  : n/a
bool SLoad::Open(const char* fileName/*, int sampleRate, int bitsPerSample, int channels*/)
{
  try
  {
    mStream = new PP::Stream_ANSI_C(fileName);
          
    if (!ReadHeader())
    {
      delete mStream;
      mStream = 0;
      return false;
    }
  }
  catch (...)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca headerul si verifica daca este un fisier valid
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : va fi suprascrisa de fiecare loader de format
/*
bool SLoad::ReadHeader()
{
  return false;
}
//---------------------------------------------------------------------------
*/

void SLoad::GetSndInfo(TSndInfo* fileInfo)
{
  (*fileInfo) = info;
}
//---------------------------------------------------------------------------

bool SLoad::Load(unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2)
{
  return false;
}
//---------------------------------------------------------------------------

// Descriere   : inchide fisierul
// Param       : n/a
// Rezultat    : true/false
// Comentarii  : n/a
bool SLoad::Close()
{
  if (!mStream)
  {
    return false;
  }

  delete mStream;
  mStream = 0;

  return true;
}
//---------------------------------------------------------------------------

size_t SLoad::read(void* pMem, size_t nBytes)
{
  if (!mStream)
  {
    throw std::runtime_error("Can not read. File not open");
  }

  return mStream->read(pMem, nBytes);
}
//---------------------------------------------------------------------------

bool SLoad::seek(size_t offset)
{
  if (!mStream)
  {
    throw std::runtime_error("Can not rewind. File not open");
  }

  return mStream->seek(offset);
}
//---------------------------------------------------------------------------

bool SLoad::skip(size_t size)
{
  if (!mStream)
  {
    throw std::runtime_error("Can not rewind. File not open");
  }

  return mStream->skip(size);
}
//---------------------------------------------------------------------------

bool SLoad::rewind()
{
  if (!mStream)
  {
    throw std::runtime_error("Can not rewind. File not open");
  }

  return mStream->rewind();
}
//---------------------------------------------------------------------------

size_t SLoad::length()
{
  if (!mStream)
  {
    throw std::runtime_error("Cannot get length. File not open");
  }

  return mStream->length();
}
//---------------------------------------------------------------------------

int SLoad::tell()
{
  if (!mStream)
  {
    throw std::runtime_error("Cannot get current pos. File not open");
  }

  return mStream->tell();
}
//---------------------------------------------------------------------------

SLoad::~SLoad()
{
  delete mStream;
  mStream = 0;
}
//---------------------------------------------------------------------------


