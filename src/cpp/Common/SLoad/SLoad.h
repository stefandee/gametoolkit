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
//  Unit              : Clasa de baza pentru loaderele de fisiere grafice (SLoad)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * clasa de baza, care nu ar trebui instantiata
//    * nu vor fi prea multe function comments; in mare, seamana cu toate celelalte loadere (GLoad)
//    * metoda Load are 2 parametri char* data, pentru compatibilitate cu DirectSound
//    * ar trebui facut un fisier cu constante si definitii generale legate de sistemul de sunet (enum pt mono/stereo, etc)
//
//  Istorie           :
//    [04.02.2000] - [Karg] - a fost creat fisierul si clasa
//    [06.02.2000] - [Karg] - am scris codul la metodele de Open, GetInfo, Close si am propus o forma pentru TSndInfo
//    [14.05.2004] - [Karg] - replaced the file IO by Stream
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef SLoadH
#define SLoadH
//---------------------------------------------------------------------------

#include "Stream.h"

// informatii despre imaginea din fisier
typedef struct
{
  int     channels;
  int     sampleRate;
  int     bitsPerSample;
  int     blockAlign;     // ?
  int     length;         // in octeti
  float   time;           // in secunde
} TSndInfo;

// clasa de baza
class SLoad
{
  public:
    SLoad();
    virtual bool Open       (const char* fileName/*, int sampleRate, int bitsPerSample, int channels*/);
    virtual void GetSndInfo (TSndInfo* fileInfo);
    virtual bool Load       (unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2);
    virtual bool Close      ();
    virtual ~SLoad();

  protected:
    TSndInfo       info;
    virtual bool   ReadHeader(/*int sampleRate, int bitsPerSample, int channels*/) = 0;

    //
    // Stream interface
    //
    size_t       read(void* pMem, size_t nBytes);
    bool         seek(size_t offset);
    bool         skip(size_t size);  // skips n bytes of input
    bool         rewind();
    size_t       length();
    int          tell();

    PP::Stream* GetStream() { return mStream; }

  private:
    PP::Stream* mStream;
};


#endif
