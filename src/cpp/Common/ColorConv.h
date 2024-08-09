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
//  Unit              : Color Conversions (ColorConv.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * macrouri/functii de conversie intre diverse formate de culori
//
//  Istorie           :
//    [15.12.1999] - [Karg] - a fost creat
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef ColorConvH
#define ColorConvH
//---------------------------------------------------------------------------

#include "PP_basetypes.h"

// color format : RGB, CMYK, YUV, etc
enum TColorFormat {BW, RGB_4, RGB_8, RGB_16, RGB_24, RGB_32, RGB_555, RGB_565, ABGR_8888};

int gfGetRValue(TColor32 _color);
int gfGetGValue(TColor32 _color);
int gfGetBValue(TColor32 _color);
int gfGetAValue(TColor32 _color);

#endif
