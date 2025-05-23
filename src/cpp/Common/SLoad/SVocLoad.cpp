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
#include "string.h"
#include <stdio.h>
#pragma hdrstop

#include "SVocLoad.h"

//---------------------------------------------------------------------------
SVocLoad::SVocLoad()
{
  memset(&fileHeader, 0, sizeof(TVocFileHeader)); // de la Kafka citire
  dataPosition = -1;
}
//---------------------------------------------------------------------------

bool SVocLoad::Load(unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2)
{
  if (dataPosition == -1)
  {
    return false;
  }

  // seek la inceputul datelor
  if (!seek(dataPosition))
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
    if (!read(data2, dataSize2))
    {
      return false;
    }
  }

  return true;
}
//---------------------------------------------------------------------------

bool SVocLoad::ReadHeader()
{
  __int8   blockType;
  T9Header block9;
  unsigned __int8 d1, d2, d3;
  __int32  dataSize;

  if (!rewind())
  {
    return false;
  }

  // citirea cimpurilor headerului; din cauza ca structurile pot fi aliniate dqord/qword, trebuie facuta citirea
  // fiecarui cimp individual
  if (read(&fileHeader.id, VOC_ID_LENGTH * sizeof(__int8)))
  {
    return false;
  }

  if (read(&fileHeader.skipPrint,          sizeof(__int8)))
  {
    return false;
  }

  if (read(&fileHeader.firstBlockOffset,   sizeof(__int16)))
  {
    return false;
  }

  if (read(&fileHeader.version,            sizeof(__int8)))
  {
    return false;
  }

  if (read(&fileHeader.crap,               sizeof(__int8)))
  {
    return false;
  }

  // e un fisier valid ?
  if (strncmp((char*)fileHeader.id, VOC_ID, VOC_ID_LENGTH) != 0)
  {
    return false;
  }

  // caut blocul de date
  if (!seek(fileHeader.firstBlockOffset))
  {
    return false;
  }

  do
  {
    // citesc un octet - tipul blocului
    if (!read(&blockType, sizeof(__int8)))
    {
      return false;
    }

    switch(blockType)
    {
      case VOC_DBLOCK_SOUNDDATA      :
      case VOC_DBLOCK_SOUNDCONT      :
      case VOC_DBLOCK_SILENCE        :
      case VOC_DBLOCK_MARKER         :
      case VOC_DBLOCK_REPEAT         :
      case VOC_DBLOCK_ENDREPEAT      :
      case VOC_DBLOCK_EXTENDED       :
        return false;

      case VOC_DBLOCK_ASCII          :
        // citesc si dimensiunea; 3 octeti
        if (!read(&d1, sizeof(__int8)))
        {
          return false;
        }

        if (!read(&d2, sizeof(__int8)))
        {
          return false;
        }

        if (!read(&d3, sizeof(__int8)))
        {
          return false;
        }

        // calculez dimensiunea datelor
        dataSize = 65536 * d3 + 256 * d2 + d1;

        // sar peste dataSize octeti
        if (!skip(dataSize))
        {
          return false;
        }

        break;

      case VOC_DBLOCK_TYPE9          :
        // citesc si dimensiunea; 3 octeti
        if (!read(&d1, sizeof(__int8)))
        {
          return false;
        }

        if (!read(&d2, sizeof(__int8)))
        {
          return false;
        }

        if (!read(&d3, sizeof(__int8)))
        {
          return false;
        }

        // calculez dimensiunea datelor
        dataSize = 65536 * d3 + 256 * d2 + d1 - sizeof(T9Header) - 1;

        // asta e ceea ce caut
        if (!read(&block9, sizeof(T9Header)))
        {
          return false;
        }

        // completez structura TSndInfo
        info.channels      = block9.channels;
        info.sampleRate    = block9.sampleRate;
        if (block9.sampleRate == 0)
        {
          return false;
        }
        info.bitsPerSample = block9.bitsPerSample;
        info.blockAlign    = info.channels * (info.bitsPerSample / 8);
        info.length        = dataSize;
        if (info.channels > 0)
        {
          info.time = (float)(((float)dataSize / (info.channels * (info.bitsPerSample / 8))) * (1.0 / (float)info.sampleRate)); // complicat, nu ? :)
        }
        else
        {
          info.time = 0.0;
        }

        // marchez inceputul datelor
        dataPosition = tell();

        // sar la urmatorul bloc
        if (!seek(info.length))
        {
         return false;
        }

        break;

      case VOC_DBLOCK_TERMINATOR :
        break;

      default :
        blockType = -1;
        return false;
    }
  }
  while(blockType != VOC_DBLOCK_TERMINATOR);

  // completez o structura de tip TSndInfo
  return true;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif


