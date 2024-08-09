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
//  Unit              : loader pentru fisiere ILBM
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * sint suportate 1-8bit ILBM cu paleta, 24 bit, fara masca si fara metodele de compresie HAM/HAM8
//    * I din ILBM insemna 'interleaved'; exista si IFF'uri LBM (non-interleaved) 
//    * ILBM e un subformat al formatului "parinte", IFF, scos de EA
//    * as fi putut face un loader general de IFF'uri, dar nu acesta este scopul
//    * IFF'urile pot fi folosite (dupa cum au fost gindite) pentru o gramada de aplicatii - inclusiv baze de date
//    * cu IFF'ul s'a incercat realizarea unui format general pentru toate fisierele de date; evident, nu au reusit :)
//    * de vreme ce e interchange, totusi nu se specifica daca numele chunk'urilor e in ascii sau sint valori numerice care eventual traduse doar ascii au doar relevanta;
//      oricum, problema asta nu afecteaza portarea pe linux sau sun;
//    * ar trebui o metoda care sa returneze paleta, dar deocamdata nu as avea ce face cu ea
//
//  Istorie           :
//    [05.03.2000] - [Karg] - am creat fisierul, header struct si clasa
//                          - am implementat ReadHeaders, pentru a face niste teste cu imagini ILBM true color
//    [06.03.2000] - [Karg] - m'am chinuit sa implementez LoadPalleted, dar din lipsa unei documentatii
//                            solide despre decompresie, am esuat lamentabil
//    [07.03.2000] - [Karg] - am gasit lbmlib.c in niste surse de quake si il folosesc ca material de inspiratie
//    [09.03.2000] - [Karg] - am descifrat misterul compresiei pe plane de biti si am adaugat o metoda de incarcare
//                            a paletei de culori (care, de altfel mi'a mincat 3 ore, pentru ca repozitiona pozitia
//                            in fisier - prostul de mine :)
//                            am descifrat si misterul compresiei pe 24 de biti; este folosita tot compresia pe plane
//                            dar nu mai e folosita o paleta pentru a accesa culoarea
//                            metoda de load e una singura, InternalLoad
//                            am adaugat doua metode private pentru disk caching - s'a redus destul de mult timpul de incarcare
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GILbmLoadH
#define GILbmLoadH
//---------------------------------------------------------------------------

// clasa de baza
#include "GLoad.h"

// extensii
#define ILBM_EXT1 ".iff"
#define ILBM_EXT2 ".ilbm"

// chunk'uri
#define ILBMC_SIZE   4
#define ILBMC_FILEID "FORM"
#define ILBMC_DATAID1 "ILBM"
#define ILBMC_DATAID2 "PBM "
#define ILBMC_BMHD   "BMHD"
#define ILBMC_BODY   "BODY"
#define ILBMC_CMAP   "CMAP"

// misc
#define ILBM_FILE_BUFFER_SIZE 32768

enum TILbmMask {ILBM_NONE = 0, ILBM_HASMASK, ILBM_TRANSPARENT, ILBM_LASSO};
enum TILbmCompression {ILBM_NOCOMPRESSION = 0, ILBM_BYTERUN = 1};

// structurile de date
typedef unsigned __int8 TChunkId[ILBMC_SIZE];

typedef struct
{
  TChunkId         chunkId;
  unsigned __int32 chunkSize;
} TILbmChunk; // general chunk structure

typedef struct
{
  unsigned __int16 width, height;
  __int16          xPos, yPos;
  unsigned __int8  nPlanes;
  unsigned __int8  masking;
  unsigned __int8  compression;
  unsigned __int8  pad;
  unsigned __int16 transparentColor;
  unsigned __int8  xAspect, yAspect;
  __int16          pageWidth, pageHeight;
} TILbm_Bmp_Header; // bitmap structure

class GILbmLoad : public GLoad
{
  private:
    unsigned __int8  *fileBuffer;
    unsigned __int32 fileBufferIndex;
    unsigned __int32 bodyChunkPos;
    unsigned __int32 cMapChunkPos;
    bool             hasCMap;
    bool             hasBmhd;
    TILbm_Bmp_Header bmpHeader;
    TILbmChunk       fileHeader;
    bool            InitFileBuffer();
    unsigned __int8 NextFileBufferByte ();
    bool LoadCMap                  (unsigned __int8* pallete, int colorNumber);
    bool DecodeBitPlane            (unsigned __int8 *bitPlane, int rowSize);

  protected:
    virtual bool ReadHeader();
    bool InternalLoad      (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  public:
    GILbmLoad();
    virtual bool Load        (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    ~GILbmLoad();
};

#endif
