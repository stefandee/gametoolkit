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

 description: Class Cursor
              update, paint,
              mouse down, up, press
              set type, speedfactor, pos
              Singleton Pattern
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
#include "easylogging++.h"
#pragma hdrstop

#include "PP_Cursor.h"
#include "basepath.h"
#include "Paths.h"             //pentru a sti cum se cheama fisierul
#include "PP_Graphic.h"

CCursor *CCursor::mInstance = NULL;
int      CCursor::mInstanceCount = 0;;

/*---------------------------------------------------------------------------
 description: returneaza instanta de cursor system
 others     : a aparut din cauza warningurilor
---------------------------------------------------------------------------*/
CPGICursor* GetCursorInstance()
{
   return CCursor::Instance();

   /*
   CCursor *lCI;
   return lCI->Instance();
   */
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza o instanta a cursorului
---------------------------------------------------------------------------*/
CCursor* CCursor::Instance()
{
   if (mInstance == NULL)
   {
      mInstance = new CCursor();
   }
   mInstanceCount ++;
   return mInstance;
}

/*---------------------------------------------------------------------------
 description: elibereaza instanta cursorului
---------------------------------------------------------------------------*/
void CCursor::Release()
{
   if (mInstance != NULL)
   {
      delete mInstance;
      mInstance = NULL;
   }
}

/*---------------------------------------------------------------------------
 description: loads the sprite from file
 parameters : fileName - the file name
              wcells, hcells - the number of animations in the sprite
---------------------------------------------------------------------------*/
void CCursor::LoadSpriteFromFile(CPString fileName, int wCells, int hCells)
{
   mSprite = GetGraphicInstance()->AddSprite(fileName, wCells, hCells);
}

/*--------------------------------------------------------------------------
 Description: metoda de afisare
 Parameters : none
 Others     : none
 --------------------------------------------------------------------------*/
void CCursor::Paint()
{
   if (mVisible == false)
      return;

#if PPT_USE_SDL
   GetGraphicInstance()->SetClipRegion(mRestrictedRect);
#else
   GetGraphicInstance()->SetClipRegion(NULL, mRestrictedRect);
#endif // PPT_USE_SDL

   if (mSprite == NULL)
   {
      GetGraphicInstance()->SetColor(0xFFFFFF);
      GetGraphicInstance()->Line(CPPoint(mPosition.x, mPosition.y), CPPoint(mPosition.x, mPosition.y+5));
      GetGraphicInstance()->Line(CPPoint(mPosition.x, mPosition.y), CPPoint(mPosition.x+4, mPosition.y+4));
      return;
   }
   CPGIGraphicSystem *lGSI;   //adica local Graphic System Instance
   try
   {
      lGSI = GetGraphicInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "Cursor::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
      throw;
   }
   lGSI->PaintSprite(mPosition.x, mPosition.y, mSprite, mAnim, mType);

/*   try
   {
      if (Type == MC_ONOWNED || Type == MC_ONENEMY )
      {
         lGSI->PaintSpriteToSurface(NULL, Position.x, Position.y, Sprite, Anim, MC_DEFAULT);
         lGSI->PaintSpriteToSurface(NULL, Position.x, Position.y-6, Sprite, Anim, Type);
      }
      else
      {
         if (Type == MC_GUARD || Type == MC_CAPTURE ||
            Type == MC_PATROL || Type == MC_LOAD || Type == MC_UNLOAD)
         {
            lGSI->PaintSpriteToSurface(NULL, Position.x, Position.y, Sprite, Anim, MC_MOVE);
            lGSI->PaintSpriteToSurface(NULL, Position.x, Position.y-6, Sprite, Anim, Type);
         }
         else
            lGSI->PaintSpriteToSurface(NULL, Position.x, Position.y, Sprite, Anim, Type);
      }
   }
   catch(...)
   {
      logWriteLn("UNABLE TO PAINT CURSOR !!!!!!!!!!!!!");
   }
*/
}

/*--------------------------------------------------------------------------
 Description: metoda care face update de animatie, pozitie a cursorului
 Parameters :
 Others     : se apeleaza la fiecare tick
              foloseste adresare directa catre Inputs
              (poate nu-i chiar bine)
--------------------------------------------------------------------------*/
void CCursor::Update()
{
   CPIIInputSystem *lISI;
   try
   {
      lISI = lISI->Instance();
   }
   catch(...)
   {
      throw;
   }
   lISI->UpdateMouse();
   if (mEnabled == false)
      return;
   mOldPosition = CPPoint(mPosition.x, mPosition.y);

   mPosition.x += mSpeedFactor * lISI->Mouse.x;
   mPosition.y += mSpeedFactor * lISI->Mouse.y;

   if (mPosition.x + mHotSpot.x > mRestrictedRect.right)
      mPosition.x = mRestrictedRect.right - mHotSpot.x;
   if (mPosition.x + mHotSpot.x < mRestrictedRect.left)
      mPosition.x = mRestrictedRect.left - mHotSpot.x ;
   if (mPosition.y + mHotSpot.y  > mRestrictedRect.bottom)
      mPosition.y = mRestrictedRect.bottom - mHotSpot.y ;
   if (mPosition.y + mHotSpot.y < mRestrictedRect.top)
      mPosition.y = mRestrictedRect.top - mHotSpot.y ;

   mOldButtons = mButtons;

   mButtons.mLeft   = (lISI->MouseLB != 0);
   mButtons.mRight  = (lISI->MouseRB != 0);
   mButtons.mMiddle = (lISI->MouseMB != 0);

   mAnimUpdateCounter++;
   if (mAnimUpdateCounter.MaxReached() == true)
   {
      mAnim++;
      mAnim = mAnim % 6;
   }
}

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
 Description: constructorul
 --------------------------------------------------------------------------*/
CCursor::CCursor()
{
   CPGIGraphicSystem *lGSI;   //adica local Graphic System Instance
   try
   {
      lGSI = lGSI->Instance();
   }
   catch(...)
   {
      throw;
   }
   mSprite = NULL;
   mAnim = 0;
   mPosition = CPPoint(200, 200);
   mOldPosition = CPPoint(200, 200);
   mType = 0;
   SetType(0);
   mSpeedFactor = 2;
   mAnimUpdateCounter.SetMaxValue(3);
   mAnimUpdateCounter.Reset();
   mRestrictedRect = CPRect(0, 0, lGSI->GetResX(), lGSI->GetResY());
   mVisible = true;
   mEnabled = true;
}

/*--------------------------------------------------------------------------
 Description: destructorul
 ---------------------------------------------------------------------------*/
CCursor::~CCursor()
{
}

/*--------------------------------------------------------------------------
 Description: seteaza tipul cursorului de mouse
 Parameters : tip (constante definite in GameDef)
 Others     : se apeleaza atunci cand se trece peste un screen
 -------------------------------------------------------------------------*/
void CCursor::SetType(int type)
{
   mPosition.x += gMouseHotSpots[mType].x - gMouseHotSpots[type].x;
   mPosition.y += gMouseHotSpots[mType].y - gMouseHotSpots[type].y;
   mHotSpot.x = gMouseHotSpots[type].x;
   mHotSpot.y = gMouseHotSpots[type].y;
   mType = type;
}

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
