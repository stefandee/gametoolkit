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
#include <string.h>
#pragma hdrstop

#include "SWavLoad.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

SWavLoad::SWavLoad()
{
  memset(&fileHeader, 0, sizeof(TWavFileHeader)); // de la Kafka citire 
}
//---------------------------------------------------------------------------

bool SWavLoad::Load(unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2)
{
  // seek to beginning of data
  if (!seek(sizeof(TWavFileHeader)))     
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

// Descriere   : citeste headerul(?) dintr'un fisier .WAV
// Param       : sampleRate, bps, channels care sint cerute pentru a deschide acest fisier
//               (daca parametrii sint diferiti de ceea ce se citeste din fisier, atunci e eroare)
// Rezultat    : true/false
// Comentarii  : se pare ca headerul nu e chiar header, ci o colectie de chunk'uri; loaderul trebuie sa arate altfel
bool SWavLoad::ReadHeader(/*int sampleRate, int bitsPerSample, int channels*/)
{
  if (!rewind())
  {
    return false;
  }

  // citirea cimpurilor headerului; din cauza ca structurile pot fi aliniate dword/qword, trebuie facuta citirea
  // fiecarui cimp individual
  if (!read(&fileHeader.riffId,       4 * sizeof(__int8)))
  {
    return false;
  }

  if (!read(&fileHeader.chunkLength,   sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.waveId,        4 * sizeof(__int8)))
  {
    return false;
  }

  if (!read(&fileHeader.fmtId,        4 * sizeof(__int8)))
  {
    return false;
  }

  if (!read(&fileHeader.fmtChunkLength, sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.fmtTag,         sizeof(__int16)))
  {
    return false;
  }

  if (!read(&fileHeader.chnNumber,      sizeof(__int16)))
  {
    return false;
  }

  if (!read(&fileHeader.sampleRate,     sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.avgSamplesPerSec,  sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.blockAlign,     sizeof(__int16)))
  {
    return false;
  }

  if (!read(&fileHeader.bitsPerSample,  sizeof(__int16)))
  {
    return false;
  }

  if (!read(&fileHeader.dataId,         4 * sizeof(__int8)))
  {
    return false;
  }

  if (!read(&fileHeader.dataLength,     sizeof(__int32)))
  {
    return false;
  }

  // verificari de validitate a fisierului WAV
  if (strncmp((char*)fileHeader.riffId, WAV_RIFF_ID, 4) != 0)
  {
    return false;
  }
  if (strncmp((char*)fileHeader.waveId, WAV_WAVE_ID, 4) != 0)
  {
    return false;
  }
  if (strncmp((char*)fileHeader.fmtId,  WAV_FMT_ID,  4) != 0)
  {
    return false;
  }
  if (strncmp((char*)fileHeader.dataId, WAV_DATA_ID, 4) != 0)
  {
    return false;
  }

  // completez structura TSndInfo
  info.channels      = fileHeader.chnNumber;

  info.sampleRate    = fileHeader.sampleRate;
  if (fileHeader.sampleRate == 0)
  {
    return false;
  }

  info.bitsPerSample = fileHeader.bitsPerSample;
  info.blockAlign    = fileHeader.blockAlign;    // se poate calcula ca channels* (bitsPerSample / 8)
  info.length        = fileHeader.dataLength;

  if (fileHeader.chnNumber > 0)
  {
    info.time = (float)(((float)fileHeader.dataLength / (fileHeader.chnNumber * (fileHeader.bitsPerSample / 8))) * (1.0 / (float)fileHeader.sampleRate)); // complicat, nu ? :)
  }
  else
  {
    info.time = 0.0;
  }  

  return true;
}
//---------------------------------------------------------------------------


