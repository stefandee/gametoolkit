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

class CKeyboard
{
   public:
      static CKeyboard*
               Instance();
      void     Release();

      void Update();

      bool IsPressed(unsigned __int8 dik);
      bool IsDown(unsigned __int8 dik);
      bool IsUp(unsigned __int8 dik);

      int GetFirstKeyPressed();
      int GetFirstKeyDown();
      int GetFirstKeyUp();

   protected:
      CKeyboard();
      ~CKeyboard();

   private:
      static CKeyboard* mInstance;

      int updateCounter;
      unsigned __int8 oldKeyz[256];

      void Clear();
};

typedef CKeyboard CPGIKeyboard;
typedef CKeyboard CPIIKeyboard;

CPGIKeyboard* GetKeyboardInstance();

//---------------------------------------------------------------------------
#endif
