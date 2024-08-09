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
//  Unit       : inputs.cpp
//  Descriere  : implementerea claselor din inputs.h
//               de fapt, copy & paste cu nesimtzire din CDX
//               poate ar trebui sa le dam ceva ban pentru asta :-))))
//  Ultima
//  modificare : 19 ianuarie '99, 23:02
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: Singleton Pattern
              nu este chiar o clasa sanatoasa, da' ne-om descurca
 last modify: 16 04 00
        by grabX
---------------------------------------------------------------------------*/
#pragma hdrstop

#include <memory.h>

#include "PP_Input.h"

//---------------------------------------------------------------------------
//#pragma package(smart_init)

CPIIInputSystem *CPIIInputSystem::mInstance = NULL;
int              CPIIInputSystem::mInstanceCount = 0;


/*---------------------------------------------------------------------------
 description: returneaza instanta de input system
 others     : a aparut din cauza warningurilor
---------------------------------------------------------------------------*/
CPIIInputSystem* GetInputInstance(CPWindow* hWnd)
{
  return CPIIInputSystem::Instance(hWnd);
}


/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza singura instanta posibila
 parameters : necesari pentru constructor
---------------------------------------------------------------------------*/
CPIIInputSystem* CPIIInputSystem::Instance(CPWindow* hWnd)
{
   if (mInstance == NULL)
   {
      mInstance = new CPIIInputSystem(hWnd);
   }
   return mInstance;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: face release daca este alocat
 others     : mai demult decrementa mInstanceCount si daca == 0 => delete
---------------------------------------------------------------------------*/
void CPIIInputSystem::Release()
{
   if (mInstance != NULL)
   {
      delete mInstance;
      mInstance = NULL;
   }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : pretty obvious; in fiecare tick de ceas se apeleaza aceasta
//              metoda; exista insa un drawback :  in momentul cind metoda e
//              apelata, userul trebuie sa aiba deshtu' pe tasta (pentru ca nu
//              e bufferuita); dar, intrucit timerul se apeleaza de 30+ ori pe
//              secunda, e foarte putin probabil sa scapam ceva din dorintza
//              userului
//  probleme  : buffering (GetDeviceData) de incercat in caz de comportamente
//              ciudate
//---------------------------------------------------------------------------
void CPIIInputSystem::UpdateKeys(void)
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_KEYDOWN:
        Keyz[event.key.keysym.sym] = 0x80;
        //logWrite("Key code: "); logWriteILn((int)event.key.keysym.sym);
        //logWrite("Wanted: "); logWriteILn(SDLK_a);
        break;

      case SDL_KEYUP:
        Keyz[event.key.keysym.sym] = 0;
        break;
     }
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : ca mai sus numai ca pentru mouse
//  probleme  : ca mai sus numai ca pentru mouse
//---------------------------------------------------------------------------
void CPIIInputSystem::UpdateMouse(void)
{
  Uint8 buttons;

  if (MouseInitialized)
  {
      buttons = SDL_GetRelativeMouseState(&Mouse.x, &Mouse.y);
  }
  else
  {
      buttons = SDL_GetRelativeMouseState(NULL, NULL);
      MouseInitialized = true;
  }

  MouseLB = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT  )) != 0;
  MouseRB = (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT )) != 0;
  MouseMB = (buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : SDL does not need device restoration
//---------------------------------------------------------------------------
void CPIIInputSystem::RestoreDevices(void)
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : SDL does not need releasing the devices
//---------------------------------------------------------------------------
void CPIIInputSystem::ReleaseDevices(void)
{
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
CPIIInputSystem::CPIIInputSystem(CPWindow* hWnd)
{
  InitInputs(hWnd);

  MouseInitialized = false;

  Mouse = CPPoint(0,0);

  MouseLB = false;
  MouseRB = false;
  MouseMB = false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : gata cu DirectInputu'; un pic modificata fata de CDX
//---------------------------------------------------------------------------
CPIIInputSystem::~CPIIInputSystem()
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//  descriere : mda...pune in functziune achizitia de date de la input devices
//              e suficient de explicita
//              de ce se numeste PowerOn ? buna intrebare
//  probleme  : netestata
//---------------------------------------------------------------------------
bool CPIIInputSystem::InitInputs(CPWindow* hWnd)
{
   mWindow = hWnd;

   return true;
}
//---------------------------------------------------------------------------

void CPIIInputSystem::SetMouseAt(int x, int y)
{
    // move the mouse in the middle of the screen
    SDL_WarpMouseInWindow(mWindow->impl, x, y);
}
//---------------------------------------------------------------------------
