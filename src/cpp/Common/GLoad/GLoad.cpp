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
// History:
//	  29-Jan-2000 [Kafka]
//      (commented unreferenced parameters names (eliminate warnings))
//---------------------------------------------------------------------------

//#include <io.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <string.h>
#include <stdexcept>
#pragma hdrstop

#include "GLoad.h"
#include "Stream_ANSI_C.h"
//#include "Stream_zzip.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

GLoad::GLoad()
:	mStream(0)
{
	info.width  = 0;
	info.height = 0;
	info.bpp    = RGB_32;
}
//---------------------------------------------------------------------------

// Descriere   : incarca headerul si verifica daca este un fisier valid
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : va fi suprascrisa de fiecare loader de format
bool GLoad::ReadHeader()
{
	return false;
}
//---------------------------------------------------------------------------

size_t GLoad::read(void* pMem, size_t nBytes)
{
	if (!mStream)
	{
		throw std::runtime_error("Can not read. File not open");
	}
	return mStream->read(pMem, nBytes);
}
//---------------------------------------------------------------------------

size_t GLoad::readInt(int* v)
{
	if (!mStream)
	{
		throw std::runtime_error("Can not read. File not open");
	}
	return mStream->readInt(v);
}
//---------------------------------------------------------------------------

size_t GLoad::readShort(short* v)
{
	if (!mStream)
	{
		throw std::runtime_error("Can not read. File not open");
	}
	return mStream->readShort(v);
}
//---------------------------------------------------------------------------

bool GLoad::seek(size_t offset)
{
	if (!mStream)
	{
		throw std::runtime_error("Can not rewind. File not open");
	}
	return mStream->seek(offset);
}
//---------------------------------------------------------------------------

bool GLoad::skip(size_t size)
{
	if (!mStream)
	{
		throw std::runtime_error("Can not rewind. File not open");
	}
	return mStream->skip(size);
}
//---------------------------------------------------------------------------

bool GLoad::rewind()
{
	if (!mStream)
	{
		throw std::runtime_error("Can not rewind. File not open");
	}
	return mStream->rewind();
}
//---------------------------------------------------------------------------

size_t GLoad::length()
{
  if (!mStream)
  {
    throw std::runtime_error("Cannot get length. File not open");
  }

  return mStream->length();
}
//---------------------------------------------------------------------------

int GLoad::tell()
{
  if (!mStream)
  {
    throw std::runtime_error("Cannot get current pos. File not open");
  }

  return mStream->tell();
}
//---------------------------------------------------------------------------

// Descriere   : deschide un fisier grafic si citeste informatiile (headerele interne)
// Param       : fileName - fisierul
// Rezultat    : true/false - operatie reusita/nu
// Comentarii  : va fi suprascrisa de fiecare loader de format
//               ([Kafka] of course if open style is different)
// History     :
//         28-Jan-2000 [Kafka] moved code from derived class in base class
//                           because they all done the same thing
bool GLoad::Open(const char* fileName)
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

void GLoad::GetTInfo(TGInfo* fileInfo)
{
	*fileInfo = info;
}
//---------------------------------------------------------------------------

void GLoad::GetInfo(int* width, int* height, TColorFormat* fileBpp)
{
	*width   = info.width;
	*height  = info.height;
	*fileBpp = info.bpp;
}
//---------------------------------------------------------------------------

bool GLoad::Load(unsigned char* /*data*/, int /*width*/, int /*height*/, TColorFormat /*dataBpp*/)
{
	return false;
}
//---------------------------------------------------------------------------

bool GLoad::LoadAligned (unsigned char* , int , int , int , TColorFormat )
{
  return false;
}

// Descriere   : inchide fisierul
// Param       : n/a
// Rezultat    : true/false
// Comentarii  : n/a
bool GLoad::Close()
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

GLoad::~GLoad()
{
  delete mStream;
  mStream = 0;
}
//---------------------------------------------------------------------------

GSave::GSave(CPString _fileName)
{
  try
  {
    mStream = new PP::Stream_ANSI_C(_fileName, true);
  }
  catch (...)
  {
    mStream = 0;
    throw std::runtime_error("GSave - Can not open for save.");
  }
}
//---------------------------------------------------------------------------

size_t GSave::write(void* pMem, size_t nBytes)
{
  if (!mStream)
  {
    throw std::runtime_error("Can not write. File not open");
  }

  return mStream->write(pMem, nBytes);
}
//---------------------------------------------------------------------------

