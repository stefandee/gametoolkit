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

//#if defined(__SUNPRO_CC) || defined(__GNUG__)
#if defined(PPT_PLATFORM_LINUX) || defined(PPT_PLATFORM_SOLARIS)
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "config.h"
#include "Cfg.h"
#include "easylogging++.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CCfg::CCfg()
{
  fileHandle = -1;
}
//---------------------------------------------------------------------------

// Descriere   : deschide un fisier de configurare; pina la distrugerea clasei, fisierul va ramine deschis
// Param       : fileName  - numele fisierului
// Rezultat    : true/false - operatie reusita/nu
// Comentarii  : n/a
bool CCfg::Open(const char* fileName)
{
	fileHandle = open(fileName, O_BINARY | O_RDWR);

	if (fileHandle > 0)
	{
		// citesc headerele sa vad daca este un fisier valid, si sa obtin informatiile generale
		if (ReadHeader())
		{
			return true;
		}
		else
		{
         LOG(ERROR) << "CCfg::Open - cannot read header";
			if (Close())
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------

// Descriere   : creaza un fisier de configurare; fisierul e gol (nu are header)
// Param       : fileName - numele fisierului
// Rezultat    : true/false
// Comentarii  : n/a
bool CCfg::Create(const char* fileName)
{
  // deschid/creez fisierul
  fileHandle = open(fileName, O_CREAT | O_BINARY | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);

  if (fileHandle > 0)
  {
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool CCfg::ReadHeader()
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfg::GetResource(const char* resName, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfg::SetResource(const char* resName, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfg::AddResource(const char* resName, TResourceType resType, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfg::DeleteResource(const char* resName)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfg::Empty()
{
  if (fileHandle == -1)
  {
    return false;
  }

  if (chsize(fileHandle, 0) == -1)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : inchide fisierul de configurare
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : n/a
bool CCfg::Close()
{
	if (fileHandle == -1)
	{
		return false;
	}
	if (close(fileHandle) == -1)
	{
		return false;
	}

	fileHandle = -1;
	return true;
}
//---------------------------------------------------------------------------

CCfg::~CCfg()
{
  Close(); // desi nu e chiar corect, dupa Kafka
}
//---------------------------------------------------------------------------


