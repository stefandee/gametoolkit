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
              mai trebuie adaugat un contor de update
 last modify: 17 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include <memory.h>

#include "PP_Keyboard.h"
#include "PP_KeyConvertor.h"
#include "PP_Input.h"
#include "logfile.h"

CKeyboard* CKeyboard::mInstance = NULL;

/*---------------------------------------------------------------------------
 description: returneaza instanta de cursor system
 others     : a aparut din cauza warningurilor
---------------------------------------------------------------------------*/
CPGIKeyboard* GetKeyboardInstance()
{
  return CKeyboard::Instance();
}


/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
CKeyboard* CKeyboard::Instance()
{
   if (mInstance == NULL)
      mInstance = new CKeyboard();
   return mInstance;
}
void CKeyboard::Release()
{
   if (mInstance != NULL)
   {
      delete mInstance;
      mInstance = NULL;
   }
}


/*--------------------------------------------------------------------------
 Description: memoreaza vechile valori si apeleaza update-ul pentru inputs
--------------------------------------------------------------------------*/
void CKeyboard::Update()
{
   CPIIInputSystem *lISI;
   try
   {
      lISI = CPIIInputSystem::Instance();
   }
   catch(...)
   {
      throw;
   }

   memcpy(oldKeyz, lISI->Keyz, KEY_MAX);
   lISI->UpdateKeys();
}
//---------------------------------------------------------------------------

/*--------------------------------------------------------------------------
 Description: true daca cheia dik este apasata
 Others     : dik = Direct Input Key
 --------------------------------------------------------------------------*/
bool CKeyboard::IsPressed(unsigned char dik)
{
   CPIIInputSystem *lISI;

   int lKey = PPKToKey(dik);

  logWriteLn(CPString("IsPressed: ") + CPString(dik) + " to " + CPString(lKey));

   try
   {
      lISI = CPIIInputSystem::Instance();
   }
   catch(...)
   {
      throw;
   }
   if ((lISI->Keyz[lKey] == 255) && (oldKeyz[lKey] == 255))
      return true;
   else
      return false;
}
//---------------------------------------------------------------------------

/*--------------------------------------------------------------------------
 Description: true daca cheia dik tocmai s-a apasata
 Others     : dik = Direct Input Key
 --------------------------------------------------------------------------*/
bool CKeyboard::IsDown(unsigned char dik)
{
   CPIIInputSystem *lISI;

   int lKey = PPKToKey(dik);

  logWriteLn(CPString("IsDown: ") + CPString(dik) + " to " + CPString(lKey));

   try
   {
      lISI = CPIIInputSystem::Instance();
   }
   catch(...)
   {
      throw;
   }
   if ((lISI->Keyz[lKey] == 255) && (oldKeyz[lKey] == 0))
      return true;
   else
      return false;
}
//---------------------------------------------------------------------------

/*--------------------------------------------------------------------------
 Description: true daca cheia dik tocmai s-a ridicat
 Others     : dik = Direct Input Key
 --------------------------------------------------------------------------*/
bool CKeyboard::IsUp(unsigned char dik)
{
   CPIIInputSystem *lISI;

   int lKey = PPKToKey(dik);

  logWriteLn(CPString("IsUp: ") + CPString(dik) + " to " + CPString(lKey));

   try
   {
      lISI = CPIIInputSystem::Instance();
   }
   catch(...)
   {
      throw;
   }
   if ((lISI->Keyz[lKey] == 0) && (oldKeyz[lKey] == 255))
      return true;
   else
      return false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the first encountered key pressed
---------------------------------------------------------------------------*/
int CKeyboard::GetFirstKeyPressed()
{
   for (int i=0; i < 1024; i++)
      if (IsPressed(i) == true)
         return i;
   return -1;
}
/*---------------------------------------------------------------------------
 description: gets the first encountered key down
---------------------------------------------------------------------------*/
int CKeyboard::GetFirstKeyDown()
{
   for (int i=0; i < 1024; i++)
      if (IsDown(i) == true)
         return i;
   return -1;
}
/*---------------------------------------------------------------------------
 description: gets the first encountered key up
---------------------------------------------------------------------------*/
int CKeyboard::GetFirstKeyUp()
{
   for (int i=0; i < 1024; i++)
      if (IsUp(i) == true)
         return i;
   return -1;
}


/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
 Description: constructorul
--------------------------------------------------------------------------*/
CKeyboard::CKeyboard()
{
  updateCounter = 0;
}
//---------------------------------------------------------------------------

/*--------------------------------------------------------------------------
 Description: destructorul
--------------------------------------------------------------------------*/
CKeyboard::~CKeyboard()
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
 Description: metoda interna
              sterge keys-urile
 Others     : nefolosita
--------------------------------------------------------------------------*/
void CKeyboard::Clear()
{
   int i;
   CPIIInputSystem *lISI;
   try
   {
      lISI = CPIIInputSystem::Instance();
   }
   catch(...)
   {
      throw;
   }
   for (i=0; i < KEY_MAX; i++)
   {
      lISI->Keyz[i] = 0;
      oldKeyz[i] = 0;
   }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
