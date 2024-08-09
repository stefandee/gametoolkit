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
//  Unit              : GPImgLoad.h
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * incarca un fisier in format intern Piron
//    * specificatii (versiunea 1.0)
//      - contine 2 parti : header si data
//      - headerul are cimpurile : id[4 bytes], width[4 bytes], height[4 bytes], bpp[4 bytes], encoding[4 bytes], versiune[4 bytes], fill [8 bytes]
//      - id are valoarea "pImg", adica (('g' << 24) | ('m' << 16) | ('I' << 8) | 'p')
//      - bpp poate lua 2 valori : 565 si 32, reprezentind formatul pixelilor din fisier
//      - versiune e organizat astfel : cel mai semnificativ word e major version, cel mai nesemnificativ word e minor version
//      - datele sint necompresate, fara nici un fel de aliniament;
//      - fill e folosit ca sa citesc headerul dintr'un singur acces, desi nu m'as baza pe asta :); oricum, headerul are 32 de octeti
//    * exista si metoda de Save, de vreme ce e format propriu
//    * am ales 32 de biti in loc de 24, pentru ca uzual accesul la suprafetele aliniate (quad, aici) sint mai rapide;
//      se va ocupa mai mult spatiu pe disc (1/3 mai mult), dar incarcarea va fi mai rapida (fara conversii, deci)
//    * LoadAlign incarca aliniat; adica suprafata (char* data) poate avea width mai mare ca width'ul imaginii din fisier
//      a fost nevoie pentru ca DirectX'ul stie numai suprafete aliniate (in fapt, bytesPitch = ddsd.lPitch); daca bytesPitch == -1, atunci nu se mai face aliniament (bytesPitch == info.width)
//
//  Istorie           :
//    [16.01.2000] - [Karg] - a fost creat
//    [22.01.2000] - [Karg] - am implementat toate functiile posibile : Save, Load16Bits_565 si Load32Bits
//    [23.01.2000] - [Karg] - am adaugat un parametru de align la metodele de Load (pentru alinierea suprafetei)
//    [28.01.2000] - [Kafka] - removed method Open (no need because is done in base class) 
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GPImgLoadH
#define GPImgLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "GLoad.h"

// diverse constante
#define PIMG_ID            (unsigned __int32)(('g' << 24) | ('m' << 16) | ('I' << 8) | 'p')
#define PIMG_MAJOR_VERSION 1
#define PIMG_MINOR_VERSION 0
#define PIMG_EXT           ".pImage"
#define PIMG_SHORT_EXT     ".pImg"
#define PIMG_ENCODING_NONE 0
#define PIMG_BUFFER_SIZE   32768

// tipuri aditionale
// enum TPImgBitCount {PIMG_BITS_555, PIMG_BITS_565, PIMG_BITS_32};

//header
typedef struct
{
  unsigned __int32 id;
  unsigned __int32 version;
  unsigned __int32 width;
  unsigned __int32 height;
  unsigned __int32 bpp;
  unsigned __int32 encoding;
  unsigned __int32 fill[2];
} TPImgFileHeader;

// clasa pentru incarcat fisiere PImg
class GPImgLoad : public GLoad
{
  private:
    TPImgFileHeader header;

  protected:
    virtual bool ReadHeader ();
    bool         Load16Bits_565 (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool         Load32Bits     (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  public:
    GPImgLoad  ();
    virtual bool Load        (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    ~GPImgLoad ();
};

class GPImgSave : public GSave
{
  public:
    GPImgSave(CPString _fileName) : GSave(_fileName) {};
    virtual bool Save        (unsigned char* data, int width, int height, TColorFormat dataBpp, TColorFormat fileBpp);
    virtual bool SaveAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp, TColorFormat fileBpp);
    virtual ~GPImgSave   () {};

  protected:
    GPImgSave();                 // no impl
    GPImgSave(const GPImgSave&);  // no impl
};

#endif
