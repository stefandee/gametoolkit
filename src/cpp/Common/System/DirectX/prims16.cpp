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
// Descriere :  primitive grafice; pot fi folosite oriunde in memorie
//              desi sint destinate lucrului cu suprafete directdraw
//---------------------------------------------------------------------------

//#include <vcl.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <math.h>
#pragma hdrstop

#include "prims16.h"
#include "colors16.h"
#include "fixed.h"
#include "logfile.h"

#define SWAP(x,y) ((x)^=(y)^=(x)^=(y))

int XLeft  = 0,
    XRight = 639,
    YTop   = 0,
    YBot   = 479;

/*--------------------------------------------------------------------------
 Description: face ce zice (sau cel putzin ase se lauda)
--------------------------------------------------------------------------*/
void setClipRegion(int xl, int yt, int xr, int yb)
{
    XLeft = xl;
    XRight = xr;
    YTop = yt;
    YBot = yb;
}
//---------------------------------------------------------------------------


// Descriere   : pune un pixel pe ecran
// Param       : x, y       - coordonatele
//               next_scan  - lungimea liniei de scan (pe orizontala)
//               color      - culoare, pe 16 biti
// Rezultat    : nimic
// Comentarii  : nici unul, pe moment
//void inline putPixelNC(BYTE *p, long next_scan, int x, int y, WORD color)
void putPixelNC (__int8 *p, long next_scan, int x, int y, unsigned __int16 color)
{
  *((unsigned __int16*)(p + y*next_scan + (x << 1))) = color;
}
//---------------------------------------------------------------------------


// Descriere   : pune un pixel pe ecran, facind clip la regiunea definita de
//               XLeft, XRight, YTop, YBottom
// Param       : ca mai sus
// Rezultat    : nimic
// Comentarii  : hmmm...tot nimic
//void inline putPixel(BYTE *p, long next_scan, int x, int y, WORD color)
void putPixel   (__int8 *p, long next_scan, int x, int y, unsigned __int16 color)
{
  if ((x >= XLeft) && (x <= XRight) &&
      (y >= YTop ) && (y <= YBot))
    *((unsigned __int16*)(p + y*next_scan + (x << 1))) = color;
}
//---------------------------------------------------------------------------

unsigned __int16  getPixel   (char *p, long next_scan, int x, int y)
{
  return *((unsigned __int16 *)(p + y*next_scan + (x << 1)));
}
//---------------------------------------------------------------------------

// Descriere   :  rutina de linie verticala
// Param       :  evidenti
// Rezultat    :
// Comentarii  :
//void inline vLine(BYTE *p, long next_scan, int x, int yStart, int yEnd, WORD color)
void inline vLine(__int8 *p, long next_scan, int x, int yStart, int yEnd, unsigned __int16 color)
{
  if ((x < XLeft) || (x > XRight)) return;
  if (yStart > yEnd) SWAP(yEnd, yStart);
  if (yStart < YTop) yStart = YTop;
  if (yEnd > YBot) yEnd = YBot;

  int y = yStart * next_scan + (x << 1);

  for(int yCount = yStart; yCount <= yEnd; yCount++)
  {
    *((__int16*)(p + y)) = color;
    y += next_scan;
  }
}

// Descriere   : rutina de linie orizontala
// Param       : evidenti
// Rezultat    : evident
// Comentarii  : face clipping la fereastra
//               e modificata dintr'o sursa microsoftiana
//void inline hLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD color)
void inline hLine(__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 color)
{
  int   dx;
  int   z;
  __int32 dw_color = 0;

  // nu desenam puncte - astea se fac cu putPixel :-))
  if (xe == xs) return;

  // inversam daca s'au dat invers
  if (xe < xs) SWAP(xe,xs);

  // teste de interioritate in fereastra
  if ((y  < YTop) ||   (y  > YBot)) return;
  if ((xs > XRight) || (xe < XLeft)) return;
  if (xs  < XLeft) xs = XLeft;
  if (xe  > XRight) xe = XRight;

  dx = xe - xs + 1;
  p  += 2 * xs + y*next_scan;

  if (dx > 1)
  {
      if (z = (__int32)p & 0x02)
      {
          while (z++ < 2)                  
          {
              *((__int16*)p) = color;
              p += 2;
              dx--;
          }
      }

      // calculez culorile
      dw_color = color << 16;
      dw_color |= color;

      while (dx > 1)
      {
          *((__int32*)p) = dw_color;
          p += 4;
          dx -= 2;
      }
  }

  // ultimul mohican...
  if (dx == 1) *((__int16*)p) = color;

  /*
  while (dx-- > 0)
  {
      *((WORD*)p++) = color;
  }
  */
}
//---------------------------------------------------------------------------

