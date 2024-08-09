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

// functii misc
void setClipRegion(int xl, int yt, int xr, int yb);

// functii de pixel
void putPixelNC (__int8 *p, long next_scan, int x, int y, unsigned __int16 color);
void putPixel   (__int8 *p, long next_scan, int x, int y, unsigned __int16 color);
unsigned __int16 getPixel   (char *p, long next_scan, int x, int y);

// functii de linie
void vLine (__int8 *p, long next_scan, int x,  int yStart, int yEnd, unsigned __int16 color);
void hLine (__int8 *p, long next_scan, int xs, int xe,     int y,    unsigned __int16 color);
void opaqueHLine  (__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 color, __int16 opacity);
void darkHLine    (__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 darkSide);
void checkerHLine (__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 color);

//void vLine(BYTE *p, long next_scan, int x, int yStart, int yEnd, WORD color);
//void hLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD color);

// functii de dreptunghiuri
void darkRect    (__int8 *p, long next_scan, int xstart, int ystart, int xend, int yend, unsigned __int16 darkSide);
void frameRect   (__int8 *p, long next_scan, int xstart, int ystart, int xend, int yend, unsigned __int16 color);
void fillRect    (__int8 *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, unsigned __int16 color);
void selRect     (__int8 *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, unsigned __int16 color);
void checkerRect (__int8 *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, unsigned __int16 color);

//void darkRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD darkSide);
//void frameRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color);
//void fillRect(BYTE *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, WORD color);
//void selRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, WORD color);
//void checkerRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color);

// functii de cercuri
void Circle(__int8 *p, long next_scan, int x, int y, int radius, unsigned __int16 color);

// triangle
void Triangle(__int8 *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, unsigned __int16 color);

#endif


