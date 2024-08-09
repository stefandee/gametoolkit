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
//---------------------------------------------------------------------------
#ifndef prims16H
#define prims16H
//---------------------------------------------------------------------------
#include "PP_basetypes.h"

// forward declarations
struct CPRect;

// misc functions
void setGlobalClipRect(const CPRect& rc);
CPRect getGlobalClipRect();

// functii de pixel
void putPixelNC (char *p, long next_scan, int x, int y, PP_uint32_t color);
void putPixel   (char *p, long next_scan, int x, int y, PP_uint32_t color);
PP_uint32_t getPixel   (char *p, long next_scan, int x, int y);

// functii de linie
extern void vLine (char *p, long next_scan, int x,  int yStart, int yEnd, PP_uint32_t color);
extern void hLine (char *p, long next_scan, int xs, int xe,     int y,    PP_uint32_t color);
void opaqueHLine  (char *p, long next_scan, int xs, int xe, int y, PP_uint32_t color, PP_int16_t opacity);
void darkHLine    (char *p, long next_scan, int xs, int xe, int y, PP_uint32_t darkSide);
void checkerHLine (char *p, long next_scan, int xs, int xe, int y, PP_uint32_t color);
void lineNC       (char *p, long next_scan, int xs, int ys, int xe, int ye, PP_uint32_t color);
void line(char *p, long next_scan, int xs, int ys, int xe, int ye, PP_uint32_t color);

//void vLine(BYTE *p, long next_scan, int x, int yStart, int yEnd, WORD color);
//void hLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD color);

// functii de dreptunghiuri
void darkRect    (char *p, long next_scan, int xstart, int ystart, int xend, int yend, PP_uint32_t darkSide);
void frameRect   (char *p, long next_scan, int xstart, int ystart, int xend, int yend, PP_uint32_t color);
void fillRect    (char *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, PP_uint32_t color);
void selRect     (char *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, PP_uint32_t color);
void checkerRect (char *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, PP_uint32_t color);

//void darkRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD darkSide);
//void frameRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color);
//void fillRect(BYTE *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, WORD color);
//void selRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, WORD color);
//void checkerRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color);

// functii de cercuri
void circle(char *p, long next_scan, int x, int y, int radius, PP_uint32_t color);

// triangle
void fillTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, PP_uint32_t color);

#endif


