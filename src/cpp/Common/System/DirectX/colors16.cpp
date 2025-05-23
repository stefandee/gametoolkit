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
//#include <vcl.h>
#pragma hdrstop

#include "colors16.h"
#include "BitOp.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

TColorFormat ApplicationBpp;

typedef struct
{
  unsigned __int16 green[64];
} _green_values;

//-----------------------------------------------------------------------------
// Pixel conversion stuff
//-----------------------------------------------------------------------------
static unsigned __int32 red_mask, green_mask, blue_mask;
static unsigned __int8 red_used_bits, green_used_bits, blue_used_bits;
static unsigned __int8 red_free_bits, green_free_bits, blue_free_bits;

static unsigned __int8 redComp[65536];
static unsigned __int8 greenComp[65536];
static unsigned __int8 blueComp[65536];

_green_values compTable[32];

// Descriere   :  initializeaza variabile interne de culori, biti, etc
// Param       :  mastile de culori
// Rezultat    :  nimic
// Comentarii  :  nimic
void InitColor(int redBitMask, int greenBitMask, int blueBitMask)
{
   int i;
  // initializez variabilele
  red_mask   = redBitMask;
  green_mask = greenBitMask;
  blue_mask  = blueBitMask;

  red_used_bits   = CountBits32(red_mask);
  green_used_bits = CountBits32(green_mask);
  blue_used_bits  = CountBits32(blue_mask);

  red_free_bits = 8 - red_used_bits;
  green_free_bits = 8 - green_used_bits;
  blue_free_bits = 8 - blue_used_bits;

  if (green_used_bits == 6)
  {
    ApplicationBpp = RGB_565;
  }
  else
  {
    ApplicationBpp = RGB_555;
  }  

  // initializez vectorii de descompunere ai culorii
  for(i = 0; i < 65536; i++)
  {
    redComp[i]   = (unsigned __int8)((i & red_mask) >> (green_used_bits + blue_used_bits));
    greenComp[i] = (unsigned __int8)((i & green_mask) >> blue_used_bits);
    blueComp[i]  = (unsigned __int8)(i & blue_mask);
  }

  // initializez hash-table pentru compunerea culorii
  // ---> cod
  for(i = 0; i < 32; i++)
  {
    for(int j = 0; j < 64; j++)
    {
      compTable[i].green[j] = (i << (green_used_bits + blue_used_bits)) | (j << blue_used_bits);
    }
  }
}
// ----------------------------------------------------------------------------

// Descriere   :  combina 2 culori pe 16 biti intr'un raport dat
// Param       :  a, b    - culorile
//                procent - procentul
// Rezultat    :  culoarea combinata
// Comentarii  :  optimizari ?
unsigned __int16  combineColor(unsigned __int32 a, unsigned __int32 b, unsigned __int32 procent)
{
  /*
  return (((a & red_mask) * procent + (b & red_mask) * (16 - procent)) >> 4) |
         (((a & green_mask)* procent + (b & green_mask) * (16 - procent)) >> 4) |
         (((a & blue_mask) * procent + (b & blue_mask) * (16 - procent)) >> 4);
  */
  /*
  return (a & red_mask) |
         (a & green_mask) |
         (((a & blue_mask) * procent + (b & blue_mask) * (16 - procent)) >> 4);
  */

  unsigned __int32 red_a = (a & red_mask) >> (green_used_bits + blue_used_bits);
  unsigned __int32 red_b = (b & red_mask) >> (green_used_bits + blue_used_bits);
  unsigned __int32 green_a = (a & green_mask) >> blue_used_bits;
  unsigned __int32 green_b = (b & green_mask) >> blue_used_bits;

  // formula lu' peshte prajit - dont try this at home
  return (unsigned __int16)(
	     (((red_a * procent + (16 - procent) * red_b) >> 4) << (green_used_bits + blue_used_bits)) |
         (((green_a * procent + (16 - procent) * green_b) >> 4) << (blue_used_bits)) |
         (((a & blue_mask) * procent + (b & blue_mask) * (16 - procent)) >> 4)
		 );
}
// ----------------------------------------------------------------------------

// Descriere   :  intuneca o culoare
// Param       :  a        - culoarea
//                darkSide - cit sa se scada
// Rezultat    :  culoarea corectata
// Comentarii  :  optimizari ?
unsigned __int16 darkColor(unsigned __int32 a, unsigned __int32 darkSide)
{
  /*
  DWORD red_a = (a & red_mask) >> (green_used_bits + blue_used_bits);
  DWORD green_a = (a & green_mask) >> blue_used_bits;
  DWORD blue_a = (a & blue_mask);
  */
  __int8 red_a   = redComp[a];
  __int8 green_a = greenComp[a];
  __int8 blue_a  = blueComp[a];

  if ((unsigned)red_a > darkSide)
  {
    red_a -= (unsigned __int8)darkSide;
  }
  else
  {
    red_a = 0;
  }

  if ((unsigned)green_a > darkSide) green_a -= (unsigned __int8)darkSide;
  else green_a = 0;

  if ((unsigned)blue_a > darkSide) blue_a -= (unsigned __int8)darkSide;
  else blue_a = 0;

  // formulitza lu' peshte prajit
  /*
  return (red_a << (green_used_bits + blue_used_bits)) |
         (green_a << blue_used_bits) |
         blue_a;
  */
  return compTable[red_a].green[green_a] | blue_a;
}

unsigned __int16 makeRGB16(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
{
  /*
  WORD toRet;
  asm
  {
    // red component
    mov cl, green_used_bits

    xor eax, eax
    mov ah, r
    shl ax, cl
    mov cl, blue_used_bits
    shl ax, cl
    and eax, red_mask

    // green component
    xor ebx, ebx
    mov bl, g
    shl bx, cl
    and ebx, green_mask

    // blue component
    xor ecx, ecx
    mov cl, b
    and ecx, blue_mask

    or ax, bx
    or ax, cx

    mov toRet, ax
  }

  return toRet;
  */

  return (unsigned __int16)(((b >> blue_free_bits) & blue_mask) |
         (((g >> green_free_bits) << blue_used_bits) & green_mask) |
         (((r >> red_free_bits) << (green_used_bits + blue_used_bits)) & red_mask));
}


