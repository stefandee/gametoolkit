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
//  Unit              : loader pentru fisiere .AU (SAuLoad)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * .AU e format de la Sun/NeXT; e bine pentru portabilitate sa avem si suport
//      pentru astfel de fisiere
//
//  Istorie           :
//    [9.02.2000] - [Karg] - am creat fisierul
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef SAuLoadH
#define SAuLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "SLoad.h"

// constante
#define AU_FILE_ID                    0x2e736e64
#define AU_UNKNOWN_SIZE               -1
#define AU_FILE_ENCODING_MULAW_8      01 // 8-bit ISDN u-law
#define AU_FILE_ENCODING_LINEAR_8     02 // 8-bit linear PCM
#define AU_FILE_ENCODING_LINEAR_16    03 // 16-bit linear PCM
#define AU_FILE_ENCODING_LINEAR_24    04 // 24-bit linear PCM
#define AU_FILE_ENCODING_LINEAR_32    05 // 32-bit linear PCM
#define AU_FILE_ENCODING_FLOAT        06 // 32-bit IEEE floating point
#define AU_FILE_ENCODING_DOUBLE       07 // 64-bit IEEE floating point
#define AU_FILE_ENCODING_ADPCM_G721   23 // 4-bit CCITT g.721 ADPCM
#define AU_FILE_ENCODING_ADPCM_G722   24 // CCITT g.722 ADPCM
#define AU_FILE_ENCODING_ADPCM_G723_3 25 // CCITT g.723 3-bit ADPCM
#define AU_FILE_ENCODING_ALAW_8       27 // 8-bit ISDN A-law
#define AU_FILE_ENCODING_ADPCM_G723_5 26 // CCITT g.723 5-bit ADPCM
#define AU_LOADER_BUFFER_SIZE         8192 // multiplu de 2

// structurile de date aferente formatului fisierului .AU
typedef struct
{
  unsigned __int32 id;
  unsigned __int32 headerSize;
  unsigned __int32 dataSize;
  unsigned __int32 encoding;   // din constantele AU_FILE_... definite mai sus
  unsigned __int32 sampleRate;
  unsigned __int32 channels;
} TAuFileHeader;

// clasa de incarcare .AU
class SAuLoad : public SLoad
{
  private:
    TAuFileHeader   fileHeader;
    unsigned __int8 buffer[AU_LOADER_BUFFER_SIZE];

  protected:
    virtual bool ReadHeader();

  public:
    SAuLoad();
    virtual bool Load       (unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2);
};

#endif
