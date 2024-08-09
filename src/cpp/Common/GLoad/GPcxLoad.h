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
//  Unit              : GPcxLoad.h
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * incarca un fisier in format PCX (de la ZSoft)
//    * evident, contine clasa si formatul headerului fisierului PCX
//    * suport numai pentru PCX 256 culori si 24 bits
//    * LoadAlign incarca aliniat; adica suprafata (char* data) poate avea width mai mare ca width'ul imaginii din fisier
//      a fost nevoie pentru ca DirectX'ul stie numai suprafete aliniate (in fapt, bytesPitch = ddsd.lPitch); daca bytesPitch == -1, atunci nu se mai face aliniament (bytesPitch == info.width)
//
//  Istorie           :
//    [10.01.2000] - [Karg] - a fost creat
//    [14.01.2000] - [Karg] - forma stabila pentru algoritmul de decodare; load8bits - RGB_565
//    [15.01.2000] - [Karg] - load8bits, RGB_32/RGB_24
//    [16.01.2000] - [Karg] - load24Bits: teste + multe buguri si nervi din cauza documentatiei foarte proaste la pcx
//    [17.01.2000] - [Karg] - load24Bits: completata; se pare ca photoshop scoate niste pcx'uri ciudate (mai lungi decit altele)
//                            din cauza asta am pierdut vreme debuguind cod care nu avea defecte
//    [23.01.2000] - [Karg] - am adaugat un parametru de align la metodele de Load (pentru alinierea suprafetei)
//    [28.01.2000] - [Kafka] - removed method Open (no need because is done in base class)
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GPcxLoadH
#define GPcxLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "GLoad.h"

// diverse constante
#define PCX_ID          10
#define PCX_ENCODING    1
#define PCX_PALLETE_ID  12
#define PCX_BUFFER_SIZE 32768

// tipuri aditionale
enum TPcxBitCount {PCX_BITS_1, PCX_BITS_2, PCX_BITS_4, PCX_BITS_8};

typedef struct
{
  unsigned __int8 red;
  unsigned __int8 green;
  unsigned __int8 blue;
} TPcxRgb;

// header
typedef struct
{
  unsigned __int8  manufacturer, version, encoding, bitsPerPixel;
  unsigned __int16 xMin, yMin, xMax, yMax;
  unsigned __int16 hDpi, vDpi;
  unsigned __int8  colorMap[48];
  unsigned __int8  reserved;
  unsigned __int8  nPlanes;
  unsigned __int16 bytesPerLine;
  unsigned __int16 palleteInfo;
  unsigned __int16 hScreenSize;
  unsigned __int16 vScreenSize;
  unsigned __int8  filler[54];
} TPcxFileHeader;

// clasa pentru incarcat fisiere PCX
class GPcxLoad : public GLoad
{
  private:
    TPcxFileHeader header;

  protected:
    virtual bool ReadHeader ();
    bool Load8Bits          (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool Load24Bits         (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  public:
    GPcxLoad  ();
    virtual bool Load      (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    ~GPcxLoad ();
};


#endif
