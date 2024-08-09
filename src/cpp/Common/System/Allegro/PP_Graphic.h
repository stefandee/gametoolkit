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

//-----------------------------------------------------------------------------
//  Unit              : Graphic System
//
//  Version           : 1.1
//
//  Description       :
//    * interface with the video drivers
//
//  History           :
//    [13.07.2000] - [grabX]    - unit created
//    [19.02.2002] - [Tmn]      - added windowed mode
//-----------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 application: Pure Power Engine

 description: Graphic System Class
 last modify: 13 07 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#ifndef PP_GraphicH
#define PP_GraphicH

#include "PP_SpriteList.h"
#include "PP_Surface.h"
#include "PP_SurfaceList.h"
#include "PP_String.h"
#include "prims16.h"
#include "colors16.h"
#include "PP_Font.h"
#include "basic2d.h"

enum PGIModeType {PGI_FULLSCREEN, PGI_WINDOWED};
enum PGIList { SPRL_GENERIC, SPRL_MISSION };

class CPGISurface;
typedef CPGISprite* TPGISpriteReference;

class CPGIGraphicSystem
{
   public:
      void WriteSprites();

      class CErrorGeneric {};

   public:
      static CPGIGraphicSystem* Instance(int resX = 0, int resY = 0, PGIModeType mode = PGI_WINDOWED);
      static void
               Release();

      int      GetResX();    //inline
      int      GetResY();    //inline

      unsigned int GetSurfaceWidth(CPGISurface* Surface = 0);
      unsigned int GetSurfaceHeight(CPGISurface* Surface = 0);

      //graphics operations
      bool     ResetClipRegion(CPGISurface* Surface);
      bool     SetClipRegion(CPGISurface* Surface, CPRect clipRegion);

      void     PaintToScreen();
      void     PaintSpriteToSurface(CPGISurface *Surface, int whereX, int whereY,
                                    TPGISpriteReference Sprite, int animCount, int animType,
                                    int zoomFactor = 1, int alpha = 255);
      void     PaintSurfaceToSurface(CPGISurface *destSurface, CPRect Dest,
                                     CPGISurface *sourceSurface, CPRect Source, int alpha = 255);


      CPRect   GetPaintRect(CPPoint _where, TPGISpriteReference Sprite, int animCount, int animType, int zoomFactor = 1);

      void     Rectangle(CPGISurface* Surface, CPRect rect, PP_int32_t color);

      void     SelectionRectangle(CPGISurface* Surface, CPRect rect, int Length, PP_int32_t color);
      void     FillRectangle(CPGISurface* Surface, CPRect rect, PP_int32_t color);
      void     Line(CPGISurface* Surface, CPPoint ptStart, CPPoint ptEnd, PP_int32_t color);
      void     FillSurface(CPGISurface* Surface, PP_int32_t color);
      void     Circle(CPGISurface* Surface, CPPoint ptCenter, int Radius, PP_int32_t color);
      void     Pie(CPGISurface* Surface, CPPoint _center, int _radius, CPPoint _dir, int _angle, PP_int32_t _color);
      void     PutPixel(CPGISurface* Surface, CPPoint ptCenter, PP_int32_t color);
      PP_int32_t GetPixel(CPGISurface* Surface, CPPoint ptCenter);
      void     DrawText(CPGISurface *Surface, CPString text, CPRect ClipRect, int Format);
      void     TextOut(CPGISurface *Surface, int X, int Y, CPString text, unsigned int Alignment = 0);//TA_LEFT);

      void     SetColor(PP_int32_t Color);      //inline
      void     SetFont (CPFont *Font);        //inline

   public: // sprites related methods
     CPGISprite* AddSprite(CPString fileName, int wCells, int hCells, PGIList _list = SPRL_GENERIC);
     CPGISprite* AddSpriteCustom(int _width, int _height, int wCells, int hCells, PGIList _list = SPRL_GENERIC);
     int         GetSpriteIndex(CPGISprite* _sprite, PGIList _list = SPRL_GENERIC);
     CPGISprite* GetSpriteByIndex(int _index, PGIList _list = SPRL_GENERIC);

     void     ReleaseSprite(CPGISprite *Sprite, PGIList _list = SPRL_GENERIC);
     void     ReleaseAllSprites(PGIList _list = SPRL_GENERIC);
     void     RestoreSprites();
     int      GetSpriteCount(PGIList _list = SPRL_GENERIC);

     void     AlphaBlend16(CPGISurface*  dest,
                           CPGISurface*  source,
                           CPPoint       ptTarget,
                           CPRect        rcSrc,
                           unsigned char alpha);

      CPGISurface* GetPaintableSurface(CPGISurface* Surface);

   public: // screen Shots
     void     TakeScreenShot(CPString baseFileName);

   public: // surfaces related methods
     CPGISurface* CreateSurface(int Width = 640, int Height = 480, bool bSysMem = false);
     void         AddSurfaceRef(CPGISurface *Surface);                // neimplementata
     void         ReleaseSurface(CPGISurface *Surface);               // neimplementata

   protected:
      CPGIGraphicSystem(int resX, int resY);
      CPGIGraphicSystem(int resX, int resY, PGIModeType mode);
      ~CPGIGraphicSystem();

   private:
      //instanta
      static CPGIGraphicSystem
                           *mInstance;
      static int            mInstanceCount;

      //rezolutia curenta a instantei
      int                  mResolutionX;
      int                  mResolutionY;

      //surface-uri principale
      CPGISurface*         mDDSPrimary;
      CPGISurface*         mDDSBack;

      bool                 mDoubleBuffered;     // arata daca se foloseste back surface
      bool                 mWindowed;           // shows if the engine will run windowed

      //lista de sprite-uri
      CPGISpriteList       *mGenericSprites;
      CPGISpriteList       *mMissionSprites;

      int                  mScreenShotsCount;

      CPFont              *mFont;
      PP_int32_t           mColor;

      bool InitGraphics(int resX, int resY, bool windowed);
};

CPGIGraphicSystem* GetGraphicInstance(/*HWND hWnd = NULL,*/
                                      int resX = 0, int resY = 0,
                                      PGIModeType mode = PGI_WINDOWED);
//pentru eliminarea warningurilor

/////////////////////////////////////////////////////////////////////////////
//INLINE SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza rezolutiaX
---------------------------------------------------------------------------*/
inline int CPGIGraphicSystem::GetResX()
{
   return mResolutionX;
}
/*---------------------------------------------------------------------------
 description: returneaza rezolutiaY
---------------------------------------------------------------------------*/
inline int CPGIGraphicSystem::GetResY()
{
   return mResolutionY;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza culoarea
---------------------------------------------------------------------------*/
inline void CPGIGraphicSystem::SetColor(PP_int32_t Color)
{
//   mColor = makeRGB16(GetRValue(Color), GetGValue(Color), GetBValue(Color));
   mColor = Color;
}

/*---------------------------------------------------------------------------
 description: seteaza fontul
---------------------------------------------------------------------------*/
inline void CPGIGraphicSystem::SetFont (CPFont *Font)
{
//   DeleteObject(mFont);
   //delete mFont;

   mFont = Font;
}

#endif

