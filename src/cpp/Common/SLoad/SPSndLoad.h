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
//  Unit              : loader pentru fisiere .pSnd (PSndLoad)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * .pSnd (.pSound) este format intern Piron
//    * headerul definit mai jos ar trebui sa fie explicit
//    * exista si compresie; deocamdata fisierele nu sint compresate, dar va
//      o compresie stil ADPCM(?)   
//
//  Istorie           :
//    [11.02.2000] - [Karg] - am creat fisierul, clasa si am scris metodele
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef SPSndLoadH
#define SPSndLoadH
//---------------------------------------------------------------------------
// clasa de baza
#include "SLoad.h"

// constante
#define PSND_ID               "pSnd"
#define PSND_ID_SIZE          4
#define PSND_DESC_SIZE        64
#define PSND_MAJOR_VERSION    1
#define PSND_MINOR_VERSION    0
#define PSND_COMPRESSION_NONE 0

// structurile de date aferente formatului fisierului .pSnd
typedef struct
{
  unsigned __int8  id[PSND_ID_SIZE];
  unsigned __int8  description[PSND_DESC_SIZE];
  unsigned __int32 version;
  unsigned __int32 compression;
  unsigned __int32 channels;
  unsigned __int32 sampleRate;
  unsigned __int32 bitsPerSample;
} TPSndFileHeader;

// clasa de incarcare a fisierelor .pSnd
class SPSndLoad : public SLoad
{
  private:
    TPSndFileHeader fileHeader;

  protected:
    virtual bool ReadHeader(/*int sampleRate, int bitsPerSample, int channels*/);

  public:
    SPSndLoad    ();
    virtual bool Load       (unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2);
    //virtual bool Save       (char* fileName, unsigned char* data,  int dataSize, TSndInfo info, char* description);
    unsigned char* GetDescription();
};

#endif
