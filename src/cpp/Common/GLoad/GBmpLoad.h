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
//  Unit              :  Bmp File Loader (GBmpLoad)
//
//  Versiune          :  1.0
//
//  Descriere         :
//    * incarca un fisier Microsoft bitmap;
//    * contine structurile pentru headerul fisierului, plus alte date specifice;
//    * nu sint suportate bitmapuri compresate cu RLE_4 si RLE_8, algoritmii urmind sa fie
//      implementati in versiuni viitoare;
//    * nu sint suportate bitmapurile cu culori pe 16 biti; ele vor fi suportate in viitoarele versiuni
//    * exista suport pentru 8 bit necompresat, 24 bit;
//    * LoadAlign incarca aliniat; adica suprafata (char* data) poate avea width mai mare ca width'ul imaginii din fisier
//      a fost nevoie pentru ca DirectX'ul stie numai suprafete aliniate (in fapt, bytesPitch = ddsd.lPitch); daca bytesPitch == -1, atunci nu se mai face aliniament (bytesPitch == info.width)
//
//  Istorie           :
//    [13.12.1999] - [Karg] - a fost creat
//    [14.12.1999] - [Karg] - am scris structurile de header
//    [15.12.1999] - [Karg] - am implementat metodele Open, Load, Close, ReadHeaders
//    [19.12.1999] - [Karg] - implementarea metodelor Load24Bits, Load8bits, Load555, Load565 (doar schelet)
//    [11.01.2000] - [Karg] - am reorganizat putin metodele
//    [16.01.2000] - [Karg] - am eliminat bugurile legate de align la dword/quad word si bugul de la GetTGInfo
//    [23.01.2000] - [Karg] - am adaugat un parametru de align la metodele de Load (pentru alinierea suprafetei)
//    [28.01.2000] - [Kafka] - removed method Open (no need because is done in base class)
//    [20.01.2002] - [Karg]  - added GBmpSave class; the "Save" method needs to be tested;
//                             saving 24/32 bit surfaces need implementation (but I don't bother :)
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GBmpLoadH
#define GBmpLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "GLoad.h"

#define BMP_ID "BM"
#define BMP_ARRAY "BA"
#define COLOR_ICON "CI"
#define COLOR_POINTER "CP"
#define ICON "IC"
#define POINTER "PT"
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// tipuri de compresie
enum TBmpShrink   {SHRINK_NONE, SHRINK_RLE4, SHRINK_RLE8, SHRINK_BITFIELDS};
enum TBmpBitCount {BITS_4, BITS_8, BITS_16, BITS_24};

// structurile de date aferente formatului fisierului (MicroSoft) BMP
typedef struct
{
  char  id[2];
  __int32 fileSize;
  __int32 reserved;
  __int32 bmpFileOffset;
} TBmpFileHeader;

typedef struct
{
  __int32 headerSize;
  __int32 imgWidth;
  __int32 imgHeight;
  __int16 planes;
  __int16 bpp;
  __int32 compressionType;
  __int32 imgSize;
  __int32 xPelsPerMeter;       // pixeli / metru
  __int32 yPelsPerMeter;       // pixeli / metru
  __int32 usedColors;
  __int32 importantColors;
} TBmpInfoHeader;

typedef struct
{
  unsigned __int8 blue;
  unsigned __int8 green;
  unsigned __int8 red;
  unsigned __int8 filler;
} TRgb;

// clasa de incarcare a bitmapului, derivata din GLoad
class GBmpLoad : public GLoad
{
  public:
    GBmpLoad();
    virtual bool Load        (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    ~GBmpLoad();

  protected:
    virtual bool ReadHeader();
    bool Load8Bits  (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool Load16Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool Load24Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  private:
    TBmpFileHeader fileHeader;
    TBmpInfoHeader infoHeader;
};

class GBmpSave : public GSave
{
  public:
    GBmpSave          (CPString _fileName) : GSave(_fileName) {};
    virtual bool Save              (unsigned char* data, int width, int height, TColorFormat dataBpp, TColorFormat fileBpp);
    virtual bool SaveAligned       (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp, TColorFormat fileBpp);
    virtual ~GBmpSave   () {};

  protected:
    GBmpSave();                 // no impl
    GBmpSave(const GBmpSave&);  // no impl

    TBmpFileHeader mFileHeader;
    TBmpInfoHeader mInfoHeader;

    bool WriteHeaders();
};

#endif
