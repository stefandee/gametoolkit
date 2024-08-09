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
//  Unit              : Graphic File Loader (GLoad)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * clasa de baza (parintele) pentru toate graphic stream decoder'ele
//    * toate functiile sint virtuale si urmeaza sa fie implementate de restul de clase
//    * vor fi suportate loadere pentru .PCX, .BMP, .TGA, .pImage
//    * vor exista doua tipuri de clase, una dintre ele avind W in nume; aceasta
//      clasa este pentru incarcare in HBITMAP si HDC, specifice Windows'ului;
//      celelalte clase sint facute pentru a fi portate [OBSOLETE, nu e nevoie]
//
//  Istorie           :
//    [11.12.1999] - [Karg] - a fost creat fisierul;
//    [11.01.2000] - [Karg] - am reorganizat putin metodele
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GLoadH
#define GLoadH
//---------------------------------------------------------------------------

#define BUFFER_SIZE 8192
#include <stdio.h>
#include "ColorConv.h"
#include "Stream.h"

// informatii despre imaginea din fisier
typedef struct __TGInfo
{
  int          width;
  int          height;
  int          bytesPerChannel;
  TColorFormat bpp;
} TGInfo;

class GLoad
{
  public:
    GLoad    ();
    virtual bool Open     (const char* fileName);
    virtual void GetTInfo (TGInfo* info);
    virtual void GetInfo  (int* width, int* height, TColorFormat* fileBpp);
    virtual bool Load     (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    virtual bool Close    ();
    virtual ~GLoad   ();

  protected:
    virtual bool ReadHeader();
    size_t       read(void* pMem, size_t nBytes);
	size_t       readInt(int* v);
	size_t       readShort(short* v);
    bool         seek(size_t offset);
    bool         skip(size_t size);  // skips n bytes of input
    bool         rewind();
    size_t       length();
    int          tell();

    PP::Stream* GetStream() { return mStream; }

    TGInfo           info;	// Yuck! Protected data!

  private:
    PP::Stream* mStream;
};

class GSave
{
  public:
    GSave(CPString _fileName);
    virtual bool Save           (unsigned char* data, int width, int height, TColorFormat dataBpp, TColorFormat fileBpp) = 0;
    virtual bool SaveAligned    (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp, TColorFormat fileBpp) = 0;
    virtual ~GSave () { delete mStream; };

  protected:
    size_t write(void* pMem, size_t nBytes);

  private:
    PP::Stream* mStream;
};

#endif
