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
//#include <vcl.h>
#include <dinput.h>
#pragma hdrstop

#include "PP_KeyConvertor.h"

int KeyToPPK(int _key)
{
  switch(_key)
  {
    case DIK_A:
      return PPK_A;

    case DIK_B:
      return PPK_B;

    case DIK_C:
      return PPK_C;

    case DIK_D:
      return PPK_D;

    case DIK_E:
      return PPK_E;

    case DIK_F:
      return PPK_F;

    case DIK_G:
      return PPK_G;

    case DIK_H:
      return PPK_H;

    case DIK_I:
      return PPK_I;

    case DIK_J:
      return PPK_J;

    case DIK_K:
      return PPK_K;

    case DIK_L:
      return PPK_L;

    case DIK_M:
      return PPK_M;

    case DIK_N:
      return PPK_N;

    case DIK_O:
      return PPK_O;

    case DIK_P:
      return PPK_P;

    case DIK_R:
      return PPK_R;

    case DIK_S:
      return PPK_S;

    case DIK_T:
      return PPK_T;

    case DIK_U:
      return PPK_U;

    case DIK_V:
      return PPK_V;

    case DIK_W:
      return PPK_W;

    case DIK_X:
      return PPK_X;

    case DIK_Y:
      return PPK_Y;

    case DIK_Z:
      return PPK_Z;

    case DIK_F1:
      return PPK_F1;

    case DIK_F2:
      return PPK_F2;

    case DIK_F3:
      return PPK_F3;

    case DIK_F4:
      return PPK_F4;

    case DIK_F5:
      return PPK_F5;

    case DIK_F6:
      return PPK_F6;

    case DIK_F7:
      return PPK_F7;

    case DIK_F8:
      return PPK_F8;

    case DIK_F9:
      return PPK_F9;

    case DIK_F10:
      return PPK_F10;

    case DIK_F11:
      return PPK_F11;

    case DIK_F12:
      return PPK_F12;

    case DIK_1:
      return PPK_1;

    case DIK_2:
      return PPK_2;

    case DIK_3:
      return PPK_3;

    case DIK_4:
      return PPK_4;

    case DIK_5:
      return PPK_5;

    case DIK_6:
      return PPK_6;

    case DIK_7:
      return PPK_7;

    case DIK_8:
      return PPK_8;

    case DIK_9:
      return PPK_9;

    case DIK_0:
      return PPK_0;

    case DIK_ESCAPE:
      return PPK_ESCAPE;

    case DIK_MINUS:
      return PPK_MINUS;

    case DIK_EQUALS:
      return PPK_EQUAL;

    case DIK_BACK:
      return PPK_BKSPACE;

    case DIK_TAB:
      return PPK_TAB;

    case DIK_LBRACKET:
      return PPK_LBRACKET;

    case DIK_RBRACKET:
      return PPK_RBRACKET;

    case DIK_RETURN:
      return PPK_ENTER;

    case DIK_LCONTROL:
      return PPK_LCTRL;

    case DIK_RCONTROL:
      return PPK_RCTRL;

    case DIK_SEMICOLON:
      return PPK_SEMICOLON;

    case DIK_LSHIFT:
      return PPK_LSHIFT;

    case DIK_RSHIFT:
      return PPK_RSHIFT;

    case DIK_BACKSLASH:
      return PPK_BKSLASH;

    case DIK_COMMA:
      return PPK_COMMA;

    case DIK_SLASH:
      return PPK_SLASH;

    case DIK_SPACE:
      return PPK_SPACE;

    case DIK_UP:
      return PPK_UP;

    case DIK_DOWN:
      return PPK_DOWN;

    case DIK_LEFT:
      return PPK_LEFT;

    case DIK_RIGHT:
      return PPK_RIGHT;

    case DIK_HOME:
      return PPK_HOME;

    case DIK_END:
      return PPK_END;

    case DIK_INSERT:
      return PPK_INS;

    case DIK_DELETE:
      return PPK_DEL;

    case DIK_LALT:
      return PPK_LALT;

    case DIK_RALT:
      return PPK_RALT;

    case DIK_PGUP:
      return PPK_PGUP;

    case DIK_PGDN:
      return PPK_PGDN;

    /*
    case DIK_:
      return PPK_;

    */
  }

  return -1;
}
//---------------------------------------------------------------------------

