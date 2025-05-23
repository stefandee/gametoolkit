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
//  Unit              : VOC file loader (SVocLoad)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * e un format destul de comun si vine de la Creative
//    * partea proasta e ca documentatia e destul de saraca si se poate ca loaderul
//      sa nu incarce TOATE voc'urile existente
//    * structura initiala a VOC'ului e cam ciudata; probabil au vrut sa faca ceva
//      in genul modului, cu markere si repeat count'uri; evident, numai un program
//      special poate prelucra astfel de VOC'uri
//    * structura noua a VOC'ului, cu blocul TYPE9, a fost introdusa probabil tocmai
//      pentru ca un VOC sa poate fi prelucrat cu metode normale;
//    * de aceea, loaderul nu suporta decit TYPE9 VOC.  
//
//  Istorie           :
//    [9.02.2000]  - [Karg] - am creat fisierul, clasa si structurile de date
//    [10.02.2000] - [Karg] - am scris metodele ReadHeader si Load
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef SVocLoadH
#define SVocLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "SLoad.h"

// constante
#define VOC_ID "Creative Voice File"
#define VOC_ID_LENGTH 19

#define VOC_DBLOCK_TERMINATOR 0
#define VOC_DBLOCK_SOUNDDATA  1
#define VOC_DBLOCK_SOUNDCONT  2
#define VOC_DBLOCK_SILENCE    3
#define VOC_DBLOCK_MARKER     4
#define VOC_DBLOCK_ASCII      5
#define VOC_DBLOCK_REPEAT     6
#define VOC_DBLOCK_ENDREPEAT  7
#define VOC_DBLOCK_EXTENDED   8
#define VOC_DBLOCK_TYPE9      9

#define VOC_COMPRESSION_8bits  0
#define VOC_COMPRESSION_4bits  1
#define VOC_COMPRESSION_26bits 2
#define VOC_COMPRESSION_2bits  3
#define VOC_COMPRESSION_DAC    3

// structurile de date aferente formatului fisierului .VOC
typedef struct
{
  unsigned __int8  id[VOC_ID_LENGTH];
  unsigned __int8  skipPrint;
  unsigned __int16 firstBlockOffset;
  unsigned __int16 version;
  unsigned __int16 crap;
} TVocFileHeader;

// structura blocului de date TYPE9
typedef struct
{
  unsigned __int32 sampleRate;
  unsigned __int8  bitsPerSample;
  unsigned __int8  channels;
  unsigned __int8  nothing;
  unsigned __int8  padding[5];
} T9Header;

// clasa de incarcare a fisierelor VOC
class SVocLoad : public SLoad
{
  private:
    unsigned __int32 dataPosition;
    TVocFileHeader fileHeader;

  protected:
    virtual bool ReadHeader();

  public:
    SVocLoad  ();
    virtual bool Load    (unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2);
};

#endif
