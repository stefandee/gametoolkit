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
//  Unit       : inputs.h
//  Versiune   : 0.01
//  Descriere  : DirectInput - alt gunoi microsoftist, care trebuie
//               adaptat la necesitatzi...oh yeah :-)
//  Ultima
//  modificare : 19 ianuarie '99, 23:03
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: Singleton Pattern
              nu este chiar o clasa sanatoasa, da' ne-om descurca
 last modify: 16 04 00
        by grabX
---------------------------------------------------------------------------*/
#ifndef PP_InputH
#define PP_InputH
//---------------------------------------------------------------------------
#include <SDL.h>
#include <map>
#include "PP_Window.h"
#include "PP_String.h"
#include "basic2d.h"

class CPIIInputSystem
{
   public:
      class CErrorGeneric {};
   public:
      static CPIIInputSystem* Instance(CPWindow* hWnd = NULL);
      static void             Release();

      void RestoreDevices(void);
      void ReleaseDevices(void);
      void UpdateKeys(void);
      void UpdateMouse(void);

      void SetMouseAt(int x, int y);

      CPPoint Mouse; /**< mouse coordinate delta */
      bool MouseLB, MouseMB, MouseRB;
      std::map<int,int> Keyz;

   protected:
      CPIIInputSystem(CPWindow* hWnd);
      ~CPIIInputSystem();
      bool InitInputs(CPWindow* hWnd);
   private:
      static CPIIInputSystem *mInstance;
      static int              mInstanceCount;

      bool MouseInitialized;
      CPWindow* mWindow;
};

CPIIInputSystem* GetInputInstance(CPWindow* hWnd = NULL);
//pentru eliminarea warningurilor
#endif
