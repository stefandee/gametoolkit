//-----------------------------------------------------------------------------
// Copyright (C) 2002 Mike Nordell                                           //
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Unit              :  PNG File Loader (GPngLoad)
//
//  Version           :  1.0
//
//  Description       :
//    * Tries to load a PNG image
//
//  History           :
//    22 Mar 2002 - TMN: Created
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GPNGLOAD_H
#define GPNGLOAD_H
//---------------------------------------------------------------------------

#include "GLoad.h"
#include "pngdib.h"


// clasa de incarcare a bitmapului, derivata din GLoad
class GPngLoad : public GLoad
{
  public:
    GPngLoad();
    ~GPngLoad();
    virtual bool Load        (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  private:
    virtual bool ReadHeader();
    bool Load8Bits  (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool Load24Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

	PNGD_P2DINFO   mInfo;
};

#endif	// GPNGLOAD_H