// Descriere   : rutina de linie orizontala opaque
// Param       : evidenti
// Rezultat    : evident
// Comentarii  : face clipping la fereastra
//               e modificata dintr'o sursa microsoftiana
//void inline opaqueHLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD color, WORD opacity)
void opaqueHLine  (__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 color, __int16 opacity)
{
  int   dx;

  // nu desenam puncte - astea se fac cu putPixel :-))
  if (xe == xs) return;

  // teste de interioritate in fereastra
  if ((y<YTop) || (y>YBot)) return;
  if ((xe>XRight) || (xe<XLeft)) return;
  if (xs<XLeft) xs = XLeft;
  if (xe>XRight) xe = XRight;

  // inversam daca s'au dat invers
  if (xe<xs) SWAP(xe,xs);

  dx = xe-xs+1;
  p += 2*xs + y*next_scan;

  for(int i = 0; i < dx; i++)
  {
    *((unsigned __int16*)p) = combineColor(*((unsigned __int16*)p), color, opacity);
    p += 2;
  }

}
//---------------------------------------------------------------------------

// Descriere   : rutina de linie orizontala care intuneca fundalul
// Param       : evidenti
// Rezultat    : evident
// Comentarii  : face clipping la fereastra
//               e modificata dintr'o sursa microsoftiana
//void darkHLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD darkSide)
void darkHLine    (__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 darkSide)
{
  int dx;
  int dw;

  dx = xe-xs+1;
  p += 2*xs + y*next_scan;

  for(int i = 0; i < dx/2; i++)
  {
    dw = *((__int32*)p);
    *((__int32*)p) = (darkColor( (dw & 0xFFFF0000) >> 16, darkSide) << 16) + darkColor( dw & 0xFFFF, darkSide);
//    *((WORD*)p) = *((WORD*)p) - darkSide;
    p += 4;
  }
}
//---------------------------------------------------------------------------

// Descriere   : deseneaza o linie din pixel in pixel
// Param       : evidenti
// Rezultat    : on screen
// Comentarii  : nu face clipping, pentru ca e apelata de checker rect
//void checkerHLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD color)
void checkerHLine (__int8 *p, long next_scan, int xs, int xe, int y, unsigned __int16 color)
{
  int   dx;
  int i;

  dx =  xe-xs+1; 
  p  += 2 * xs + y * next_scan;

  for(i = 0; i < dx / 2; i++)
  {
    *((__int16*)p) = color;
    p += 4;
  }
}
//---------------------------------------------------------------------------

// Rutina interna pentru linie orizontala
// Input     : xs, xe    - coordonatele de inceput si sfirsit (axa x)
//             next_scan - lungimea liniei de scan
//	            color     - culoare
// Cometarii : trebuie facuta in ASM
//             nu face clipping la fereastra
//             va fi folosita de Triangle pt umplere
/*
void FillSpecial(BYTE *p, int xs, int xe, DWORD color)
{
    int dx;
    int z;

    if ((xe>XRight) || (xe<XLeft)) return;
    if (xs<XLeft) xs = XLeft;
    if (xe>XRight) xe = XRight;
    if (xe<xs) SWAP(xe,xs);

    dx = xe-xs+1;
    p += xs;

    if (dx > 4)
    {
        if (z = (DWORD)p & 0x03)
        {
            while (z++ < 4)
            {
                *p++ = (BYTE)color;
                dx--;
            }
        }

        while (dx >= 4)
        {
            *((DWORD*)p) = color;
            p += 4;
            dx -= 4;
        }
    }

    while (dx-- > 0)
    {
        *p++ = (BYTE)color;
    }
}
//---------------------------------------------------------------------------
*/

