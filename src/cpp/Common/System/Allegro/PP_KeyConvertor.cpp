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
 last modify: 04 01 01
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_KeyConvertor.h"
#include "allegro.h"

int PPKToKey(int _key)
{
  switch(_key)
  {
    case PPK_A:
      return -1;

    case PPK_B:
      return -1;

    case PPK_C:
      return -1;

    case PPK_D:
      return -1;

    case PPK_E:
      return -1;

    case PPK_F:
      return -1;

    case PPK_G:
      return -1;

    case PPK_I:
      return -1;

    case PPK_K:
      return -1;

    case PPK_L:
      return -1;

    case PPK_M:
      return -1;

    case PPK_O:
      return -1;

    case PPK_Q:
      return -1;

    case PPK_R:
      return -1;

    case PPK_S:
      return -1;

    case PPK_T:
      return -1;

    case PPK_U:
      return -1;

    case PPK_V:
      return -1;

    case PPK_W:
      return -1;

    case PPK_X:
      return KEY_X;

    case PPK_Y:
      return -1;

    case PPK_Z:
      return -1;

    case PPK_LALT:
      return KEY_ALT;

    case PPK_RALT:
      return KEY_ALTGR;

    case PPK_LSHIFT:
      return -1;

    case PPK_RSHIFT:
      return -1;

    case PPK_LCTRL:
      return -1;

    case PPK_RCTRL:
      return -1;

    case PPK_F1:
      return -1;

    case PPK_F2:
      return -1;

    case PPK_F3:
      return -1;

    case PPK_F4:
      return -1;

    case PPK_F5:
      return -1;

    case PPK_F6:
      return -1;

    case PPK_F7:
      return -1;

    case PPK_F8:
      return -1;

    case PPK_F9:
      return -1;

    case PPK_F10:
      return -1;

    case PPK_F11:
      return -1;

    case PPK_F12:
      return -1;

    case PPK_1:
      return -1;

    case PPK_2:
      return -1;

    case PPK_3:
      return -1;

    case PPK_4:
      return -1;

    case PPK_5:
      return -1;

    case PPK_6:
      return -1;

    case PPK_7:
      return -1;

    case PPK_8:
      return -1;

    case PPK_9:
      return -1;

    case PPK_0:
      return -1;

    case PPK_ENTER:
      return -1;

    case PPK_BKSPACE:
      return -1;

    case PPK_INS:
      return -1;

    case PPK_DEL:
      return -1;

    case PPK_PGUP:
      return -1;

    case PPK_PGDN:
      return -1;

    case PPK_HOME:
      return -1;

    case PPK_END:
      return -1;

    case PPK_TAB:
      return -1;

    case PPK_SPACE:
      return -1;

    case PPK_SLASH:
      return -1;

    case PPK_BKSLASH:
      return -1;

    case PPK_DOT:
      return -1;

    case PPK_COMMA:
      return -1;

    case PPK_BKQUOTE:
      return -1;

    case PPK_SEMICOLON:
      return -1;

    case PPK_MINUS:
      return -1;

    case PPK_EQUAL:
      return -1;

    case PPK_LBRACKET:
      return -1;

    case PPK_RBRACKET:
      return -1;

    case PPK_ESCAPE:
      return -1;

    case PPK_UP:
      return KEY_UP;

    case PPK_DOWN:
      return KEY_DOWN;

    case PPK_LEFT:
      return KEY_LEFT;

    case PPK_RIGHT:
      return KEY_RIGHT;

    default:
      return -1;
  }

  //return SDLK_UNKNOWN;
}
//---------------------------------------------------------------------------

CPString ConvertKey(bool shift, int key)
{
   switch(key)
   {
      case 16: if (shift == true)
                  return CPString("Q");
               else
                  return CPString("q");
      case 17: if (shift == true)
                  return CPString("W");
               else
                  return CPString("w");
      case 18: if (shift == true)
                  return CPString("E");
               else
                  return CPString("e");
      case 19: if (shift == true)
                  return CPString("R");
               else
                  return CPString("r");
      case 20: if (shift == true)
                  return CPString("T");
               else
                  return CPString("t");
      case 21: if (shift == true)
                  return CPString("Y");
               else
                  return CPString("y");
      case 22: if (shift == true)
                  return CPString("U");
               else
                  return CPString("u");
      case 23: if (shift == true)
                  return CPString("I");
               else
                  return CPString("i");
      case 24: if (shift == true)
                  return CPString("O");
               else
                  return CPString("o");
      case 25: if (shift == true)
                  return CPString("P");
               else
                  return CPString("p");
      case 26: if (shift == true)
                  return CPString("{");
               else
                  return CPString("[");
      case 27: if (shift == true)
                  return CPString("}");
               else
                  return CPString("]");

      case 30: if (shift == true)
                  return CPString("A");
               else
                  return CPString("a");
      case 31: if (shift == true)
                  return CPString("S");
               else
                  return CPString("s");
      case 32: if (shift == true)
                  return CPString("D");
               else
                  return CPString("d");
      case 33: if (shift == true)
                  return CPString("F");
               else
                  return CPString("f");
      case 34: if (shift == true)
                  return CPString("G");
               else
                  return CPString("g");
      case 35: if (shift == true)
                  return CPString("H");
               else
                  return CPString("h");
      case 36: if (shift == true)
                  return CPString("J");
               else
                  return CPString("j");
      case 37: if (shift == true)
                  return CPString("K");
               else
                  return CPString("k");
      case 38: if (shift == true)
                  return CPString("L");
               else
                  return CPString("l");
      case 39: if (shift == true)
                  return CPString(":");
               else
                  return CPString(";");
      case 40: if (shift == true)
                  return CPString("\"");
               else
                  return CPString("'");

      case 44: if (shift == true)
                  return CPString("Z");
               else
                  return CPString("z");
      case 45: if (shift == true)
                  return CPString("X");
               else
                  return CPString("x");
      case 46: if (shift == true)
                  return CPString("C");
               else
                  return CPString("c");
      case 47: if (shift == true)
                  return CPString("V");
               else
                  return CPString("v");
      case 48: if (shift == true)
                  return CPString("B");
               else
                  return CPString("b");
      case 49: if (shift == true)
                  return CPString("N");
               else
                  return CPString("n");
      case 50: if (shift == true)
                  return CPString("M");
               else
                  return CPString("m");
      case 51: if (shift == true)
                  return CPString("<");
               else
                  return CPString(",");
      case 52: if (shift == true)
                  return CPString(">");
               else
                  return CPString(".");
      case 53: if (shift == true)
                  return CPString("?");
               else
                  return CPString("/");

      case 57: return CPString(" ");
   }
   return CPString("");
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
