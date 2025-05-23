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
//-----------------------------------------------------------------------------
//  Unit              : Wav file loader (SWavLoad)
//
//  Versiune          : 1.01
//
//  Descriere         :
//    * incarca fisiere in format .WAV, de la Microsoft
//    * trebuie sa se lege de DirectSound, deci Load'ul are parametri cam ciudati
//
//  Istorie           :
//    [4.02.2000] - [Karg] - a fost creat fisierul
//    [7.02.2000] - [Karg] - am creat clasa SWavLoad
//    [8.02.2000] - [Karg] - am terminat de implementat toate metodele si am scos erorile de compilare
//-----------------------------------------------------------------------------

#ifndef SWavLoadH
#define SWavLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "SLoad.h"

// constante
#define WAV_RIFF_ID "RIFF"
#define WAV_WAVE_ID "WAVE"
#define WAV_FMT_ID  "fmt "
#define WAV_DATA_ID "data"

// structurile de date aferente formatului fisierului .WAV
typedef struct
{
  unsigned __int8  riffId[4];      // is RIFF
  unsigned __int32 chunkLength;
  unsigned __int8  waveId[4];      // is WAVE
  unsigned __int8  fmtId[4];       // is fmt%20
  unsigned __int32 fmtChunkLength; //
  unsigned __int16 fmtTag;
  unsigned __int16 chnNumber;      // 1 = mono, 2 = stereo
  unsigned __int32 sampleRate;
  unsigned __int32 avgSamplesPerSec;
  unsigned __int16 blockAlign;
  unsigned __int16 bitsPerSample;
  unsigned __int8  dataId[4];
  unsigned __int32 dataLength;
} TWavFileHeader;

// clasa de incarcare a fisierelor .WAV
class SWavLoad : public SLoad
{
  private:
    TWavFileHeader fileHeader;

  protected:
    virtual bool ReadHeader(/*int sampleRate, int bitsPerSample, int channels*/);

  public:
    SWavLoad();
    virtual bool Load       (unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2);
};

#endif
