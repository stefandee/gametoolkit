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
 application: Pure Power Engine

 description: Graphic System Class
              nu poate avea decat o singura instanta (Singleton)
 last modify: 13 07 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#pragma hdrstop

#include "PP_Graphic.h"
#include "GBmpLoad.h"
#include "logfile.h"
#include "basic3d.h"

#ifdef _MSC_VER	// Standard workaround for Microsoft compiler bug
#define for if(0) {} else for
#endif

//---------------------------------------------------------------------------


CPGIGraphicSystem* CPGIGraphicSystem::mInstance = NULL; //initializarea instantei
int                CPGIGraphicSystem::mInstanceCount = 0;

//TEST METHOD
void CPGIGraphicSystem::WriteSprites()
{
   //Sprites->WriteFiles();
}

/*---------------------------------------------------------------------------
 description: pentru returnarea instantei de Graphic System
              se transmite explicit modul dorit
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
              mode - arata modul dorit
 others     : pentru eliminarea warningurilor
---------------------------------------------------------------------------*/
CPGIGraphicSystem* GetGraphicInstance(/*HWND hWnd,*/
                                      int resX, int resY,
                                      PGIModeType mode)
{
   return CPGIGraphicSystem::Instance(/*hWnd,*/ resX, resY, mode);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creaza o instanta daca nu exista sau returneaza this daca ea exista
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
---------------------------------------------------------------------------*/
CPGIGraphicSystem* CPGIGraphicSystem::Instance(/*HWND hWnd,*/
                                               int resX, int resY,
                                               PGIModeType mode)
{
   if (mInstance == 0)
   {
      mInstance = new CPGIGraphicSystem(/*hwnd, */resX, resY, mode);
   }

   return mInstance;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: release instance, decrementeaza mInstanceCount + daca este < 0 => delete
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Release()
{
   if (mInstance != NULL)
   {
      delete mInstance;
      mInstance = 0;
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza clip region
 parameters : ClipRegion - rect
              Surface - daca este NULL => se seteaza back sau in primary dupa
                        mDoubleBuffered
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::SetClipRegion(CPGISurface* Surface, CPRect clipRegion)
{
   Surface = GetPaintableSurface(Surface);
   assert(Surface);
   return Surface->SetClipRect(clipRegion);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza clipul original, adica cat toata suprafata
 parameters : Surface - daca este NULL => se seteaza back sau in primary dupa
                        mDoubleBuffered
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::ResetClipRegion(CPGISurface* Surface)
{
   Surface = GetPaintableSurface(Surface);
   assert(Surface);
   return Surface->ResetClipRect();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adica face flip daca este double buffered
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::PaintToScreen()
{
   /*
   if (mWindowed)
   {
      // TODO: do blit
      //SDL_BlitSurface(mDDSBack->pimpl, 0, mDDSPrimary->pimpl, 0);
	  //SDL_Rect rc = { 0, 0, mDDSPrimary->Width(), mDDSPrimary->Height() };
      //SDL_UpdateRects(mDDSPrimary->pimpl, 1, &rc);
   }
   else
   {
      // TODO: do flip
      //SDL_Flip(GetPaintableSurface(0)->pimpl);
   }
   */

   // TODO: verify this
   //show_video_bitmap(GetPaintableSurface(0)->pimpl);
   //show_video_bitmap(mDDSBack->pimpl);
   blit(mDDSBack->pimpl, screen, 0, 0, 0, 0, mResolutionX, mResolutionY);

   int x1, x2, y1, y2;

   get_clip_rect(mDDSBack->pimpl, &x1, &y1, &x2, &y2);

   logWriteLn(CPString("back clip rect: ") + CPString(x1) + " - " + CPString(y1) + " - " + CPString(x2) + " - " + CPString(y2));
   logWriteLn(CPString("back size: ") + CPString(mDDSBack->pimpl->w) + " - " + CPString(mDDSBack->pimpl->h));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: tipareste Sprite in back ,primary sau in Surface
 parameters : Sprite - pointer la CSprite
              anims  - care animatie din sprite trebuie tiparita
              Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              zoom    - factor cu care se imparte rectul destinatie
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::PaintSpriteToSurface(CPGISurface *Surface, int whereX, int whereY,
                                             TPGISpriteReference Sprite, int animCount, int animType,
                                             int zoomFactor, int alpha)
{
   Surface = GetPaintableSurface(Surface);
   CPGISurface *lSpriteSurf;
   CPRect lSource, lDest;
   lSpriteSurf = Sprite->GetSurface();
   lSource     = Sprite->GetAnimRect(animCount, animType);
   lDest       = CPRect(whereX, whereY, whereX + lSource.right - lSource.left, whereY + lSource.bottom - lSource.top);

   //logWriteLn(CPString("src rect: ") + CPString(lSource.left) + " - " + CPString(lSource.top) + " - " + CPString(lSource.right) + " - " + CPString(lSource.bottom));
   //logWriteLn(CPString("dest rect: ") + CPString(lDest.left) + " - " + CPString(lDest.top) + " - " + CPString(lDest.right) + " - " + CPString(lDest.bottom));

   PaintSurfaceToSurface(Surface, lDest, lSpriteSurf, lSource, alpha);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: aka BLT
 parameters : destSurface   - the destination surface
              Dest          - the destination rectangle
              sourceSurface - the source surface (where to copy from)
              Source        - the source rectangle
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::PaintSurfaceToSurface(CPGISurface *destSurface, CPRect Dest,
                                              CPGISurface *sourceSurface, CPRect Source, int alpha)
{
   destSurface = GetPaintableSurface(destSurface);

   if (sourceSurface == NULL)
   {
      logWriteLn("CPGIGraphicSystem::PaintSurfaceToSurface - Source Surface is NULL !!!!!!!!!!");
      return;
   }

    //SDL_Rect rcSrc = { (short)Source.left, (short)Source.top, (short)Source.Width(), (short)Source.Height() };
    //SDL_Rect rcDst = { (short)Dest.left, (short)Dest.top, (short)Dest.Width(), (short)Dest.Height() };

    // test
    //SDL_SetAlpha(sourceSurface->pimpl, SDL_SRCALPHA, alpha);

    //SDL_BlitSurface(sourceSurface->pimpl, &rcSrc, destSurface->pimpl, &rcDst);
    // TODO: do blit of surface to another surface

    //logWriteLn(CPString("surface to surface: ") + CPString(Source.Width()) + "x" + CPString(Source.Height()));

    acquire_bitmap(destSurface->pimpl);
    masked_blit(sourceSurface->pimpl, destSurface->pimpl, Source.left, Source.top, Dest.left, Dest.top, Source.Width(), Source.Height());
    release_bitmap(destSurface->pimpl);
}
//---------------------------------------------------------------------------

CPRect CPGIGraphicSystem::GetPaintRect(CPPoint _where, TPGISpriteReference Sprite, int animCount, int animType, int zoomFactor)
{
  if (!Sprite)
  {
    return CPRect(-1, -1, -1, -1);
  }

  CPRect lSource;

  lSource = Sprite->GetAnimRect(animCount, animType);

  return CPRect(_where.x, _where.y, _where.x + lSource.right - lSource.left, _where.y + lSource.bottom - lSource.top);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza un dreptunghi gol cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              rect    - dim dreptunghiului
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Rectangle(CPGISurface* Surface, CPRect rect, PP_int32_t color)
{
   Surface = GetPaintableSurface(Surface);
   Surface->FrameRect(rect, color);
                      //makeRGB16(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza un semi-dreptunghi (adica doar in colturi) cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              rect    - the rectangle
              Lenght  - the length of the lines
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::SelectionRectangle(CPGISurface* Surface, CPRect rect, int Length, PP_int32_t color)
{
   Surface = GetPaintableSurface(Surface);
   Surface->DrawSelRect(rect,
                        Length,
                        makeRGB16(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza un dreptunghi plin cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::FillRectangle(CPGISurface* Surface, CPRect rect, PP_int32_t color)
{
   Surface = GetPaintableSurface(Surface);
   Surface->FillRect(rect,
                     makeRGB16(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza o linie cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Line(CPGISurface* Surface, CPPoint ptStart, CPPoint ptEnd, PP_int32_t color)
{
  Surface = GetPaintableSurface(Surface);

  Surface->Line
    (
      ptStart,
      ptEnd,
      makeRGB16(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color))
    );
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: umple toata suprafata cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::FillSurface(CPGISurface* Surface, PP_int32_t color)
{
   Surface = GetPaintableSurface(Surface);
   assert(Surface);
   Surface->FillSurface(makeRGB16(gfGetRValue(color),
                                  gfGetGValue(color),
                                  gfGetBValue(color)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: tipareste un cerc
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              ptCenter- centrul cercului
              Radius  - raza cercului
              color   - culoarea
 others     :
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Circle(CPGISurface* Surface, CPPoint ptCenter, int Radius, PP_int32_t color)
{
   Surface = GetPaintableSurface(Surface);

   assert(Surface);

   Surface->Circle
     (
       ptCenter,
       Radius,
       makeRGB16(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color))
     );
}
//---------------------------------------------------------------------------

// Description : draws a pie (a filled circular sector)
// Params      : there are many ways to pass parameters for a pie; I've chosen
//               the following interface:
//               _center - a point for the center of the pie
//               _radius - the radius of the circle this sector belongs
//               _dir    - a vector representing the central axis of the pie
//               _angle  - relative to the direction above, the angle that defines the pie
//               _color  - the color the pie will be filled
// Result      : the algorithm goes as follows:
//               * if the angle is higher than 90, than split it in half, compute the new directions
//                 and fill two pies with the two half-angles (this isn't yet done)
//               * compute the two points where the _dir +/- _angle/2 cross the circle => points P1 and P2
//               * fill a triangle with the vertices _center, P1, P2
//               * chose between the steep of the line P1-P2; if the steep is higher than 45 and lower than
//                 lower than 225, then go from P1.y to P2.y, find the intersection of the horizontal line
//                 with the circle and draw a line; else, go from P1.x to P2.x, find the intersection of
//                 the vertical line with the circle and draw a line
// Comments    : Dont use this function with angle value higher than 180
// Comments comment: TMN: Why not? It seems to work just fine.
void CPGIGraphicSystem::Pie(CPGISurface* Surface,
                            CPPoint      _center,
                            int          _radius,
                            CPPoint      _dir,
                            int          _angle,
                            PP_int32_t     _color)
{
   Surface = GetPaintableSurface(Surface);

   CPGISurface::SurfaceLocker locker(*Surface);

   /*
   CPRect rcClip = Surface->GetClipRect();
   --rcClip.right;
   --rcClip.bottom;
   setGlobalClipRect(rcClip);
   */

   // make the color
   //_color = makeRGB16(gfGetRValue(_color), gfGetGValue(_color), gfGetBValue(_color));

   // normalize the direction
   float lValue = sqrt(_dir.x * _dir.x + _dir.y * _dir.y);

   if (lValue == 0)
   {
     return;
   }

   float lDirX = (float)_dir.x / lValue;
   float lDirY = (float)_dir.y / lValue;

   // compute the two points on the circle based on direction and angle
   float lAngle = _angle/2 * PI/180.0;
   float lSinPlus = sin(lAngle),   lCosPlus = cos(lAngle);
   float lSinMinus = sin(-lAngle), lCosMinus = cos(-lAngle);

   float x1 = _radius * (lDirX * lCosPlus - lDirY * lSinPlus);
   float y1 = _radius * (lDirX * lSinPlus + lDirY * lCosPlus);

   float x2 = _radius * (lDirX * lCosMinus - lDirY * lSinMinus);
   float y2 = _radius * (lDirX * lSinMinus + lDirY * lCosMinus);

   // draw the main part - the triangle
   Surface->FillTriangle(
              CPPoint(_center.x + x1, _center.y + y1),
              CPPoint(_center.x + x2, _center.y + y2),
              CPPoint(_center.x, _center.y),
              _color
   );

   /*
   ::fillTriangle(static_cast<char*>(Surface->Bits()),
              Surface->Pitch(),
              _center.x + x1, _center.y + y1,
              _center.x + x2, _center.y + y2,
              _center.x, _center.y,
              static_cast<PP_uint16_t>(_color)
             );
    */

   // go go with the rest of the pie (yummy! :)

   // it depends on how the steep of line - if dy > dx, then we have to draw
   // using horizontal lines
   if (abs(y1 - y2) > abs(x1 - x2))
   {
     // go with the line (x1, y1) to (x2, y2), draw horizonal lines (y is constant)
     // intersect with the circle and chose a point between the +/-
     int t;

     // sort and swap the (x1, y1) and (x2, y2)
     if (y2 < y1)
     {
       t = y1; y1 = y2; y2 = t;
       t = x1; x1 = x2; x2 = t;
     }

     // prepare to scan the line - dont care for a line Bresenham for now
     float lM;

     if ((y2 - y1)!=0)
     {
       lM = (float)(x2 - x1) / (y2 - y1);
     }
     else
     {
       lM = (x2 - x1) * 100;
     }

     // compute the sign of the point (_dir.x * radius, _dir.y * radius) so that we know which
     // of the two intersections of the horizontal line with the circle to chose
     int lSign = -1;

     if (_radius * lDirX - x1 - lM * (_radius * lDirY - y1) > 0)
     {
       lSign = 1;
     }

     // we got the steep, ready to scan
     int   y = y1;
     float x = x1;

     while (y < y2)
     {
       // compute the intersections between horizontal line with y=ct and the circle
       float x0 = sqrt(fabs(_radius * _radius - y * y));

       Surface->HLine(_center.x + x,
             _center.x + lSign * x0,
             _center.y + y,
             _color
       );

       /*
       hLine(static_cast<char*>(Surface->Bits()), Surface->Pitch(),
             _center.x + x,
             _center.x + lSign * x0,
             _center.y + y,
             static_cast<PP_uint16_t>(_color)
             );
       */

       y++;
       x  += lM;
     }

     //setGlobalClipRect(rcClip);
     return;
   }

   // it depends on how the steep of line - if dy > dx, then we have to draw
   // using vertical lines

   // go with the line (x1, y1) to (x2, y2), draw vertical lines (x is constant)
   // intersect with the circle and chose a point between the +/-
   int t;

   // sort and swap the (x1, y1) and (x2, y2)
   if (x2 < x1)
   {
     t = y1; y1 = y2; y2 = t;
     t = x1; x1 = x2; x2 = t;
   }

   // prepare to scan the line - dont care for a line Bresenham for now
   float lM;

   if ((x2 - x1)!=0)
   {
     lM = (float)(y2 - y1) / (x2 - x1);
   }
   else
   {
     lM = (y2 - y1) * 100;
   }

   // compute the sign of the point (_dir.x * radius, _dir.y * radius) so that we know which
   // of the two intersections of the horizontal line with the circle to chose
   int lSign = -1;

   if (_radius * lDirY - y1 - (_radius * lDirX - x1) * lM > 0)
   {
     lSign = 1;
   }

   // we got the steep, ready to scan
   float y = y1;
   int   x = x1;

   while (x < x2)
   {
     // compute the intersections between horizontal line with y=ct and the circle
     float y0 = sqrt(fabs(_radius * _radius - x * x));

     Surface->VLine(_center.x + x,
           _center.y + lSign * y0,
           _center.y + y, _color);

/*
     vLine(static_cast<char*>(Surface->Bits()), Surface->Pitch(),
           _center.x + x,
           _center.y + lSign * y0,
           _center.y + y,
           static_cast<PP_uint16_t>(_color)
           );
           */

     x++;
     y += lM;
   }

   //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: tipareste un cerc
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              ptCenter- centrul cercului
              Radius  - raza cercului
              color   - culoarea
 others     :
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::PutPixel(CPGISurface* Surface, CPPoint ptCenter, PP_int32_t color)
{
   Surface = GetPaintableSurface(Surface);

   //CPGISurface::SurfaceLocker locker(*Surface);

   //color = makeRGB16(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color));

   Surface->Circle(ptCenter, 1, color);

   //::circle(static_cast<char*>(Surface->Bits()), Surface->Pitch(),
   //          ptCenter.x, ptCenter.y,
   //          1,
   //          static_cast<PP_uint16_t>(color));
//   putPixel((BYTE*)Surface->Bits(), Surface->Pitch(),
//             ptCenter.x, ptCenter.y,
//             color);
}
//---------------------------------------------------------------------------

PP_int32_t CPGIGraphicSystem::GetPixel(CPGISurface* Surface, CPPoint ptCenter)
{
   Surface = GetPaintableSurface(Surface);

   //CPGISurface::SurfaceLocker locker(*Surface);

   // TODO: implement a getpixel in surface
   //return    ::getPixel(static_cast<char*>(Surface->Bits()), Surface->Pitch(),
   //          ptCenter.x, ptCenter.y);
   return 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: tipreste un text intr-un chenar
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              Text    - textul de tiparit
              ClipRect- dreptunghiul de clip
              Format  - flag care arata formatul aliniamentului
 others     : culoarea trebuie setata in prealabil (mColor)
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::DrawText(CPGISurface *Surface, CPString Text,
                                  CPRect ClipRect,
                                  int Format)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: tipreste un text
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              Text    - textul de tiparit
              Alignment - aliniamentul fata de X, Y (default este TA_LEFT)
 others     : culoarea trebuie setata in prealabil (mColor)
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::TextOut(CPGISurface* Surface,
                                int X, int Y,
                                CPString Text,
                                unsigned int Alignment)
{
   if (!mFont)
   {
     return;
   }

   if (Alignment & PPDT_RIGHT)
   {
     X = X - mFont->GetTextWidth(Text);
   }

   if (Alignment & PPDT_CENTER)
   {
     X = X - mFont->GetTextWidth(Text) / 2;
   }

   mFont->PaintText(GetPaintableSurface(Surface), X, Y, Text);
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: add sprite (adica o imagine) in lista generala
 parameters : file name, no of cells per width, no of cells per height
---------------------------------------------------------------------------*/
CPGISprite* CPGIGraphicSystem::AddSprite(CPString fileName, int wCells, int hCells, PGIList _list)
{
  int lIndex;

  switch(_list)
  {
    case SPRL_GENERIC:
      lIndex = mGenericSprites->Add(fileName, wCells, hCells);
      return mGenericSprites->GetSprite(lIndex);

    case SPRL_MISSION:
      lIndex = mMissionSprites->Add(fileName, wCells, hCells);
      return mMissionSprites->GetSprite(lIndex);
  }

  return NULL;
}
//---------------------------------------------------------------------------

CPGISprite* CPGIGraphicSystem::AddSpriteCustom(int _width, int _height, int wCells, int hCells, PGIList _list)
{
  int lIndex;

  switch(_list)
  {
    case SPRL_GENERIC:
      lIndex = mGenericSprites->AddCustom(_width, _height, wCells, hCells);
      return mGenericSprites->GetSprite(lIndex);

    case SPRL_MISSION:
      lIndex = mMissionSprites->AddCustom(_width, _height, wCells, hCells);
      return mMissionSprites->GetSprite(lIndex);
  }

  return NULL;
}
//---------------------------------------------------------------------------

int CPGIGraphicSystem::GetSpriteIndex(CPGISprite* _sprite, PGIList _list)
{
  CPGISpriteList* lSpriteList;

  if (!_sprite)
  {
    return -1;
  }

  switch(_list)
  {
    case SPRL_GENERIC:
      lSpriteList = mGenericSprites;
      break;

    case SPRL_MISSION:
      lSpriteList = mMissionSprites;
      break;

    default:
      return -1;
  }

  for(int i = 0; i < lSpriteList->noOfSprites; i++)
  {
    if (lSpriteList->GetSprite(i))
    {
      if (_sprite == lSpriteList->GetSprite(i))
      {
        return i;
      }
    }
  }

  return -1;
}
//---------------------------------------------------------------------------

CPGISprite* CPGIGraphicSystem::GetSpriteByIndex(int _index, PGIList _list)
{
  switch(_list)
  {
    case SPRL_GENERIC:
      return mGenericSprites->GetSprite(_index);

    case SPRL_MISSION:
      return mMissionSprites->GetSprite(_index);
  }

  return NULL;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: neimplementata
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::ReleaseSprite(CPGISprite *Sprite, PGIList _list)
{
  switch(_list)
  {
    case SPRL_GENERIC:
      mGenericSprites->Remove(Sprite);
	  return;

    case SPRL_MISSION:
      mMissionSprites->Remove(Sprite);
	  return;
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: neimplementata
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::ReleaseAllSprites(PGIList _list)
{
  switch(_list)
  {
    case SPRL_GENERIC:
      mGenericSprites->RemoveAll();
	  return;

    case SPRL_MISSION:
      mMissionSprites->RemoveAll();
	  return;
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: neimplementata
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::RestoreSprites()
{
}
//---------------------------------------------------------------------------

int CPGIGraphicSystem::GetSpriteCount(PGIList _list)
{
  switch(_list)
  {
    case SPRL_GENERIC:
      return mGenericSprites->noOfSprites;

    case SPRL_MISSION:
      return mMissionSprites->noOfSprites;
  }

  return -1;
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: creaza o suprafata cu dimensiunile date si ret. un pointer
 parameters : Width, Height - dimensiunile suprafetei
---------------------------------------------------------------------------*/
CPGISurface* CPGIGraphicSystem::CreateSurface(int Width, int Height, bool bSysMem)
{
   CPGISurface* lSurface = new CPGISurface;

   if (bSysMem)
   {
       lSurface->pimpl = create_system_bitmap(Width, Height);
   }
   else
   {
       lSurface->pimpl = create_video_bitmap(Width, Height);

       if (!lSurface->pimpl)
       {
         logWriteLn("CPGIGraphicSystem::CreateSurface - could not create in video memory, creating in system memory.");
         lSurface->pimpl = create_system_bitmap(Width, Height);
       }
   }

   if (!lSurface->pimpl)
   {
     logWriteLn("CPGIGraphicSystem::CreateSurface - failed to create surface.");
   }

   // TODO: set the color key to other value than the default allegro magenta
   //SDL_SetColorKey(lSurface->pimpl, SDL_SRCCOLORKEY, 0);

   lSurface->ResetClipRect();

   return lSurface;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: neimplementata
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::AddSurfaceRef(CPGISurface *Surface)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: neimplementata
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::ReleaseSurface(CPGISurface *Surface)
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructorul clasei
 parameters : resX, resY - rezolutia dorita
---------------------------------------------------------------------------*/
CPGIGraphicSystem::CPGIGraphicSystem(int resX, int resY, PGIModeType mode)
:	mDDSPrimary(new CPGISurface),
	mDDSBack(new CPGISurface)
{
   mResolutionX = 0;//resX;
   mResolutionY = 0;//resY;
   //mResolutionX = 1024;
   //mResolutionY = 768;

#ifndef PPT_USE_SDL
   mDD         = NULL;
#endif

   mGenericSprites = new CPGISpriteList();
   mMissionSprites = new CPGISpriteList();

   mScreenShotsCount = 0;

   mColor      = 0x00FFFFFF;
   //mPen        = CreatePen(PS_SOLID, 0, mColor);
   //mFont       = new CPFont("./font.fon");

   switch(mode)
   {
     case PGI_WINDOWED:
       mWindowed = true;
       break;

     case PGI_FULLSCREEN:
       mWindowed = false;
       break;

     default:
       mWindowed = false;
   }

   mDoubleBuffered   = true;

   InitGraphics(/*hwnd, */resX, resY, mWindowed);
}
//---------------------------------------------------------------------------

CPGIGraphicSystem::CPGIGraphicSystem(/*HWND hWnd,*/ int resX, int resY)
:	mDDSPrimary(new CPGISurface),
	mDDSBack(new CPGISurface)
{
   mResolutionX = 0;
   mResolutionY = 0;

   mDDSPrimary = NULL;
   mDDSBack    = NULL;

   mGenericSprites = new CPGISpriteList();
   mMissionSprites = new CPGISpriteList();

   mScreenShotsCount = 0;

   mColor      = 0x00FFFFFF;
   //mPen        = CreatePen(PS_SOLID, 0, mColor);
   //mFont       = new CPFont(getGamePath() + PATH_FONTS + FILE_FONT_SMALL);
   mFont = 0;

   mWindowed         = false;
   mDoubleBuffered   = true;

   InitGraphics(/*hwnd, */resX, resY, mWindowed);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructorul clasei
 others     : elibereaza obiectele DX si lista de sprite-uri
---------------------------------------------------------------------------*/
CPGIGraphicSystem::~CPGIGraphicSystem()
{
   //DeleteObject(mPen);
   logWriteLn("CPGIGraphicSystem::~CPGIGraphicSystem()", LOGDET_LOWEST);

   if (mGenericSprites != NULL)
   {
     delete mGenericSprites;
   }

   if (mMissionSprites != NULL)
   {
     delete mMissionSprites;
   }

   logWriteLn("CPGIGraphicSystem::~CPGIGraphicSystem() - sprites deleted", LOGDET_LOWEST);

   delete mDDSBack;
   delete mDDSPrimary;

   logWriteLn("CPGIGraphicSystem::~CPGIGraphicSystem() - end", LOGDET_LOWEST);
}

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: seteaza variabilele de rezolutie
 parameters : resX, resY - rezolutia dorita
 others     : daca sunt 0 => rezolutia default
              returneaza true daca s-a schimbat modul
                         sau false daca a ramas acelasi mod
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::InitGraphics(/*HWND hWnd,*/ int resX, int resY, bool windowed)
{
   if (resX == 0)
   {
      resX = 640;
      resY = 480;
   }
   if (mResolutionX != resX && mResolutionY != resY)
   {
      mResolutionX = resX;
      mResolutionY = resY;
   }
   else
   {
      return false;
   }

   if (mWindowed)
   {
      set_color_depth(32);

      if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, mResolutionX, mResolutionY, 0, 0) != 0)
      {
          logWriteLn("Cannot set the windowed graphics mode.");
          return false;
      }

      mDDSPrimary->pimpl = screen;//create_video_bitmap(mResolutionX, mResolutionY);
      mDDSBack->pimpl = create_system_bitmap(mResolutionX, mResolutionY);

      set_clip_state(mDDSPrimary->pimpl, 1);
      set_clip_state(mDDSBack->pimpl, 1);
      set_clip_rect(mDDSBack->pimpl, 0, 0, mResolutionX - 1, mResolutionY - 1);
      set_clip_rect(screen, 0, 0, mResolutionX - 1, mResolutionY - 1);

      logWriteLn(CPString("back size: ") + CPString(mDDSBack->pimpl->w) + " x " + CPString(mDDSBack->pimpl->h));

      // TODO: set the video mode and create the back surface
      //mDDSPrimary->pimpl = SDL_SetVideoMode(mResolutionX, mResolutionY,
      //                                      16, SDL_HWSURFACE);
      //mDDSBack->pimpl = SDL_CreateRGBSurface(SDL_HWSURFACE,
      //                                       mResolutionX, mResolutionY,
      //                                       16, 0, 0, 0, 0);
   }
   else
   {
      set_color_depth(32);

      if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, mResolutionX, mResolutionY, 0, 0) != 0)
      {
          logWriteLn("Cannot set the fullscreen graphics mode.");
          return false;
      }

      mDDSPrimary->pimpl = screen;//create_video_bitmap(mResolutionX, mResolutionY);
      mDDSBack->pimpl = create_system_bitmap(mResolutionX, mResolutionY);

      // TODO: set the video mode
      //mDDSBack->pimpl = SDL_SetVideoMode(mResolutionX, mResolutionY, 16, SDL_HWSURFACE | SDL_DOUBLEBUF /*SDL_SWSURFACE*/);
      //mDDSPrimary->pimpl = mDDSBack->pimpl;
   }

   // TODO: init the internal color format
   //const SDL_PixelFormat* fmt = mDDSPrimary->pimpl->format;
   //InitColor(fmt->Rmask, fmt->Gmask, fmt->Bmask);

   return true;

}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returneaza suprafata unde se tipareste
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
---------------------------------------------------------------------------*/
CPGISurface *CPGIGraphicSystem::GetPaintableSurface(CPGISurface *Surface)
{
   if (Surface != NULL)
      return Surface;

   if (mDoubleBuffered)
      return mDDSBack;
   else
      return mDDSPrimary;
}
//---------------------------------------------------------------------------

unsigned int CPGIGraphicSystem::GetSurfaceWidth(CPGISurface* Surface)
{
   Surface = GetPaintableSurface(Surface);
   assert(Surface);
   return Surface->Width();
}
//---------------------------------------------------------------------------

unsigned int CPGIGraphicSystem::GetSurfaceHeight(CPGISurface* Surface)
{
   Surface = GetPaintableSurface(Surface);
   assert(Surface);
   return Surface->Height();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: takes a screenshot and save the file with the baseFileName + count
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::TakeScreenShot(CPString baseFileName)
{
   //#ifndef PPT_USE_SDL // TMN: Disabled during SDL conversion
   for (int i=0; i<5 - CPString(mScreenShotsCount).Length(); i++) {
      baseFileName = baseFileName + "0";
   }
   baseFileName = baseFileName + CPString(mScreenShotsCount) + CPString(".bmp");
   mScreenShotsCount ++;

   GBmpSave lBmpSave(baseFileName);

   // lock & save

   CPGISurface::SurfaceLocker locker(*mDDSPrimary);

   if (!lBmpSave.SaveAligned(
                             static_cast<unsigned char*>(mDDSPrimary->Bits()),
                             mResolutionX,
                             mResolutionY,
                             mDDSPrimary->Pitch(),
                             ApplicationBpp,
                             ApplicationBpp))
   {
     logWriteLn("CPGIGraphicSystem::TakeScreenShot - error saving image.");
     return;
   }

   /*
   for (int i=0; i<5 - CPString(mScreenShotsCount).Length(); i++)
      baseFileName = baseFileName + "0";
   baseFileName = baseFileName + CPString(mScreenShotsCount) + CPString(".bmp");
   mScreenShotsCount ++;

   Graphics::TBitmap *lBmp = new Graphics::TBitmap;
   lBmp->Width   = mResolutionX;
   lBmp->Height  = mResolutionY;

   HRESULT lHRet;
   HDC     hdc;
   if ((lHRet = mDDSPrimary->GetDC(&hdc)) == DD_OK)
   {
      BitBlt(lBmp->Canvas->Handle, 0, 0, lBmp->Width, lBmp->Height, hdc, 0, 0, SRCCOPY);
      mDDSPrimary->ReleaseDC(hdc);
   }
   lBmp->SaveToFile(baseFileName.c_str());
   delete lBmp;
   */
   //#endif	// PPT_USE_SDL
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
