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

#include <SDL.h>
#include "PP_SpriteList.h"
#include "PP_Surface.h"
#include "PP_String.h"
#include "PP_Font.h"
#include "PP_Window.h"
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
      static CPGIGraphicSystem* Instance(int resX = 0, int resY = 0, PGIModeType mode = PGI_WINDOWED, std::string windowTitle = "No Title");
      static void
               Release();

      int      GetResX();    //inline
      int      GetResY();    //inline

      CPWindow* GetWindow(); // inline

      //graphics operations
      bool     ResetClipRegion();
      bool     SetClipRegion(CPRect clipRegion);

      void     BeginRender();
      void     EndRender();

      /*
      void     PaintSpriteToSurface(CPGISurface *Surface, int whereX, int whereY,
                                    TPGISpriteReference Sprite, int animCount, int animType,
                                    int zoomFactor = 1, int alpha = 255);
      void     PaintSurfaceToSurface(CPGISurface *destSurface, CPRect Dest,
                                     CPGISurface *sourceSurface, CPRect Source, int alpha = 255);
      */

      void PaintSprite(int whereX, int whereY, TPGISpriteReference Sprite, int animCount, int animType, int alpha = 255);
      void PaintTexture(int whereX, int whereY, CPGITexture* texture);


      void     Rectangle(CPRect rect);
      void     SelectionRectangle(CPRect rect, int Length);
      void     FillRectangle(CPRect rect);
      void     Line(CPPoint ptStart, CPPoint ptEnd);
      void     Circle(CPPoint ptCenter, int Radius);
      void     FillCircle(CPPoint ptCenter, int radius);
      void     Pie(CPPoint _center, int _radius, CPPoint _dir, int _angle);
      void     PutPixel(CPPoint ptCenter);
      void     HLine(int startX, int endX, int y);
      void     VLine(int startY, int endY, int x);
      void     FillTriangle(int p0x, int p0y, int p1x, int p1y, int p2x, int p2y);

      // stupid wingdi does #define TextOut TextOutA and then this fails to compile
      void     TextOut2(int X, int Y, CPString text, unsigned int Alignment = 0);//TA_LEFT);
      void     TextOutWrapped(int X, int Y, CPString text, int width);

      void     SetColor(TColor32 Color);      //inline
      void     SetFont (CPFont *Font);        //inline

     CPGITexture* CreateTexture(int Width, int Height);
     CPGITexture* CreateTextureFromFile(CPString fileName);

   public: // sprites related methods
     CPGISprite* AddSprite(CPString fileName, int wCells, int hCells, PGIList _list = SPRL_GENERIC);
     CPGISprite* AddSpriteCustom(int _width, int _height, int wCells, int hCells, PGIList _list = SPRL_GENERIC);
     int         GetSpriteIndex(CPGISprite* _sprite, PGIList _list = SPRL_GENERIC);
     CPGISprite* GetSpriteByIndex(int _index, PGIList _list = SPRL_GENERIC);

     void     ReleaseSprite(CPGISprite *Sprite, PGIList _list = SPRL_GENERIC);
     void     ReleaseAllSprites(PGIList _list = SPRL_GENERIC);
     void     RestoreSprites();
     int      GetSpriteCount(PGIList _list = SPRL_GENERIC);

   public:
        void TakeScreenShot(CPString fileName);

        /*
   public: // surfaces related methods
     CPGISurface* CreateSurface(int Width = 640, int Height = 480, bool bSysMem = false);
     void         AddSurfaceRef(CPGISurface *Surface);                // neimplementata
     void         ReleaseSurface(CPGISurface *Surface);               // neimplementata
     void      ConvertSurface(CPGISurface* src);
     */

   public:
     bool IsWindowActive();

   protected:
      CPGIGraphicSystem(int resX, int resY, std::string windowTitle);
      CPGIGraphicSystem(int resX, int resY, PGIModeType mode, std::string windowTitle);
      ~CPGIGraphicSystem();

   private:
      static CPGIGraphicSystem *mInstance; /**< singleton instance */
      static int            mInstanceCount;

      //rezolutia curenta a instantei
      int                  mResolutionX;
      int                  mResolutionY;

      bool                 mWindowed;           // shows if the engine will run windowed

      //lista de sprite-uri
      CPGISpriteList       *mGenericSprites;
      CPGISpriteList       *mMissionSprites;

      CPFont              *mFont;
      TColor32             mColor;

      // SDL2 related
      CPWindow*            mWindow;
      SDL_Renderer*        mRenderer;

      Uint32    mPixelFormat;

      bool InitGraphics(int resX, int resY, bool windowed, std::string windowTitle);

      friend class CPFont;
};

CPGIGraphicSystem* GetGraphicInstance(/*HWND hWnd = NULL,*/
                                      int resX = 0, int resY = 0,
                                      PGIModeType mode = PGI_WINDOWED,
                                      std::string windowTitle = "No Title");
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

inline CPWindow* CPGIGraphicSystem::GetWindow()
{
    return mWindow;
}
//---------------------------------------------------------------------------

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