int PPKToKey(int _key)
{
  switch(_key)
  {
    case PPK_A:
      return DIK_A;

    case PPK_B:
      return DIK_B;

    case PPK_C:
      return DIK_C;

    case PPK_D:
      return DIK_D;

    case PPK_E:
      return DIK_E;

    case PPK_F:
      return DIK_F;

    case PPK_G:
      return DIK_G;

    case PPK_H:
      return DIK_H;

    case PPK_I:
      return DIK_I;

    case PPK_J:
      return DIK_J;

    case PPK_K:
      return DIK_K;

    case PPK_L:
      return DIK_L;

    case PPK_M:
      return DIK_M;

    case PPK_N:
      return DIK_N;

    case PPK_O:
      return DIK_O;

    case PPK_P:
      return DIK_P;

    case PPK_R:
      return DIK_R;

    case PPK_S:
      return DIK_S;

    case PPK_T:
      return DIK_T;

    case PPK_U:
      return DIK_U;

    case PPK_V:
      return DIK_V;

    case PPK_W:
      return DIK_W;

    case PPK_X:
      return DIK_X;

    case PPK_Y:
      return DIK_Y;

    case PPK_Z:
      return DIK_Z;

    case PPK_F1:
      return DIK_F1;

    case PPK_F2:
      return DIK_F2;

    case PPK_F3:
      return DIK_F3;

    case PPK_F4:
      return DIK_F4;

    case PPK_F5:
      return DIK_F5;

    case PPK_F6:
      return DIK_F6;

    case PPK_F7:
      return DIK_F7;

    case PPK_F8:
      return DIK_F8;

    case PPK_F9:
      return DIK_F9;

    case PPK_F10:
      return DIK_F10;

    case PPK_F11:
      return DIK_F11;

    case PPK_F12:
      return DIK_F12;

    case PPK_1:
      return DIK_1;

    case PPK_2:
      return DIK_2;

    case PPK_3:
      return DIK_3;

    case PPK_4:
      return DIK_4;

    case PPK_5:
      return DIK_5;

    case PPK_6:
      return DIK_6;

    case PPK_7:
      return DIK_7;

    case PPK_8:
      return DIK_8;

    case PPK_9:
      return DIK_9;

    case PPK_0:
      return DIK_0;

    case PPK_ESCAPE:
      return DIK_ESCAPE;

    case PPK_MINUS:
      return DIK_MINUS;

    case PPK_EQUAL:
      return DIK_EQUALS;

    case PPK_BKSPACE:
      return DIK_BACK;

    case PPK_TAB:
      return DIK_TAB;

    case PPK_LBRACKET:
      return DIK_LBRACKET;

    case PPK_RBRACKET:
      return DIK_RBRACKET;

    case PPK_ENTER:
      return DIK_RETURN;

    case PPK_LCTRL:
      return DIK_LCONTROL;

    case PPK_RCTRL:
      return DIK_RCONTROL;

    case PPK_SEMICOLON:
      return DIK_SEMICOLON;

    case PPK_LSHIFT:
      return DIK_LSHIFT;

    case PPK_RSHIFT:
      return DIK_RSHIFT;

    case PPK_BKSLASH:
      return DIK_BACKSLASH;

    case PPK_COMMA:
      return DIK_COMMA;

    case PPK_SLASH:
      return DIK_SLASH;

    case PPK_SPACE:
      return DIK_SPACE;

    case PPK_UP:
      return DIK_UP;

    case PPK_DOWN:
      return DIK_DOWN;

    case PPK_LEFT:
      return DIK_LEFT;

    case PPK_RIGHT:
      return DIK_RIGHT;

    case PPK_HOME:
      return DIK_HOME;

    case PPK_END:
      return DIK_END;

    case PPK_INS :
      return DIK_INSERT;

    case PPK_DEL :
      return DIK_DELETE;

    case PPK_LALT:
      return DIK_LALT;

    case PPK_RALT:
      return DIK_RALT;

    case PPK_PGUP:
      return DIK_PGUP;

    case PPK_PGDN:
      return DIK_PGDN;

    /*
    case PPK_:
      return DIK_;

    */
  }

  return -1;
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
