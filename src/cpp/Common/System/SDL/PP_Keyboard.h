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

 description: keyboard + metode de keypresed si keydown si keyup
 last modify: 17 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef KeybH
#define KeybH

#include <map>

class CKeyboard
{
   public:
      static CKeyboard*
               Instance();
      static void     Release();

      void Update();

      bool IsPressed(unsigned char dik);
      bool IsDown(unsigned char dik);
      bool IsUp(unsigned char dik);

      int GetFirstKeyPressed(bool noModifier = false);
      int GetFirstKeyDown();
      int GetFirstKeyUp();

   protected:
      CKeyboard();
      ~CKeyboard();

   private:
      static CKeyboard* mInstance;

      int updateCounter;
      std::map<int,int> oldKeyz;

      void Clear();

      bool IsModifierKey(int key);
};

typedef CKeyboard CPGIKeyboard;
typedef CKeyboard CPIIKeyboard;

CPGIKeyboard* GetKeyboardInstance();

//---------------------------------------------------------------------------
#endif
