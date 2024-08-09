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
#include "logfile.h"

//---------------------------------------------------------------------------
//#pragma package(smart_init)

CPIIInputSystem *CPIIInputSystem::mInstance = NULL;
int              CPIIInputSystem::mInstanceCount = 0;


/*---------------------------------------------------------------------------
 description: returneaza instanta de input system
 others     : a aparut din cauza warningurilor
---------------------------------------------------------------------------*/
CPIIInputSystem* GetInputInstance(void *hInst, void *hWnd)
{
  return CPIIInputSystem::Instance(hInst, hWnd);
}


/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza singura instanta posibila
 parameters : necesari pentru constructor
---------------------------------------------------------------------------*/
CPIIInputSystem* CPIIInputSystem::Instance(void *hInst, void *hWnd)
{
   if (mInstance == NULL)
   {
      mInstance = new CPIIInputSystem(hInst, hWnd);
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
  //TODO: event management specific to the API
  /*
  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_KEYDOWN:
        Keyz[event.key.keysym.sym] = 0x80;
        logWrite("Key code: "); logWriteILn((int)event.key.keysym.sym);
        logWrite("Wanted: "); logWriteILn(SDLK_a);
        break;

      case SDL_KEYUP:
        Keyz[event.key.keysym.sym] = 0;
        break;
     }
  }
  */

  poll_keyboard();
  poll_joystick();

  for(int i = 0 ; i < KEY_MAX; i++)
  {
      Keyz[i] = key[i];
      logWriteLn(CPString(i) + " - " + CPString(Keyz[i]));
  }

  logWriteLn("");

  //memcpy(Keyz, key, sizeof(Keyz));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : ca mai sus numai ca pentru mouse
//  probleme  : ca mai sus numai ca pentru mouse
//---------------------------------------------------------------------------
void CPIIInputSystem::UpdateMouse(void)
{
  // TODO: update the mouse
  //const Uint8 buttons = SDL_GetRelativeMouseState(&Mouse.x, &Mouse.y);

  //MouseLB = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT  )) != 0;
  //MouseRB = (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT )) != 0;
  //MouseMB = (buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

  MouseLB = false;
  MouseRB = false;
  MouseMB = false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : how does allegro handles this?
//---------------------------------------------------------------------------
void CPIIInputSystem::RestoreDevices(void)
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : allegro may have a remove of input device handlers
//---------------------------------------------------------------------------
void CPIIInputSystem::ReleaseDevices(void)
{
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
CPIIInputSystem::CPIIInputSystem(void *hInst, void *hWnd)
{
  MouseLB = false;
  MouseRB = false;
  MouseMB = false;
  Mouse = CPPoint(0,0);
  memset(Keyz, 0, sizeof(Keyz));
  InitInputs(hInst, hWnd);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : gata cu DirectInputu'; un pic modificata fata de CDX
//---------------------------------------------------------------------------
CPIIInputSystem::~CPIIInputSystem()
{
  remove_keyboard();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//  descriere : mda...pune in functziune achizitia de date de la input devices
//              e suficient de explicita
//              de ce se numeste PowerOn ? buna intrebare
//  probleme  : netestata
//---------------------------------------------------------------------------
bool CPIIInputSystem::InitInputs(void *hInst, void *hWnd)
{
  if (install_keyboard() < 0)
  {
      logWriteLn("Failed to init the keyboard.");
  }

  return true;
}
//---------------------------------------------------------------------------

/*
int CPIIInputSystem::GetKey(int keyCode)
{
    // TODO
    return 0;
}
//---------------------------------------------------------------------------
*/

