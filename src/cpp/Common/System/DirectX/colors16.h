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
// Descriere : pixel conversion functions, pentru 16-bit color modes
//---------------------------------------------------------------------------
#ifndef colors16H
#define colors16H
//---------------------------------------------------------------------------

#include "ColorConv.h"

// Color format for the graphical application
//typedef int TColor32;

extern TColorFormat ApplicationBpp;

unsigned __int16  combineColor(unsigned __int32 a, unsigned __int32 b, unsigned __int32 procent);
unsigned __int16 darkColor(unsigned __int32 a, unsigned __int32 darkSide);
unsigned __int16 makeRGB16(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b);
void InitColor(int redBitMask, int greenBitMask, int blueBitMask);

#endif
