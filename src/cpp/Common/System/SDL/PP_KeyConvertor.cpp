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

#include "SDL.h"
#include "PP_KeyConvertor.h"
#include "easylogging++.h"

int PPKToKey(int _key)
{
  switch(_key)
  {
    case PPK_A:
      return SDLK_a;

    case PPK_B:
      return SDLK_b;

    case PPK_C:
      return SDLK_c;

    case PPK_D:
      return SDLK_d;

    case PPK_E:
      return SDLK_e;

    case PPK_F:
      return SDLK_f;

    case PPK_G:
      return SDLK_g;

    case PPK_H:
      return SDLK_h;

    case PPK_I:
      return SDLK_i;

    case PPK_J:
      return SDLK_j;

    case PPK_K:
      return SDLK_k;

    case PPK_L:
      return SDLK_l;

    case PPK_M:
      return SDLK_m;

    case PPK_N:
      return SDLK_n;

    case PPK_O:
      return SDLK_o;

    case PPK_P:
      return SDLK_p;

    case PPK_Q:
      return SDLK_q;

    case PPK_R:
      return SDLK_r;

    case PPK_S:
      return SDLK_s;

    case PPK_T:
      return SDLK_t;

    case PPK_U:
      return SDLK_u;

    case PPK_V:
      return SDLK_v;

    case PPK_W:
      return SDLK_w;

    case PPK_X:
      return SDLK_x;

    case PPK_Y:
      return SDLK_y;

    case PPK_Z:
      return SDLK_z;

    case PPK_LALT:
      return SDLK_LALT;

    case PPK_RALT:
      return SDLK_RALT;

    case PPK_LSHIFT:
      return SDLK_LSHIFT;

    case PPK_RSHIFT:
      return SDLK_RSHIFT;

    case PPK_LCTRL:
      return SDLK_LCTRL;

    case PPK_RCTRL:
      return SDLK_RCTRL;

    case PPK_F1:
      return SDLK_F1;

    case PPK_F2:
      return SDLK_F2;

    case PPK_F3:
      return SDLK_F3;

    case PPK_F4:
      return SDLK_F4;

    case PPK_F5:
      return SDLK_F5;

    case PPK_F6:
      return SDLK_F6;

    case PPK_F7:
      return SDLK_F7;

    case PPK_F8:
      return SDLK_F8;

    case PPK_F9:
      return SDLK_F9;

    case PPK_F10:
      return SDLK_F10;

    case PPK_F11:
      return SDLK_F11;

    case PPK_F12:
      return SDLK_F12;

    case PPK_1:
      return SDLK_1;

    case PPK_2:
      return SDLK_2;

    case PPK_3:
      return SDLK_3;

    case PPK_4:
      return SDLK_4;

    case PPK_5:
      return SDLK_5;

    case PPK_6:
      return SDLK_6;

    case PPK_7:
      return SDLK_7;

    case PPK_8:
      return SDLK_8;

    case PPK_9:
      return SDLK_9;

    case PPK_0:
      return SDLK_0;

    case PPK_ENTER:
      return SDLK_RETURN;

    case PPK_BKSPACE:
      return SDLK_BACKSPACE;

    case PPK_INS:
      return SDLK_INSERT;

    case PPK_DEL:
      return SDLK_DELETE;

    case PPK_PGUP:
      return SDLK_PAGEUP;

    case PPK_PGDN:
      return SDLK_PAGEDOWN;

    case PPK_HOME:
      return SDLK_HOME;

    case PPK_END:
      return SDLK_END;

    case PPK_TAB:
      return SDLK_TAB;

    case PPK_SPACE:
      return SDLK_SPACE;

    case PPK_SLASH:
      return SDLK_SLASH;

    case PPK_BKSLASH:
      return SDLK_BACKSLASH;

    case PPK_DOT:
      return SDLK_PERIOD;

    case PPK_COMMA:
      return SDLK_COMMA;

    case PPK_BKQUOTE:
      return SDLK_BACKQUOTE;

    case PPK_SEMICOLON:
      return SDLK_SEMICOLON;

    case PPK_MINUS:
      return SDLK_MINUS;

    case PPK_EQUAL:
      return SDLK_EQUALS;

    case PPK_LBRACKET:
      return SDLK_LEFTBRACKET;

    case PPK_RBRACKET:
      return SDLK_RIGHTBRACKET;

    case PPK_ESCAPE:
      return SDLK_ESCAPE;

    case PPK_UP:
      return SDLK_UP;

    case PPK_DOWN:
      return SDLK_DOWN;

    case PPK_LEFT:
      return SDLK_LEFT;

    case PPK_RIGHT:
      return SDLK_RIGHT;

    default:
      return SDLK_UNKNOWN;
  }

  //return SDLK_UNKNOWN;
}
//---------------------------------------------------------------------------

