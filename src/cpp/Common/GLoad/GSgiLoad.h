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
//  Unit              : GSgiLoad.h (experimental)
//
//  Versiune          : 0.9
//
//  Descriere         :
//    * incarca fisiere in format SGI, specifice masinilor SGI
//    * e mai mult o incercare de imi exersa skill'urile de portare; de vreme ce ACDSEE'ul suporta
//      acest format, nu vad de ce nu l'am suporta si noi
//    * formatul a fost luat dintr'un fisier scris de Paul Haeberli de la Sillicon Graphics
//    * porcaria este ca datele sint scrise in fisier in Motorola byte order; de aceea
//      BitOp.h se va imbogati cu niste functii noi
//    * suport de load pentru 1 byte per channel, NORMAL colormap, BW/RGB24/RGB32, RLE/VERBATIM
//
//  Istorie           :
//    [26.02.2000] - [Karg] - a fost creat fisierul
//    [02.03.2000] - [Karg] - am scris ReadHeader (am facut rost de imagine SGI pe care sa testez loaderul)
//    [03.03.2000] - [Karg] - am scris LoadRle si LoadScanTable;
//                            de adaugat un test ca uncompresia rle sa nu depaseasca info.width
//    [04.03.2000] - [Karg] - am scris LoadVerbatim;
//                          - am adaugat cod de dealocare a memoriei in LoadRle
//                          - am sters metoda de Save (e inutila)
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GSgiLoadH
#define GSgiLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "GLoad.h"

// constante specifice formatului
#define SGI_FILEID   474
#define SGI_EXT      ".sgi"
#define SGI_RGB_EXT  ".rgb"
#define SGI_RGBA_EXT ".rgba"
#define SGI_BW_EXT   ".bw"

// enums
enum TSgiStorage  {SGI_VERBATIM = 0, SGI_RLE = 1};
enum TSgiColorMap {SGI_NORMAL   = 0, SGI_DITHERED, SGI_SCREEN, SGI_COLORMAP};
enum TSgiBpc      {SGI_BPC1     = 1, SGI_BPC2 = 2};

// headerul fisierului
typedef struct
{
  unsigned __int16 fileId;
  unsigned __int8  storage;
  unsigned __int8  bpc;
  unsigned __int16 dimension;
  unsigned __int16 xSize;
  unsigned __int16 ySize;
  unsigned __int16 zSize;
  unsigned __int32 pixMin;
  unsigned __int32 pixMax;
  unsigned __int32 dummy;
  unsigned __int8  imageName[80];
  unsigned __int32 colorMapId;
  unsigned __int8  filler[404];
} TSgiFileHeader;

// clasa efectiva 
class GSgiLoad : public GLoad
{
  private:
    TSgiFileHeader fileHeader;

  protected:
    virtual bool ReadHeader   ();
    bool         LoadScanTable(unsigned tablesLength, unsigned __int32 *startTable, unsigned __int32 *lengthTable);
    bool         LoadRle      (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool         LoadVerbatim (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  public:
    GSgiLoad();
    virtual bool Load        (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    ~GSgiLoad();
};



#endif
