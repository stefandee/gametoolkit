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

  /*
   CPIIInputSystem* lISI;
   return lISI->Instance(hInst, hWnd);
  */ 
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
/*   if (hInst == NULL || hWnd == NULL)
   {
      if (mInstance != NULL)
         mInstanceCount++;
   }
   else
*/   if (mInstance == NULL)
   {
      mInstance = new CPIIInputSystem(hInst, hWnd);
   }
//   mInstanceCount++;
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
  if(lpDIDKeyboard->GetDeviceState(256, &Keyz) == (DIERR_INPUTLOST | DIERR_NOTACQUIRED))
  {
      RestoreDevices();
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : ca mai sus numai ca pentru mouse
//  probleme  : ca mai sus numai ca pentru mouse
//---------------------------------------------------------------------------
void CPIIInputSystem::UpdateMouse(void)
{
  DIMOUSESTATE MouseState;

  if(lpDIDMouse->GetDeviceState(sizeof(MouseState), &MouseState) == (DIERR_INPUTLOST | DIERR_NOTACQUIRED))
  {
      RestoreDevices();
  }

  Mouse.x = MouseState.lX;
  Mouse.y = MouseState.lY;

  MouseLB = MouseState.rgbButtons[0];
  MouseRB = MouseState.rgbButtons[1];
  MouseMB = MouseState.rgbButtons[2];
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : nu e facuta sa fie folosita din afara (adica nu e nevoie)
//              din cite am intzeles, se da acquire daca, de exemplu,
//              userul clicaie intr'un meniu (sau in orice componenta) care
//              are propriul handler de mesaje windows (WM_KEY....); atunci
//              dinputul cedeaza "focusul" si e nevoie sa fie restoreuit
//---------------------------------------------------------------------------
void CPIIInputSystem::RestoreDevices(void)
{
  lpDIDMouse->Acquire();
  lpDIDKeyboard->Acquire();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : nu shtiu foarte bine unde se foloseshte
//---------------------------------------------------------------------------
void CPIIInputSystem::ReleaseDevices(void)
{
  lpDIDMouse->Unacquire();
  lpDIDKeyboard->Unacquire();
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
CPIIInputSystem::CPIIInputSystem(void *hInst, void *hWnd)
{
  lpDI = NULL;
  lpDIDKeyboard = NULL;
  lpDIDMouse = NULL;
  MouseLB = FALSE;
  MouseRB = FALSE;
  MouseMB = FALSE;
  Mouse = CPPoint(0,0);
  ZeroMemory(&Keyz, sizeof(BYTE)*256);
  InitInputs(hInst, hWnd);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  descriere : gata cu DirectInputu'; un pic modificata fata de CDX
//---------------------------------------------------------------------------
CPIIInputSystem::~CPIIInputSystem()
{
  if (lpDI != NULL)
  {
    if (lpDIDKeyboard != NULL)
    {
      lpDIDKeyboard->Unacquire();
      lpDIDKeyboard->Release();
    }
    if (lpDIDMouse!= NULL)
    {
      lpDIDMouse->Unacquire();
      lpDIDMouse->Release();
    }
    lpDI->Release();
  }
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
BOOL CPIIInputSystem::InitInputs(void *hInst, void *hWnd)
{
   HRESULT rval;
   rval = DirectInputCreate(static_cast<HINSTANCE>(hInst), DIRECTINPUT_VERSION, &lpDI, NULL);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }
   rval = lpDI->CreateDevice(GUID_SysMouse, &lpDIDMouse, NULL);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }
   rval = lpDI->CreateDevice(GUID_SysKeyboard, &lpDIDKeyboard, NULL);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }

   rval = lpDIDMouse->SetCooperativeLevel(static_cast<HWND>(hWnd), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
   //rval = lpDIDMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }

   rval = lpDIDKeyboard->SetCooperativeLevel(static_cast<HWND>(hWnd), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
   //rval = lpDIDKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }
   rval = lpDIDMouse->SetDataFormat(&c_dfDIMouse);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }
   rval = lpDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }

   rval = lpDIDMouse->Acquire();
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }

   rval = lpDIDKeyboard->Acquire();
   if(rval != DI_OK)
   {
      logWriteLn(ErrorDetailedStr(rval));
      throw CErrorGeneric();
   }

   return TRUE;
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: returneaza eroarea intr-un string
 parameters : hRet - eroarea
---------------------------------------------------------------------------*/
CPString CPIIInputSystem::ErrorDetailedStr(HRESULT hRet)
{
   CPString lError;
   switch(hRet)
   {
      case DIERR_ACQUIRED :               lError = "DIERR_ACQUIRED"; break;
      case DIERR_ALREADYINITIALIZED:      lError = "DIERR_ALREADYINITIALIZED"; break;
      case DIERR_BETADIRECTINPUTVERSION : lError = "DIERR_BETADIRECTINPUTVERSION"; break;
      case DIERR_BADDRIVERVER:            lError = "DIERR_BADDRIVERVER"; break;
      case DIERR_DEVICEFULL:              lError = "DIERR_DEVICEFULL"; break;
      case DIERR_DEVICENOTREG:            lError = "DIERR_DEVICENOTREG"; break;
      case DIERR_EFFECTPLAYING:           lError = "DIERR_EFFECTPLAYING"; break;
      case DIERR_HASEFFECTS:              lError = "DIERR_HASEFFECTS"; break;
      case DIERR_GENERIC:                 lError = "DIERR_GENERIC"; break;
//    case DIERR_HANDLEEXISTS:            lError = ""; break;
      case DIERR_INCOMPLETEEFFECT:        lError = "DIERR_INCOMPLETEEFFECT"; break;
      case DIERR_INPUTLOST:               lError = "DIERR_INPUTLOST"; break;
      case DIERR_INVALIDPARAM :           lError = "DIERR_INVALIDPARAM"; break;
      case DIERR_MOREDATA:                lError = "DIERR_MOREDATA"; break;
      case DIERR_NOAGGREGATION:           lError = "DIERR_NOAGGREGATION"; break;
      case DIERR_NOINTERFACE:             lError = "DIERR_NOINTERFACE"; break;
      case DIERR_NOTACQUIRED:             lError = "DIERR_NOTACQUIRED"; break;
      case DIERR_NOTBUFFERED:             lError = "DIERR_NOTBUFFERED"; break;
      case DIERR_NOTDOWNLOADED:           lError = "DIERR_NOTDOWNLOADED"; break;
      case DIERR_NOTEXCLUSIVEACQUIRED:    lError = "DIERR_NOTEXCLUSIVEACQUIRED"; break;
      case DIERR_NOTFOUND:                lError = "DIERR_NOTFOUND"; break;
      case DIERR_NOTINITIALIZED :         lError = "DIERR_NOTINITIALIZED"; break;
//    case DIERR_OBJECTNOTFOUND:          lError = ""; break;
      case DIERR_OLDDIRECTINPUTVERSION :  lError = "DIERR_OLDDIRECTINPUTVERSION"; break;
      case DIERR_OTHERAPPHASPRIO:         lError = "DIERR_OTHERAPPHASPRIO"; break;
      case DIERR_OUTOFMEMORY :            lError = "DIERR_OUTOFMEMORY"; break;
//    case DIERR_READONLY:                lError = ""; break;
      case DIERR_REPORTFULL:              lError = "DIERR_REPORTFULL"; break;
      case DIERR_UNPLUGGED:               lError = "DIERR_UNPLUGGED"; break;
      case DIERR_UNSUPPORTED:             lError = "DIERR_UNSUPPORTED"; break;
      case E_HANDLE:                      lError = "E_HANDLE"; break;
      case E_PENDING:                     lError = "E_PENDING"; break;
      default:                            lError = "UNKNOWN DIERR"; break;
   }
   return lError;
}
//---------------------------------------------------------------------------