// Rutina pentru desenat triunghiuri
// Inputs     :
// Comentarii : trebuie facuta in ASM
//		nu face clipping pe y
void Triangle(__int8 *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, unsigned __int16 _color)
{
    Fixed   d,d0;
    Fixed   x,x0;
    int     y;
    //int     c;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y) { SWAP(p0x,p1x); SWAP(p0y, p1y); }
    if (p0y > p2y) { SWAP(p0x,p2x); SWAP(p0y, p2y); }
    if (p1y > p2y) { SWAP(p1x,p2x); SWAP(p1y, p2y); }

    //
    //  check for quick out?
    //
    if (p2y - p0y == 0)
    {
        return;
    }

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0) d = (Fixed)(p2x - p0x) / (Fixed)(p2y - p0y);
    else d = (p2x - p0x) * 100;

    x  = p0x;
    y  = p0y;
    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0) d0 = (Fixed)(p1x - p0x) / (Fixed)(p1y - p0y);
        else d0 = (p1x - p0x) * 100;

        x0 = p0x;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                //FillSpecial(p, x, x0, c);
                hLine(p, next_scan, x, x0, y, _color);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
        else
        {
            while (y < p1y)
            {
                hLine(p, next_scan, x, x0, y, _color);
                //FillSpecial(p, x0, x, c);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
    }

    //
    // do the bottom.
    //

    if (p2y - p1y == 0)
    {
        return;
    }

    if ((p2y - p1y) != 0) d0 = (Fixed)(p2x - p1x) / (Fixed)(p2y - p1y);
    else d0 = (p2x - p1x) * 100;

    x0 = p1x;

    //
    // check for left or right fill
    //
    if (x < x0)
    {
        while (y < p2y)
        {
            hLine(p, next_scan, x, x0, y, _color);
            //FillSpecial(p, x, x0, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
    else
    {
        while (y < p2y)
        {
            hLine(p, next_scan, x, x0, y, _color);
            //FillSpecial(p, x0, x, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
}
//---------------------------------------------------------------------------


// Rutina de trasat linie
// Inputs     : xs,ys - coordonate de inceput
//		xe,ye - coordonate de sfirsit
//		color - culoare
// Comentarii : nu face clipping
void LineNC(__int8 *p, long next_scan, int xs, int ys, int xe, int ye, __int8 color)
{
    int deltax, deltay, numpixels, d, dinc1, dinc2,
    xinc1, xinc2, yinc1, yinc2,
    screeninc1, screeninc2;

    // Calculate deltax and deltay for initialisation
    deltax = abs(xe - xs);
    deltay = abs(ye - ys);

    // Initialize all vars based on which is the independent variable
    if (deltax >= deltay)
    {
        // x is independent variable
        numpixels  = deltax + 1;
        d 	   = (2 * deltay) - deltax;
        dinc1      = deltay << 1;
        dinc2 	   = (deltay - deltax) << 1;
        xinc1      = 1;
        xinc2      = 1;
        yinc1      = 0;
        yinc2 	   = 1;
    }
    else
    {
        // y is independent variable
        numpixels  = deltay + 1;
        d 	   = (2 * deltax) - deltay;
        dinc1 	   = deltax << 1;
        dinc2      = (deltax - deltay) << 1;
        xinc1      = 0;
        xinc2      = 1;
        yinc1      = 1;
        yinc2      = 1;
    }

    // Make sure x and y move in the right directions
    if (xs > xe)
    {
        xinc1 = - xinc1;
        xinc2 = - xinc2;
    }
    if (ys > ye)
    {
        yinc1 = - yinc1;
        yinc2 = - yinc2;
    }

    // Start drawing at <x1, y1>
    screeninc1 = yinc1 * next_scan + xinc1;
    screeninc2 = yinc2 * next_scan + xinc2;
    p = p + ys * next_scan + xs*2;

    // Draw the pixels
    while (numpixels-- >=0)
    {
         *p = color;
         if (d < 0)
         {
              d = d + dinc1;
              p = p + screeninc1;
         }
         else
         {
              d = d + dinc2;
              p = p + screeninc2;
         }
    }
}
//---------------------------------------------------------------------------

// Rutina de cerc - algoritmul bresenham
// inputs     : x,y - centrul cercului
//		radius - raza
// comentarii : nu e optimizat - cele 8 apeluri la putpixel
// void Circle(BYTE *p, long next_scan, int x, int y, int radius, WORD color)
void Circle(__int8 *p, long next_scan, int x, int y, int radius, unsigned __int16 color)
{
    int d, xi, yi;

    radius = abs(radius);
    if (radius == 0) return;
    if (radius == 1)
    {
        putPixel(p, next_scan, x, y, color);
        return;
    }

    xi = radius;
    yi = 0;
    d  = 3 - 2 * radius;

    while (xi >= yi)
    {
       putPixel(p, next_scan, x+xi, y+yi, color);
       putPixel(p, next_scan, x-xi, y+yi, color);
       putPixel(p, next_scan, x+yi, y+xi, color);
       putPixel(p, next_scan, x-yi, y+xi, color);

       putPixel(p, next_scan, x+xi, y-yi, color);
       putPixel(p, next_scan, x-xi, y-yi, color);
       putPixel(p, next_scan, x+yi, y-xi, color);
       putPixel(p, next_scan, x-yi, y-xi, color);
       if (d<0)
       {
           yi++;
           d = d + (yi << 2) + 2;
       }
       else
       {
           yi++;
           xi--;
           d = d - (xi << 2) + (yi << 2) + 2;
       }
    }
}
//---------------------------------------------------------------------------


// Descriere   : deseneaza un dreptunghi gamma corrected
// Param       : evidenti
// Rezultat    : evidenti
// Comentarii  : nici unul
//void darkRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD darkSide)
void darkRect(__int8 *p, long next_scan, int xstart, int ystart, int xend, int yend, unsigned __int16 darkSide)
{
  // clipping
  if (ystart > yend) SWAP(ystart, yend);
  if (xstart > xend) SWAP(xstart, xend);

  if (xstart < XLeft) xstart = XLeft;
  if (xend > XRight) xend = XRight;
  if (ystart < YTop) ystart = YTop;
  if (yend > YBot) yend = YBot;

  for(int y = ystart; y <= yend; y++)
  {
    darkHLine(p, next_scan, xstart, xend, y, darkSide);
  }  
}
//---------------------------------------------------------------------------

// Rutina de desenat un dreptunghi
// Inputs     : xs,ys - coordonate colt stinga-sus
//		xe,ye - coordonate colt dreapta-jos
//		color - culoare
// Comentarii : clippingul depinde de clipingul procedurilor de Line si HLine;
//void frameRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color)
void frameRect(__int8 *p, long next_scan, int xstart, int ystart, int xend, int yend, unsigned __int16 color)
{
    hLine(p, next_scan, xstart, xend, ystart, color);
    hLine(p, next_scan, xstart, xend, yend, color);
    vLine(p, next_scan, xstart, ystart, yend, color);
    vLine(p, next_scan, xend, ystart, yend, color);
}
//---------------------------------------------------------------------------

// Descriere   : deseneaza un dreptunghi plin
// Param       : evidenti
// Rezultat    : pe ecran
// Comentarii  : nimic
//void fillRect(BYTE *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, WORD color)
void fillRect(__int8 *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, unsigned __int16  color)
{
  // clipping
  if (yStart > yEnd) SWAP(yStart, yEnd);
  if (xStart > xEnd) SWAP(xStart, xEnd);

  if (xStart < XLeft) xStart = XLeft;
  if (xEnd > XRight) xEnd = XRight;
  if (yStart < YTop) yStart = YTop;
  if (yEnd > YBot) yEnd = YBot;

  for(int y = yStart; y <= yEnd; y++)
  {
    hLine(p, next_scan, xStart, xEnd, y, color);
  }  
}
//---------------------------------------------------------------------------

// Descriere   : deseneaza un fel de dreptunghi
// Param       : evidenti
// Rezultat    : pe ecran
// Comentarii  : nimic
// void selRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, WORD color)
void selRect(__int8 *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, unsigned __int16 color)
{
      hLine(p, next_scan, xstart, xstart+d, ystart, color);
      hLine(p, next_scan, xstart, xstart+d, yend, color);
      hLine(p, next_scan, xend, xend-d, ystart, color);
      hLine(p, next_scan, xend, xend-d, yend, color);

      vLine(p, next_scan, xstart, ystart, ystart+d, color);
      vLine(p, next_scan, xstart, yend, yend-d, color);
      vLine(p, next_scan, xend, ystart, ystart+d, color);
      vLine(p, next_scan, xend, yend, yend-d, color);
}
//---------------------------------------------------------------------------

// Descriere   : deseneaza dreptunghi fillat din pixel in pixel (un fel de tabla de sah)
// Param       : tot aia
// Rezultat    : on screen
// Comentarii  : optimizari ?
//void checkerRect(BYTE *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, WORD color)
void checkerRect(__int8 *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, unsigned __int16 color)
{
  int i;

  // ies daca dreptunghiul nu se incadreaza in ecran
  if (yStart > YBot)   return;
  if (yEnd   < YTop)   return;
  if (xStart > XRight) return;
  if (xEnd   < XLeft)  return;

  // clipping
  if (yStart > yEnd) SWAP(yStart, yEnd);
  if (xStart > xEnd) SWAP(xStart, xEnd);

  if (xStart < XLeft)  xStart = XLeft;
  if (xEnd   > XRight) xEnd   = XRight;
  if (yStart < YTop)   yStart = YTop;
  if (yEnd   > YBot)   yEnd   = YBot;

  for(i = yStart; i < yEnd; i++)
  {
    checkerHLine(p, next_scan, xStart + i % 2, xEnd, i, color);
  }
}
//---------------------------------------------------------------------------