CPString ConvertKey(bool shift, int key)
{
   if (key != -1)
   {
      VLOG(9) << "ConvertKey: " + key;
   }

   switch(key)
   {
      case PPK_1:
        if (shift == true)
            return CPString("!");
        else
            return CPString("1");

      case PPK_2: if (shift == true)
                  return CPString("@");
               else
                  return CPString("2");

      case PPK_3: if (shift == true)
                  return CPString("#");
               else
                  return CPString("3");

      case PPK_4: if (shift == true)
                  return CPString("$");
               else
                  return CPString("4");

      case PPK_5: if (shift == true)
                  return CPString("%");
               else
                  return CPString("5");

      case PPK_6: if (shift == true)
                  return CPString("^");
               else
                  return CPString("6");

      case PPK_7: if (shift == true)
                  return CPString("&");
               else
                  return CPString("7");

      case PPK_8: if (shift == true)
                  return CPString("*");
               else
                  return CPString("8");

      case PPK_9: if (shift == true)
                  return CPString("(");
               else
                  return CPString("9");

      case PPK_0: if (shift == true)
                  return CPString(")");
               else
                  return CPString("0");

      case PPK_MINUS: if (shift == true)
                  return CPString("_");
               else
                  return CPString("-");

      case PPK_Q: if (shift == true)
                  return CPString("Q");
               else
                  return CPString("q");
      case PPK_W: if (shift == true)
                  return CPString("W");
               else
                  return CPString("w");
      case PPK_E: if (shift == true)
                  return CPString("E");
               else
                  return CPString("e");
      case PPK_R: if (shift == true)
                  return CPString("R");
               else
                  return CPString("r");
      case PPK_T: if (shift == true)
                  return CPString("T");
               else
                  return CPString("t");
      case PPK_Y: if (shift == true)
                  return CPString("Y");
               else
                  return CPString("y");
      case PPK_U: if (shift == true)
                  return CPString("U");
               else
                  return CPString("u");
      case PPK_I: if (shift == true)
                  return CPString("I");
               else
                  return CPString("i");
      case PPK_O: if (shift == true)
                  return CPString("O");
               else
                  return CPString("o");
      case PPK_P: if (shift == true)
                  return CPString("P");
               else
                  return CPString("p");
      case PPK_LBRACKET: if (shift == true)
                  return CPString("{");
               else
                  return CPString("[");
      case PPK_RBRACKET: if (shift == true)
                  return CPString("}");
               else
                  return CPString("]");

      case PPK_A: if (shift == true)
                  return CPString("A");
               else
                  return CPString("a");
      case PPK_S: if (shift == true)
                  return CPString("S");
               else
                  return CPString("s");
      case PPK_D: if (shift == true)
                  return CPString("D");
               else
                  return CPString("d");
      case PPK_F: if (shift == true)
                  return CPString("F");
               else
                  return CPString("f");
      case PPK_G: if (shift == true)
                  return CPString("G");
               else
                  return CPString("g");
      case PPK_H: if (shift == true)
                  return CPString("H");
               else
                  return CPString("h");
      case PPK_J: if (shift == true)
                  return CPString("J");
               else
                  return CPString("j");
      case PPK_K: if (shift == true)
                  return CPString("K");
               else
                  return CPString("k");
      case PPK_L: if (shift == true)
                  return CPString("L");
               else
                  return CPString("l");
      case PPK_SEMICOLON: if (shift == true)
                  return CPString(":");
               else
                  return CPString(";");
      case PPK_BKSLASH: if (shift == true)
                  return CPString("\"");
               else
                  return CPString("'");

      case PPK_Z: if (shift == true)
                  return CPString("Z");
               else
                  return CPString("z");
      case PPK_X: if (shift == true)
                  return CPString("X");
               else
                  return CPString("x");
      case PPK_C: if (shift == true)
                  return CPString("C");
               else
                  return CPString("c");
      case PPK_V: if (shift == true)
                  return CPString("V");
               else
                  return CPString("v");
      case PPK_B: if (shift == true)
                  return CPString("B");
               else
                  return CPString("b");
      case PPK_N: if (shift == true)
                  return CPString("N");
               else
                  return CPString("n");
      case PPK_M: if (shift == true)
                  return CPString("M");
               else
                  return CPString("m");
      case PPK_COMMA: if (shift == true)
                  return CPString("<");
               else
                  return CPString(",");
      case PPK_DOT: if (shift == true)
                  return CPString(">");
               else
                  return CPString(".");
      case PPK_SLASH: if (shift == true)
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
