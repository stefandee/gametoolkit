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
#include <stdio.h>
#include "string.h"
#pragma hdrstop

#include "SPSndLoad.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

SPSndLoad::SPSndLoad()
{
  memset(&fileHeader, 0, sizeof(TPSndFileHeader));
}
//---------------------------------------------------------------------------

bool SPSndLoad::Load(unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2)
{
  // seek la inceputul datelor
  if (!seek(sizeof(TPSndFileHeader)))
  {
   return false;
  }

  // citirea efectiva a datelor
  // prima bucata
  if (dataSize1 > 0)
  {
    if (!data1)
    {
      return false;
    }

    // sau cu try..catch ?
    if (!read(data1, dataSize1))
    {
      return false;
    }
  }

  // a doua bucata, daca e necesar
  if (dataSize2 > 0)
  {
    if (!data2)
    {
      return false;
    }

    // sau cu try..catch ?
    if (read(data2, dataSize2))
    {
      return false;
    }
  }

  return true;
}
//---------------------------------------------------------------------------

/*
bool SPSndLoad::Save(char* fileName, unsigned char* data, int dataSize, TSndInfo info, char* description)
{
  TPSndFileHeader hdr;
  int             descLength;

  // handle'ul e folosit ?
  if (fileHandle != -1)
  {
    return false;
  }

  // datele sint valide ?
  if (!data)
  {
   return false;
  }

  // deschid / creez fisierul
  if ((fileHandle = open(fileName, O_BINARY | O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE)) <= 0)
  {
   return false;
  }

  // completez header'ul
  strncpy((char*)hdr.id, PSND_ID, PSND_ID_SIZE);
  descLength = strlen(description);
  if (descLength > PSND_DESC_SIZE) descLength = PSND_DESC_SIZE;
  strncpy((char*)hdr.description, description, descLength);

  hdr.channels      = info.channels;
  hdr.sampleRate    = info.sampleRate;
  hdr.bitsPerSample = info.bitsPerSample;
  hdr.compression   = PSND_COMPRESSION_NONE;

  hdr.version       = ((unsigned __int32)PSND_MAJOR_VERSION << 16) | PSND_MINOR_VERSION;

  // scriu headerul
	 if ((write(fileHandle, &hdr.id,            PSND_ID_SIZE)    == -1) ||
	     (write(fileHandle, &hdr.description,   PSND_DESC_SIZE)  == -1) ||
	     (write(fileHandle, &hdr.version,       sizeof(__int32)) == -1) ||
	     (write(fileHandle, &hdr.compression,   sizeof(__int32)) == -1) ||
	     (write(fileHandle, &hdr.channels,      sizeof(__int32)) == -1) ||
	     (write(fileHandle, &hdr.sampleRate,    sizeof(__int32)) == -1) ||
	     (write(fileHandle, &hdr.bitsPerSample, sizeof(__int32)) == -1))
  {
	   return false;
  }

  // scriu datele
  if (write(fileHandle, data, dataSize) == -1)
  {
    return false;
  }

  if (!Close())
  {
    return false;
  }

  return false;
}
//---------------------------------------------------------------------------
*/

bool SPSndLoad::ReadHeader()
{
  if (!rewind())
  {
    return false;
  }
	
  // citirea cimpurilor headerului; din cauza ca structurile pot fi aliniate dqord/qword, trebuie facuta citirea
  // fiecarui cimp individual
  if (!read(&fileHeader.id,            PSND_ID_SIZE * sizeof(__int8)))
  {
    return false;
  }

  if (!read(&fileHeader.description,   PSND_DESC_SIZE * sizeof(__int8)))
  {
    return false;
  }

  if (!read(&fileHeader.version,       sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.compression,   sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.channels,      sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.sampleRate,    sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.bitsPerSample, sizeof(__int32)))
  {
    return false;
  }

  // verificari - fisierul e de tip .pSnd
  if (strncmp((char*)fileHeader.id, PSND_ID, PSND_ID_SIZE) != 0)
  {
    return false;
  }

  // verificari de versiune
  if (((fileHeader.version >> 16)        != PSND_MAJOR_VERSION) ||
      ((fileHeader.version & 0xFFFF0000) != PSND_MINOR_VERSION))
  {
    return false;
  }

  // completez o structura TSndInfo
  info.channels      = fileHeader.channels;
  info.sampleRate    = fileHeader.sampleRate;
  if (info.sampleRate == 0)
  {
    return false;
  }

  info.bitsPerSample = fileHeader.bitsPerSample;
  info.blockAlign    = info.channels * (info.bitsPerSample / 8);
  info.length        = length() - sizeof(TPSndFileHeader);

  if (info.channels > 0)
  {
    info.time = (float)(((float)info.length / (info.channels * (info.bitsPerSample / 8))) * (1.0 / (float)info.sampleRate)); // complicat, nu ? :)
  }
  else
  {
    info.time = 0.0;
  }  
  
  return true;
}
//---------------------------------------------------------------------------

unsigned char* SPSndLoad::GetDescription()
{
  return reinterpret_cast<unsigned char*>(fileHeader.description);
}
//---------------------------------------------------------------------------


