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
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Key Convertor
 last modify: 04 01 01 by grabX
	      26 03 04 by Karg (added key codes)
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_KeyConvertorH
#define PP_KeyConvertorH

#include "PP_String.h"

// internal key codes, to be independent of the key codes the 
// input system provides (SDL/DX/whatever)
const int PPK_A = 1;
const int PPK_B = 2;
const int PPK_C = 3;
const int PPK_D = 4;
const int PPK_E = 5;
const int PPK_F = 6;
const int PPK_G = 7;
const int PPK_H = 8;
const int PPK_I = 9;
const int PPK_J = 10;
const int PPK_K = 11;
const int PPK_L = 12;
const int PPK_M = 13;
const int PPK_N = 14;
const int PPK_O = 15;
const int PPK_P = 16;
const int PPK_Q = 17;
const int PPK_R = 18;
const int PPK_S = 19;
const int PPK_T = 20;
const int PPK_U = 21;
const int PPK_V = 22;
const int PPK_X = 23;
const int PPK_Y = 24;
const int PPK_W = 25;
const int PPK_Z = 26;

const int PPK_LSHIFT = 27;
const int PPK_RSHIFT = 28;
const int PPK_LCTRL  = 29;
const int PPK_RCTRL  = 30;
const int PPK_LALT   = 31;
const int PPK_RALT   = 32;

const int PPK_F1     = 33;
const int PPK_F2     = 34;
const int PPK_F3     = 35;
const int PPK_F4     = 36;
const int PPK_F5     = 37;
const int PPK_F6     = 38;
const int PPK_F7     = 39;
const int PPK_F8     = 40;
const int PPK_F9     = 41;
const int PPK_F10    = 42;
const int PPK_F11    = 43;
const int PPK_F12    = 44;

const int PPK_1      = 45;
const int PPK_2      = 46;
const int PPK_3      = 47;
const int PPK_4      = 48;
const int PPK_5      = 49;
const int PPK_6      = 50;
const int PPK_7      = 51;
const int PPK_8      = 52;
const int PPK_9      = 53;
const int PPK_0      = 54;

const int PPK_ENTER  = 55;
const int PPK_BKSPACE= 56;
const int PPK_INS    = 57;
const int PPK_DEL    = 58;
const int PPK_PGUP   = 59;
const int PPK_PGDN   = 60;
const int PPK_HOME   = 61;
const int PPK_END    = 62;
const int PPK_TAB    = 63;
const int PPK_SPACE  = 64;

const int PPK_SLASH  = 65;
const int PPK_BKSLASH= 66;

const int PPK_DOT    = 67;
const int PPK_COMMA  = 68;
const int PPK_BKQUOTE= 69;
const int PPK_SEMICOLON = 70;
const int PPK_MINUS  = 71;
const int PPK_EQUAL  = 72;
const int PPK_LBRACKET  = 73;
const int PPK_RBRACKET  = 74;

const int PPK_ESCAPE = 75;
const int PPK_UP     = 76;
const int PPK_DOWN   = 77;
const int PPK_LEFT   = 78;
const int PPK_RIGHT  = 79;

// converts a read key to a pp key code
int KeyToPPK(int _key);

// converts a PPK to a system key (depending on the input system used)
// override this function to assure input system indepenency
int PPKToKey(int _key);

CPString ConvertKey(bool shift, int key);
/*class CPIIKeyConvertor
{
   public:
      static CPString Convert(bool shift, int key);

      static bool IsEscape(int key);
      static bool IsEnter(int key);
      static bool IsBackspace(int key);
      static bool IsTab(int key);

      static bool IsLeftArrow(int key);
      static bool IsRightArrow(int key);
      static bool IsUpArrow(int key);
      static bool IsDownArrow(int key);

      static bool IsPgUp(int key);
      static bool IsPgDown(int key);

      static bool IsHome(int key);
      static bool IsEnd(int key);

      static bool IsInsert(int key);
      static bool IsDelete(int key);

      static bool IsNumLock(int key);

      static bool IsNumPad0(int key);
      static bool IsNumPad1(int key);
      static bool IsNumPad2(int key);
      static bool IsNumPad3(int key);
      static bool IsNumPad4(int key);
      static bool IsNumPad5(int key);
      static bool IsNumPad6(int key);
      static bool IsNumPad7(int key);
      static bool IsNumPad8(int key);
      static bool IsNumPad9(int key);

      static bool IsLeftShift(int key);
      static bool IsRightShift(int key);
      static bool IsShift(int key);

      static bool IsLeftAlt(int key);
      static bool IsRightAlt(int key);
      static bool IsAlt(int key);

      static bool IsLeftCtrl(int key);
      static bool IsRightCtrl(int key);
      static bool IsCtrl(int key);

      static bool IsF1(int key);
      static bool IsF2(int key);
      static bool IsF3(int key);
      static bool IsF4(int key);
      static bool IsF5(int key);
      static bool IsF6(int key);
      static bool IsF7(int key);
      static bool IsF8(int key);
      static bool IsF9(int key);
      static bool IsF10(int key);
      static bool IsF11(int key);
      static bool IsF12(int key);

};
*/


//---------------------------------------------------------------------------
#endif
