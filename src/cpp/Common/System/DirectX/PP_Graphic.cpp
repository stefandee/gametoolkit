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
//#include <vcl.h>
#include "math.h"
#include <io.h>
#pragma hdrstop

#include "easylogging++.h"
#include "PP_Graphic.h"
#include "GBmpLoad.h"
#include "HelpersWin32.h"
#include "basic3d.h"
#include "PP_Stdlib.h"

#ifdef _MSC_VER	// Standard workaround for Microsoft compiler bug
#define for if(0) {} else for
#endif

// A RAII helper class to reduce possibility of errors
class SurfaceLocker16bpp
{
public:
   SurfaceLocker16bpp(CPGISurface* surface)
   :   mSurface(surface)
   {
      mDDSD.dwSize = sizeof(mDDSD);
      mRes = surface->Lock(NULL, &mDDSD, DDLOCK_WAIT, NULL);
      if (FAILED(mRes))
      {
         LOG(ERROR) << CPGIGraphicSystem::ErrorDetailedStr(mRes);
         throw CPGIGraphicSystem::CErrorGeneric();
      }

      auto bitCount = mDDSD.ddpfPixelFormat.dwRGBBitCount;

      /*if (bitCount != 16)
      {
         // Reduce the chance of compiler errors (such as not calling
         // the d'tor when c'tor throws) leave the surface locked.
         LOG(ERROR) << "Surface pixel format is not 16 bit!";
         surface->Unlock(0);
         mRes = DDERR_GENERIC;
         throw CPGIGraphicSystem::CErrorGeneric();
      } */
   }
   ~SurfaceLocker16bpp()
   {
      if (!FAILED(mRes))
      {
         mSurface->Unlock(0);
      }
   }

   const DDSURFACEDESC_t& Desc() const { return mDDSD; }

private:
   SurfaceLocker16bpp(const SurfaceLocker16bpp&); // no impl.
   void operator=(const SurfaceLocker16bpp&);     // no impl.

   CPGISurface*    mSurface;
   HRESULT         mRes;
   DDSURFACEDESC_t mDDSD;
};
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
              se foloseste full-screen mode
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
 others     : pentru eliminarea warningurilor
---------------------------------------------------------------------------*/
/*
CPGIGraphicSystem* GetGraphicInstance(HWND hWnd, int resX, int resY)
{
  return CPGIGraphicSystem::Instance(hWnd, resX, resY);
}
//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: pentru returnarea instantei de Graphic System
              se transmite explicit modul dorit
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
              mode - arata modul dorit
 others     : pentru eliminarea warningurilor
---------------------------------------------------------------------------*/
CPGIGraphicSystem* GetGraphicInstance(HWND hWnd,
                                      int resX, int resY,
                                      PGIModeType mode)
{
   return CPGIGraphicSystem::Instance(hWnd, resX, resY, mode);
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: creaza o instanta daca nu exista sau returneaza this daca ea exista
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
---------------------------------------------------------------------------*/
/*
CPGIGraphicSystem* CPGIGraphicSystem::Instance(HWND hwnd, int resX, int resY)
{
   if (mInstance == NULL)
   {
      mInstance = new CPGIGraphicSystem(hwnd, resX, resY);
   }
   else
   {
//      if (hwnd != 0)
//      {
//         mInstance->Release();
//         mInstance->InitGraphics(hwnd, resX, resY);
//      }
   }
   return mInstance;
}

//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: creaza o instanta daca nu exista sau returneaza this daca ea exista
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
---------------------------------------------------------------------------*/
CPGIGraphicSystem* CPGIGraphicSystem::Instance(HWND hwnd,
                                               int resX, int resY,
                                               PGIModeType mode)
{
   if (mInstance == NULL)
   {
      mInstance = new CPGIGraphicSystem(hwnd, resX, resY, mode);
   }
   /*
   else
   {
      if (hwnd != 0)
      {
         mInstance->Release();
         mInstance->InitGraphics(hwnd, resX, resY, mode);
      }
   }
   */

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
      mInstance = NULL;
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the cooperative level to another window support
 parameters : Handle - the window handle
              Flags  - the flags of caps
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::SetCooperativeLevel(HWND hWnd, DWORD Flags)
{
   HRESULT lHRet;
   lHRet = mDD->SetCooperativeLevel(hWnd, Flags);
   if (lHRet != DD_OK && lHRet != DDERR_HWNDALREADYSET)
   {
      LOG(ERROR) << ErrorDetailedStr(lHRet);
      throw CErrorGeneric();
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza clip region
 parameters : ClipRegion - rect
              Surface - daca este NULL => se seteaza back sau in primary dupa
                        mDoubleBuffered
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::SetClipRegion(CPGISurface **Surface, CPRect clipRegion)
{
   CPGISurface *lSurface;
   if (Surface == NULL)
      lSurface = mDDSBack;
   else
      lSurface = *Surface;
   HRESULT hRet;
   LPDIRECTDRAWCLIPPER pclip;
   HRGN hrgn;
   BYTE rgnDataBuffer[1024];


   if (clipRegion.left < 0)
      clipRegion.left = 0;
   if (clipRegion.left >= GetResX())
      clipRegion.left = GetResX()-1;

   if (clipRegion.top < 0)
      clipRegion.top = 0;
   if (clipRegion.top >= GetResY() + 1)
      clipRegion.top = GetResY();

   if (clipRegion.right <= clipRegion.left)
      clipRegion.right = clipRegion.left + 1;
   if (clipRegion.right >= GetResX()+1)
      clipRegion.right = GetResX();

   if (clipRegion.bottom <= clipRegion.top)
      clipRegion.bottom = clipRegion.top + 1;
   if (clipRegion.bottom >= GetResY()+1)
      clipRegion.bottom = GetResY();

   //for karg's prims
   setClipRegion(clipRegion.left, clipRegion.top, clipRegion.right - 1, clipRegion.bottom - 1);

   hrgn = CreateRectRgn(clipRegion.left, clipRegion.top, clipRegion.right, clipRegion.bottom);
   GetRegionData(hrgn, sizeof(rgnDataBuffer), (RGNDATA*)rgnDataBuffer);
   if ((hRet = mDD->CreateClipper(0, &pclip, 0)) != DD_OK)
   {
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   if ((hRet = pclip->SetClipList((RGNDATA*)rgnDataBuffer, 0)) != DD_OK)
   {
      pclip->Release();
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   if ((hRet = (lSurface)->SetClipper(pclip)) != DD_OK)
   {
      pclip->Release();
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   pclip->Release();
   DeleteObject(hrgn);
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza clipul original, adica cat toata suprafata
 parameters : Surface - daca este NULL => se seteaza back sau in primary dupa
                        mDoubleBuffered
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::ResetClipRegion(CPGISurface **Surface)
{
   CPGISurface *lSurface;
   if (Surface == NULL)
      lSurface = mDDSBack;
   else
      lSurface = *Surface;
   DDSURFACEDESC_t     ddsd;
   HRESULT             hRet;
   LPDIRECTDRAWCLIPPER pclip;
   HRGN hrgn;
   BYTE rgnDataBuffer[1024];

   ZeroMemory(&ddsd,sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);
   if ((hRet = (lSurface)->GetSurfaceDesc(&ddsd)) != DD_OK)
   {
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   hrgn = CreateRectRgn(0, 0, ddsd.dwWidth, ddsd.dwHeight);
   GetRegionData(hrgn, sizeof(rgnDataBuffer), (RGNDATA*)rgnDataBuffer);
   if ((hRet = mDD->CreateClipper(0, &pclip, 0)) != DD_OK)
   {
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   if ((hRet = pclip->SetClipList((RGNDATA*)rgnDataBuffer, 0)) != DD_OK)
   {
      pclip->Release();
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   if ((hRet = (lSurface)->SetClipper(pclip)) != DD_OK)
   {
      pclip->Release();
      LOG(ERROR) << ErrorDetailedStr(hRet);
      return false;
   }
   pclip->Release();
   DeleteObject(hrgn);

   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adica face flip daca este double buffered
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::PaintToScreen()
{
   HRESULT lHRet;
   while (TRUE)
   {
      if (mWindowed)
      {
         // blit stuff - written by TMN, added by Karg
         RECT rcDest;
         RECT rcSrc  = { 0, 0, mResolutionX-1, mResolutionY-1 };

         if (GetWindowRect(mHandle, &rcDest) == 0)
         {
           lHRet = DD_OK;
            LPVOID lpMsgBuf;

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0,
                NULL
            );

            // Display the string.
            VLOG(9) << "GetWindowRect: " << (char*)lpMsgBuf;

            // Free the buffer.
            LocalFree( lpMsgBuf );
//           break;
         }

         rcDest.top += GetSystemMetrics(SM_CYCAPTION);
         rcDest.right = rcDest.left + mResolutionX;
         rcDest.bottom = rcDest.top + mResolutionY;
         DDBLTFX bltfx = { sizeof(bltfx), 0 };

         //Rectangle(mDDSPrimary, CPRect(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom), 0xFF00FF);
         lHRet = mDDSPrimary->Blt(&rcDest, mDDSBack, &rcSrc, 0, &bltfx);
         if (lHRet != DD_OK)
         {
            VLOG(9) << "To primary blt:" << ErrorDetailedStr(lHRet);

            if (lHRet == DDERR_SURFACELOST)
            {
               lHRet = mDDSPrimary->Restore();
               if (lHRet != DD_OK )
                  break;

               lHRet = mDDSBack->Restore();
               if (lHRet != DD_OK )
                  break;
            }
         }

         // TMN: Lie a bit and say everything went fine
         lHRet = DD_OK;
      }
      else
      {
         // fullscreen mode, so perform a flip.
         lHRet = mDDSPrimary->Flip( NULL, 0L );
      }
      if (lHRet == DD_OK)
         break;
      if (lHRet == DDERR_SURFACELOST)
      {
         lHRet = mDDSPrimary->Restore();
         if (lHRet != DD_OK )
            break;

         lHRet = mDDSBack->Restore();
         if (lHRet != DD_OK )
            break;
      }
      if (lHRet != DDERR_WASSTILLDRAWING)
         break;
   }
   if (lHRet != DD_OK)
   {
      LOG(ERROR) << ErrorDetailedStr(lHRet);
      throw CErrorGeneric();
   }
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
   lDest       = CPRect(whereX, whereY, whereX + (lSource.right - lSource.left), whereY + lSource.bottom - lSource.top);

   bool lResult = PaintSurfaceToSurface(Surface, lDest, lSpriteSurf, lSource, alpha);

   // false means sprite surface was lost
   if (!lResult)
   {
     Sprite->Restore();
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: aka BLT
 parameters : destSurface   - the destination surface
              Dest          - the destination rectangle
              sourceSurface - the source surface (where to copy from)
              Source        - the source rectangle
              Alpha         - the alpha blending value (no range checking here)
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::PaintSurfaceToSurface(CPGISurface *destSurface, CPRect Dest,
                                              CPGISurface *sourceSurface, CPRect Source,
                                              int alpha)
{
   destSurface = GetPaintableSurface(destSurface);
   if (sourceSurface == NULL)
   {
      LOG(ERROR) << "CPGIGraphicSystem::PaintSurfaceToSurface - Source Surface is NULL !!!!!!!!!!";
      return true;
   }
   HRESULT lHRet;
   while (true)
   {
      tagRECT lDest, lSource;

      lDest.left = Dest.left; lDest.right = Dest.right; lDest.top = Dest.top; lDest.bottom = Dest.bottom;
      lSource.left = Source.left; lSource.right = Source.right; lSource.top = Source.top; lSource.bottom = Source.bottom;

      DDBLTFX lBltFx;

	  // TODO: this seems not to work on directx7 surfaces...or my videocard is a total crap
      ZeroMemory(&lBltFx, sizeof(DDBLTFX));
      lBltFx.dwSize                  = sizeof(lBltFx);
      lBltFx.dwAlphaSrcConstBitDepth = 8;
      lBltFx.dwAlphaSrcConst         = alpha;
      //lBltFx.dwAlphaDestConstBitDepth = 8;
      //lBltFx.dwAlphaDestConst = 63;

      lHRet = destSurface->Blt(&lDest, sourceSurface, &lSource, DDBLT_ALPHASRCCONSTOVERRIDE | DDBLT_KEYSRC, &lBltFx);

      if (lHRet == DD_OK)
	  {
         break;
	  }

      if (lHRet == DDERR_SURFACELOST)
      {
         // What to do? For now just return...
         destSurface->Restore();
         //sourceSurface->Restore();

         //mDDSPrimary->Restore();
         //mDDSBack->Restore();
         //RestoreSprites();
         //logWriteLn("surface lost.");
         return false;
      }

	  if (lHRet == DDERR_NOALPHAHW)
	  {
		  LOG(WARNING) << "No hardware alpha support.";
		  return false;
	  }

      /*
      if (lHRet != DDERR_WASSTILLDRAWING)
      {
         logWriteLn(ErrorDetailedStr(lHRet));
         throw CErrorGeneric();
      }
      */
   }

   return true;
}
//---------------------------------------------------------------------------

CPRect CPGIGraphicSystem::GetPaintRect(CPPoint _where, TPGISpriteReference Sprite, int animCount, int animType, int zoomFactor)
{
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
void CPGIGraphicSystem::Rectangle(CPGISurface* Surface, CPRect rect, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   color = makeRGB16(GetRValue(color), GetGValue(color), GetBValue(color));
   frameRect(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
             rect.left, rect.top, rect.right-1, rect.bottom-1, static_cast<unsigned __int16>(color));
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
void CPGIGraphicSystem::SelectionRectangle(CPGISurface* Surface, CPRect rect, int Length, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   color = makeRGB16(GetRValue(color), GetGValue(color), GetBValue(color));
   selRect(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch, rect.left, rect.top, rect.right, rect.bottom, Length, static_cast<unsigned __int16>(color));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza un dreptunghi plin cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::FillRectangle(CPGISurface* Surface, CPRect rect, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   color = makeRGB16(GetRValue(color), GetGValue(color), GetBValue(color));
   fillRect(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch, rect.left, rect.top, rect.right, rect.bottom, static_cast<unsigned __int16>(color));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza o linie cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Line(CPGISurface* Surface, CPPoint ptStart, CPPoint ptEnd, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);
   HDC   hdc;
   HRESULT lHRet;
   if ((lHRet = Surface->GetDC(&hdc)) == DD_OK)
   {
      SelectObject(hdc, mPen);
      MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
      LineTo(hdc, ptEnd.x, ptEnd.y);
      Surface->ReleaseDC(hdc);
   }
   else
   {
      LOG(ERROR) << ErrorDetailedStr(lHRet);
      throw CErrorGeneric();
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: umple toata suprafata cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::FillSurface(CPGISurface* Surface, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);

   DDBLTFX lDDBltFx;
   HRESULT lHRet;
   lDDBltFx.dwSize = sizeof(lDDBltFx);
   lDDBltFx.dwFillColor = color;
   while (true)
   {
      lHRet = Surface->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL,&lDDBltFx);
      if (lHRet == DD_OK )
         break;
      if (lHRet != DDERR_WASSTILLDRAWING)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }
   }
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
void CPGIGraphicSystem::Circle(CPGISurface* Surface, CPPoint ptCenter, int Radius, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   color = makeRGB16(GetRValue(color), GetGValue(color), GetBValue(color));
   ::Circle(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
             ptCenter.x, ptCenter.y,
             Radius,
             static_cast<unsigned __int16>(color));
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
void CPGIGraphicSystem::Pie(CPGISurface* Surface, CPPoint _center, int _radius, CPPoint _dir, int _angle, COLORREF _color)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   // make the color
   _color = makeRGB16(GetRValue(_color), GetGValue(_color), GetBValue(_color));

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
   ::Triangle(static_cast<__int8*>(lDDSD.lpSurface),
              lDDSD.lPitch,
              _center.x + x1, _center.y + y1,
              _center.x + x2, _center.y + y2,
              _center.x, _center.y,
              static_cast<unsigned __int16>(_color)
             );

   //Line(Surface, CPPoint(_center.x + x1, _center.y + y1), CPPoint(_center.x + x2, _center.y + y2), _color);
   //Line(Surface, CPPoint(_center.x + x1, _center.y + y1), CPPoint(_center.x, _center.y), _color);
   //Line(Surface, CPPoint(_center.x + x2, _center.y + y2), CPPoint(_center.x, _center.y), _color);

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

       /*
       hLine(static_cast<char*>(Surface->Bits()), Surface->Pitch(),
             _center.x + x,
             _center.x + lSign * x0,
             _center.y + y,
             static_cast<PP_uint16_t>(_color)
             );
       */
       hLine(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
             _center.x + x,
             _center.x + lSign * x0,
             _center.y + y,
             static_cast<unsigned __int16>(_color)
             );

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

     /*
     vLine(static_cast<char*>(Surface->Bits()), Surface->Pitch(),
           _center.x + x,
           _center.y + lSign * y0,
           _center.y + y,
           static_cast<PP_uint16_t>(_color)
           );
     */      
     vLine(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
           _center.x + x,
           _center.y + lSign * y0,
           _center.y + y,
           static_cast<unsigned __int16>(_color)
           );

     x++;
     y += lM;
   }

   /*
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

       hLine(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
             _center.x + x,
             _center.x + lSign * x0,
             _center.y + y,
             static_cast<unsigned __int16>(_color)
             );

       y++;
       x  += lM;
     }

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

     vLine(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
           _center.x + x,
           _center.y + lSign * y0,
           _center.y + y,
           static_cast<unsigned __int16>(_color)
           );

     x++;
     y += lM;
   }
   */
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
void CPGIGraphicSystem::PutPixel(CPGISurface* Surface, CPPoint ptCenter, COLORREF color)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   color = makeRGB16(GetRValue(color), GetGValue(color), GetBValue(color));
   ::Circle(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
             ptCenter.x, ptCenter.y,
             1,
			 static_cast<unsigned __int16>(color));
//   putPixel((BYTE*)lDDSD.lpSurface, lDDSD.lPitch,
//             ptCenter.x, ptCenter.y,
//             color);
}
//---------------------------------------------------------------------------

COLORREF CPGIGraphicSystem::GetPixel(CPGISurface* Surface, CPPoint ptCenter)
{
   Surface = GetPaintableSurface(Surface);

   SurfaceLocker16bpp locker(Surface);

   const DDSURFACEDESC_t& lDDSD = locker.Desc();

   return    ::getPixel(static_cast<__int8*>(lDDSD.lpSurface), lDDSD.lPitch,
             ptCenter.x, ptCenter.y);
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
   HDC   hdc;
   HRESULT lHRet;
   Surface = GetPaintableSurface(Surface);
   lHRet = Surface->GetDC(&hdc);
   if (lHRet == DD_OK)
   {
      tagRECT lClipRect;

      lClipRect.left = ClipRect.left; lClipRect.right = ClipRect.right;
      lClipRect.top = ClipRect.top; lClipRect.bottom = ClipRect.bottom;

      SelectObject(hdc, mFont->GetFontHandle());
      SetBkMode(hdc, TRANSPARENT);
      SetTextColor(hdc, mColor);      //adica alb

      std::wstring textW = StringToWString(Text.c_str());

      ::DrawText(hdc, textW.c_str(), textW.length(), &lClipRect, Format);
      Surface->ReleaseDC(hdc);
   }
   else
   {
      LOG(ERROR) << ErrorDetailedStr(lHRet);
      throw CErrorGeneric();
   }
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
void CPGIGraphicSystem::TextOut(CPGISurface *Surface, int X, int Y, CPString Text, UINT Alignment)
{
   HDC   hdc;
   HRESULT lHRet;
   Surface = GetPaintableSurface(Surface);
   lHRet = Surface->GetDC(&hdc);
   if (lHRet == DD_OK)
   {
      SelectObject(hdc, mFont->GetFontHandle());
      SetBkMode(hdc, TRANSPARENT);
      SetTextColor(hdc, mColor);      //adica alb
      SetTextAlign(hdc, Alignment);

      std::wstring textW = StringToWString(Text.c_str());

      ::TextOut(hdc, X, Y, textW.c_str(), textW.length());

      Surface->ReleaseDC(hdc);
   }
   else
   {
      LOG(ERROR) << ErrorDetailedStr(lHRet);
      throw CErrorGeneric();
   }
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
  CPGISpriteList* lSpriteList = mGenericSprites;

  for(int i = 0; i < lSpriteList->noOfSprites; i++)
  {
    lSpriteList->GetSprite(i)->Restore();
  }

  lSpriteList = mMissionSprites;
  for(int i = 0; i < lSpriteList->noOfSprites; i++)
  {
    lSpriteList->GetSprite(i)->Restore();
  }
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
CPGISurface* CPGIGraphicSystem::CreateSurface(int Width, int Height)
{
   CPGISurface*    lSurface;
   DDSURFACEDESC_t lDDSD = { 0 };
   HRESULT         lHRet;

   //aici urmeaza alocare pentru DESCRIPTOR in memoria video
   ZeroMemory(&lDDSD, sizeof(lDDSD));
   lDDSD.dwSize         = sizeof( lDDSD );
   lDDSD.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
   lDDSD.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
   lDDSD.dwWidth        = Width;
   lDDSD.dwHeight       = Height;

   lHRet = PrivateCreateSurface(lDDSD, lSurface);

   if (lHRet != DD_OK)
   {   // Try allocating the surface in system memory
      lDDSD.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

      if ((lHRet = PrivateCreateSurface(lDDSD, lSurface))!= DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }
   }

   DDCOLORKEY lDDCK;
   lDDCK.dwColorSpaceLowValue = 0;
   lDDCK.dwColorSpaceHighValue = lDDCK.dwColorSpaceLowValue;
   lSurface->SetColorKey(DDCKEY_SRCBLT, &lDDCK);

   ResetClipRegion(&lSurface);

   return lSurface;

//   return MGM_CreateSurface();
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
CPGIGraphicSystem::CPGIGraphicSystem(HWND hwnd, int resX, int resY, PGIModeType mode)
{
   mResolutionX = 0;//resX;
   mResolutionY = 0;//resY;
   //mResolutionX = 1024;
   //mResolutionY = 768;

   mDD         = NULL;
   mDDSPrimary = NULL;
   mDDSBack    = NULL;

   mGenericSprites = new CPGISpriteList();
   mMissionSprites = new CPGISpriteList();

   mScreenShotsCount = 0;

   mColor      = 0x00FFFFFF;
   mPen        = CreatePen(PS_SOLID, 0, mColor);
   mFont       = new CPFont("Small Fonts");

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

   InitGraphics(hwnd, resX, resY, mWindowed);
}
//---------------------------------------------------------------------------

CPGIGraphicSystem::CPGIGraphicSystem(HWND hwnd, int resX, int resY)
{
   mResolutionX = 0;
   mResolutionY = 0;

   mDD         = NULL;
   mDDSPrimary = NULL;
   mDDSBack    = NULL;

   mGenericSprites = new CPGISpriteList();
   mMissionSprites = new CPGISpriteList();

   mScreenShotsCount = 0;

   mColor      = 0x00FFFFFF;
   mPen        = CreatePen(PS_SOLID, 0, mColor);
   mFont       = new CPFont("Small Fonts");

   mWindowed         = false;
   mDoubleBuffered   = true;

   InitGraphics(hwnd, resX, resY, mWindowed);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructorul clasei
 others     : elibereaza obiectele DX si lista de sprite-uri
---------------------------------------------------------------------------*/
CPGIGraphicSystem::~CPGIGraphicSystem()
{
   DeleteObject(mPen);
   VLOG(9) << "CPGIGraphicSystem::~CPGIGraphicSystem()";

   if (mGenericSprites != NULL)
   {
     delete mGenericSprites;
   }

   if (mMissionSprites != NULL)
   {
     delete mMissionSprites;
   }

   VLOG(9) << "CPGIGraphicSystem::~CPGIGraphicSystem() - sprites deleted";
   if (mDDSBack != NULL)
   {
      mDDSBack->Release();
      mDDSBack = NULL;
   }
   if (mDDSPrimary != NULL)
   {
      mDDSPrimary->Release();
      mDDSPrimary = NULL;
   }
   if (mDD != NULL)
   {
      mDD->Release();
      mDD = NULL;
   }
   VLOG(9) << "CPGIGraphicSystem::~CPGIGraphicSystem() - end";
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
bool CPGIGraphicSystem::InitGraphics(HWND hWnd, int resX, int resY, bool windowed)
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

   //mResolutionX = 1024;
   //mResolutionY = 768;


   //initializarea Direct Draw
   HRESULT lHRet;
#if DIRECTDRAW_VERSION >= 0x0700
   lHRet = DirectDrawCreateEx(NULL, (VOID**)&mDD, IID_IDirectDraw7, NULL);
#else
   LPDIRECTDRAW pDD1;
   lHRet = DirectDrawCreate(NULL, reinterpret_cast<LPDIRECTDRAW FAR*>(&pDD1), NULL);
   if (lHRet != DD_OK) {
      logWriteLn(ErrorDetailedStr(lHRet));
      return false;
   }
   lHRet = pDD1->QueryInterface(IID_IDirectDraw2, reinterpret_cast<LPVOID FAR*>(&mDD));
   pDD1->Release();
#endif
   if (lHRet != DD_OK)
   {
      LOG(ERROR) << ErrorDetailedStr(lHRet);
      return false;
   }

   //initializarea Suprafetelor
   DDSURFACEDESC_t     lDDSD = { 0 };
   LPDIRECTDRAWCLIPPER lClipper;
   DDPIXELFORMAT       lDDPF;

   if (mWindowed)
   {// Windowed mode
      lHRet = mDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      // Create the primary surface
      ZeroMemory( &lDDSD, sizeof( lDDSD ) );
      lDDSD.dwSize = sizeof(lDDSD);
      lDDSD.dwFlags = DDSD_CAPS;
      lDDSD.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        DDPIXELFORMAT ddpf;
        ZeroMemory(&ddpf, sizeof(DDPIXELFORMAT));
        ddpf.dwSize = sizeof(DDPIXELFORMAT);
        ddpf.dwFlags = DDPF_RGB;
        ddpf.dwRGBBitCount = 16;
        ddpf.dwRBitMask = 0x0000F800;
        ddpf.dwGBitMask = 0x000007E0;
        ddpf.dwBBitMask = 0x0000001F;

      lDDSD.ddpfPixelFormat = ddpf;

      lHRet = PrivateCreateSurface(lDDSD, mDDSPrimary);

      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      // Create a clipper object since this is for a Windowed render
      lHRet = mDD->CreateClipper(0, &lClipper, NULL);
      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }
      
      // Associate the clipper with the window
      lClipper->SetHWnd(0, hWnd);
      mDDSPrimary->SetClipper(lClipper);
      lClipper->Release();
      lClipper = NULL;

      // Create the back surface
      ZeroMemory( &lDDSD, sizeof( lDDSD ) );
      lDDSD.dwSize         = sizeof(lDDSD);
      lDDSD.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
      lDDSD.dwWidth        = mResolutionX;
      lDDSD.dwHeight       = mResolutionY;
      //lDDSD.dwWidth        = 640;
      //lDDSD.dwHeight       = 480;
      lDDSD.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

        ZeroMemory(&ddpf, sizeof(DDPIXELFORMAT));
        ddpf.dwSize = sizeof(DDPIXELFORMAT);
        ddpf.dwFlags = DDPF_RGB;
        ddpf.dwRGBBitCount = 16;
        ddpf.dwRBitMask = 0x0000F800;
        ddpf.dwGBitMask = 0x000007E0;
        ddpf.dwBBitMask = 0x0000001F;

      lDDSD.ddpfPixelFormat = ddpf;

      lHRet = PrivateCreateSurface(lDDSD, mDDSBack);

      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      //if( SetClipRegion(&mDDSBack, CPRect(0, 0, 639, 479)) == false)
      if( SetClipRegion(&mDDSBack, CPRect(0, 0, mResolutionX-1, mResolutionY-1)) == false)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      lDDPF.dwSize = sizeof(lDDPF);
      mDDSPrimary->GetPixelFormat(&lDDPF);

      auto dwRGBBitCount = lDDPF.dwRGBBitCount;
      auto dwRBitMask = ddpf.dwRBitMask;
      auto dwGBitMask = ddpf.dwGBitMask;
      auto dwBBitMask = ddpf.dwBBitMask;

      if (!(lDDPF.dwFlags & DDPF_RGB))
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }
      InitColor(lDDPF.dwRBitMask, lDDPF.dwGBitMask, lDDPF.dwBBitMask);
    }
   else
   {// Full screen mode
      lHRet = mDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE |
                                             DDSCL_FULLSCREEN|
                                             DDSCL_ALLOWREBOOT);
      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
//         throw CErrorGeneric();
      }

      // Set the video mode to 640x480x16 - by default
#if DIRECTDRAW_VERSION >= 0x0900
//      lHRet = mDD->SetDisplayMode(resX, resY, 16);
      lHRet = mDD->SetDisplayMode(resX, resY, 16, 0, 0);
#else
      lHRet = mDD->SetDisplayMode(resX, resY, 16, 0, 0);
#endif
      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      // Create the primary surface with 1 back buffer
      lDDSD.dwSize = sizeof(lDDSD);
      lDDSD.dwFlags = DDSD_CAPS |
                    DDSD_BACKBUFFERCOUNT;
      lDDSD.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                       DDSCAPS_FLIP |
                       DDSCAPS_COMPLEX;
      lDDSD.dwBackBufferCount = 1;

      lHRet = PrivateCreateSurface(lDDSD, mDDSPrimary);

      if (lHRet != DD_OK)
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      DDSCAPS_t lDDSDCaps = { 0 };
      lDDSDCaps.dwCaps = DDSCAPS_BACKBUFFER;
      lHRet = mDDSPrimary->GetAttachedSurface(&lDDSDCaps, &mDDSBack);
      if (lHRet != DD_OK)
      {
         LOG(ERROR) << "mDDSPrimary->GetAttachedSurface(&lDDSDCaps, &mDDSBack): " << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }
      if (SetClipRegion(NULL, CPRect(0, 0, resX, resY)) == false)
      {
         LOG(ERROR) << "SetClipRegion(NULL, CPRect(0, 0, resX, resY): " << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }

      lDDPF.dwSize = sizeof(lDDPF);
      mDDSPrimary->GetPixelFormat(&lDDPF);
      if (!(lDDPF.dwFlags & DDPF_RGB))
      {
         LOG(ERROR) << ErrorDetailedStr(lHRet);
         throw CErrorGeneric();
      }
      InitColor(lDDPF.dwRBitMask, lDDPF.dwGBitMask, lDDPF.dwBBitMask);
   }

   mHandle = hWnd;

   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: returneaza char* cu eroarea
 parameters: hErr - eroarea in cauza
             text - additional text
---------------------------------------------------------------------------*/
CPString CPGIGraphicSystem::ErrorDetailedStr(HRESULT hErr)
{
	const char* lDDErr;

	switch (hErr)
   {
		case DDERR_ALREADYINITIALIZED :     lDDErr = "DDERR_ALREADYINITIALIZED"; break;
		case DDERR_CANNOTATTACHSURFACE :    lDDErr = "DDERR_CANNOTATTACHSURFACE"; break;
		case DDERR_CANNOTDETACHSURFACE :    lDDErr = "DDERR_CANNOTDETACHSURFACE"; break;
		case DDERR_CURRENTLYNOTAVAIL :      lDDErr = "DDERR_CURRENTLYNOTAVAIL"; break;
		case DDERR_EXCEPTION :              lDDErr = "DDERR_EXCEPTION"; break;
		case DDERR_GENERIC :                lDDErr = "DDERR_GENERIC"; break;
		case DDERR_HEIGHTALIGN :            lDDErr = "DDERR_HEIGHTALIGN"; break;
		case DDERR_INCOMPATIBLEPRIMARY :    lDDErr = "DDERR_INCOMPATIBLEPRIMARY"; break;
		case DDERR_INVALIDCAPS :            lDDErr = "DDERR_INVALIDCAPS"; break;
		case DDERR_INVALIDCLIPLIST :        lDDErr = "DDERR_INVALIDCLIPLIST"; break;
		case DDERR_INVALIDMODE :            lDDErr = "DDERR_INVALIDMODE"; break;
		case DDERR_INVALIDOBJECT :          lDDErr = "DDERR_INVALIDOBJECT"; break;
		case DDERR_INVALIDPARAMS :          lDDErr = "DDERR_INVALIDPARAMS"; break;
		case DDERR_INVALIDPIXELFORMAT :     lDDErr = "DDERR_INVALIDPIXELFORMAT"; break;
		case DDERR_INVALIDRECT :            lDDErr = "DDERR_INVALIDRECT"; break;
		case DDERR_LOCKEDSURFACES :         lDDErr = "DDERR_LOCKEDSURFACES"; break;
		case DDERR_NO3D :                   lDDErr = "DDERR_NO3D"; break;
		case DDERR_NOALPHAHW :              lDDErr = "DDERR_NOALPHAHW"; break;
		case DDERR_NOCLIPLIST :             lDDErr = "DDERR_NOCLIPLIST"; break;
		case DDERR_NOCOLORCONVHW :          lDDErr = "DDERR_NOCOLORCONVHW"; break;
		case DDERR_NOCOOPERATIVELEVELSET :  lDDErr = "DDERR_NOCOOPERATIVELEVELSET"; break;
		case DDERR_NOCOLORKEY :             lDDErr = "DDERR_NOCOLORKEY"; break;
		case DDERR_NOCOLORKEYHW :           lDDErr = "DDERR_NOCOLORKEYHW"; break;
		case DDERR_NODIRECTDRAWSUPPORT :    lDDErr = "DDERR_NODIRECTDRAWSUPPORT"; break;
		case DDERR_NOEXCLUSIVEMODE :        lDDErr = "DDERR_NOEXCLUSIVEMODE"; break;
		case DDERR_NOFLIPHW :               lDDErr = "DDERR_NOFLIPHW"; break;
		case DDERR_NOGDI :                  lDDErr = "DDERR_NOGDI"; break;
		case DDERR_NOMIRRORHW :             lDDErr = "DDERR_NOMIRRORHW"; break;
		case DDERR_NOTFOUND :               lDDErr = "DDERR_NOTFOUND"; break;
		case DDERR_NOOVERLAYHW :            lDDErr = "DDERR_NOOVERLAYHW"; break;
		case DDERR_NORASTEROPHW :           lDDErr = "DDERR_NORASTEROPHW"; break;
		case DDERR_NOROTATIONHW :           lDDErr = "DDERR_NOROTATIONHW"; break;
		case DDERR_NOSTRETCHHW :            lDDErr = "DDERR_NOSTRETCHHW"; break;
		case DDERR_NOT4BITCOLOR :           lDDErr = "DDERR_NOT4BITCOLOR"; break;
		case DDERR_NOT4BITCOLORINDEX :      lDDErr = "DDERR_NOT4BITCOLORINDEX"; break;
		case DDERR_NOT8BITCOLOR :           lDDErr = "DDERR_NOT8BITCOLOR"; break;
		case DDERR_NOTEXTUREHW :            lDDErr = "DDERR_NOTEXTUREHW"; break;
		case DDERR_NOVSYNCHW :              lDDErr = "DDERR_NOVSYNCHW"; break;
		case DDERR_NOZBUFFERHW :            lDDErr = "DDERR_NOZBUFFERHW"; break;
		case DDERR_NOZOVERLAYHW :           lDDErr = "DDERR_NOZOVERLAYHW"; break;
		case DDERR_OUTOFCAPS :              lDDErr = "DDERR_OUTOFCAPS"; break;
		case DDERR_OUTOFMEMORY :            lDDErr = "DDERR_OUTOFMEMORY"; break;
		case DDERR_OUTOFVIDEOMEMORY :       lDDErr = "DDERR_OUTOFVIDEOMEMORY"; break;
		case DDERR_OVERLAYCANTCLIP :        lDDErr = "DDERR_OVERLAYCANTCLIP"; break;
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE : lDDErr = "DDERR_OVERLAYCOLORKEYONLYONEACTIVE"; break;
		case DDERR_PALETTEBUSY :            lDDErr = "DDERR_PALETTEBUSY"; break;
		case DDERR_COLORKEYNOTSET :         lDDErr = "DDERR_COLORKEYNOTSET"; break;
		case DDERR_SURFACEALREADYATTACHED : lDDErr = "DDERR_SURFACEALREADYATTACHED"; break;
		case DDERR_SURFACEALREADYDEPENDENT :lDDErr = "DDERR_SURFACEALREADYDEPENDENT"; break;
		case DDERR_SURFACEBUSY :            lDDErr = "DDERR_SURFACEBUSY"; break;
		case DDERR_CANTLOCKSURFACE :        lDDErr = "DDERR_CANTLOCKSURFACE"; break;
		case DDERR_SURFACEISOBSCURED :      lDDErr = "DDERR_SURFACEISOBSCURED"; break;
		case DDERR_SURFACELOST :            lDDErr = "DDERR_SURFACELOST"; break;
		case DDERR_SURFACENOTATTACHED :     lDDErr = "DDERR_SURFACENOTATTACHED"; break;
		case DDERR_TOOBIGHEIGHT :           lDDErr = "DDERR_TOOBIGHEIGHT"; break;
		case DDERR_TOOBIGSIZE :             lDDErr = "DDERR_TOOBIGSIZE"; break;
		case DDERR_TOOBIGWIDTH :            lDDErr = "DDERR_TOOBIGWIDTH"; break;
		case DDERR_UNSUPPORTED :            lDDErr = "DDERR_UNSUPPORTED"; break;
		case DDERR_UNSUPPORTEDFORMAT :      lDDErr = "DDERR_UNSUPPORTEDFORMAT"; break;
		case DDERR_UNSUPPORTEDMASK :        lDDErr = "DDERR_UNSUPPORTEDMASK"; break;
		case DDERR_VERTICALBLANKINPROGRESS :lDDErr = "DDERR_VERTICALBLANKINPROGRESS"; break;
		case DDERR_WASSTILLDRAWING :        lDDErr = "DDERR_WASSTILLDRAWING"; break;
		case DDERR_XALIGN :                 lDDErr = "DDERR_XALIGN"; break;
		case DDERR_INVALIDDIRECTDRAWGUID :  lDDErr = "DDERR_INVALIDDIRECTDRAWGUID"; break;
		case DDERR_DIRECTDRAWALREADYCREATED:lDDErr = "DDERR_DIRECTDRAWALREADYCREATED"; break;
		case DDERR_NODIRECTDRAWHW :         lDDErr = "DDERR_NODIRECTDRAWHW"; break;
		case DDERR_PRIMARYSURFACEALREADYEXISTS
                                        : lDDErr = "DDERR_PRIMARYSURFACEALREADYEXISTS"; break;
		case DDERR_NOEMULATION            : lDDErr = "DDERR_NOEMULATION"; break;
		case DDERR_REGIONTOOSMALL         : lDDErr = "DDERR_REGIONTOOSMALL"; break;
		case DDERR_CLIPPERISUSINGHWND     : lDDErr = "DDERR_CLIPPERISUSINGHWND"; break;
		case DDERR_NOCLIPPERATTACHED      : lDDErr = "DDERR_NOCLIPPERATTACHED"; break;
		case DDERR_NOHWND                 : lDDErr = "DDERR_NOHWND"; break;
		case DDERR_HWNDSUBCLASSED         : lDDErr = "DDERR_HWNDSUBCLASSED"; break;
		case DDERR_HWNDALREADYSET         : lDDErr = "DDERR_HWNDALREADYSET"; break;
		case DDERR_NOPALETTEATTACHED      : lDDErr = "DDERR_NOPALETTEATTACHED"; break;
		case DDERR_NOPALETTEHW            : lDDErr = "DDERR_NOPALETTEHW"; break;
		case DDERR_BLTFASTCANTCLIP        : lDDErr = "DDERR_BLTFASTCANTCLIP"; break;
		case DDERR_NOBLTHW                : lDDErr = "DDERR_NOBLTHW"; break;
		case DDERR_NODDROPSHW             : lDDErr = "DDERR_NODDROPSHW"; break;
		case DDERR_OVERLAYNOTVISIBLE      : lDDErr = "DDERR_OVERLAYNOTVISIBLE"; break;
		case DDERR_NOOVERLAYDEST          : lDDErr = "DDERR_NOOVERLAYDEST"; break;
		case DDERR_INVALIDPOSITION        : lDDErr = "DDERR_INVALIDPOSITION"; break;
		case DDERR_NOTAOVERLAYSURFACE     : lDDErr = "DDERR_NOTAOVERLAYSURFACE"; break;
		case DDERR_EXCLUSIVEMODEALREADYSET: lDDErr = "DDERR_EXCLUSIVEMODEALREADYSET"; break;
		case DDERR_NOTFLIPPABLE           : lDDErr = "DDERR_NOTFLIPPABLE"; break;
		case DDERR_CANTDUPLICATE          : lDDErr = "DDERR_CANTDUPLICATE"; break;
		case DDERR_NOTLOCKED              : lDDErr = "DDERR_NOTLOCKED"; break;
		case DDERR_CANTCREATEDC           : lDDErr = "DDERR_CANTCREATEDC"; break;
		case DDERR_NODC                   : lDDErr = "DDERR_NODC"; break;
		case DDERR_WRONGMODE              : lDDErr = "DDERR_WRONGMODE"; break;
		case DDERR_IMPLICITLYCREATED      : lDDErr = "DDERR_IMPLICITLYCREATED"; break;
		case DDERR_NOTPALETTIZED          : lDDErr = "DDERR_NOTPALETTIZED"; break;
		case DDERR_UNSUPPORTEDMODE        : lDDErr = "DDERR_UNSUPPORTEDMODE"; break;
		case DDERR_NOMIPMAPHW             : lDDErr = "DDERR_NOMIPMAPHW"; break;
		case DDERR_INVALIDSURFACETYPE     : lDDErr = "DDERR_INVALIDSURFACETYPE"; break;
		case DDERR_DCALREADYCREATED       : lDDErr = "DDERR_DCALREADYCREATED"; break;
		case DDERR_CANTPAGELOCK           : lDDErr = "DDERR_CANTPAGELOCK"; break;
		case DDERR_CANTPAGEUNLOCK         : lDDErr = "DDERR_CANTPAGEUNLOCK"; break;
		case DDERR_NOTPAGELOCKED          : lDDErr = "DDERR_NOTPAGELOCKED"; break;
		case DDERR_NOTINITIALIZED         : lDDErr = "DDERR_NOTINITIALIZED"; break;
		default                           : lDDErr = "Unknown Direct Draw Error"; break;
	}
   return CPString(lDDErr);
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

HRESULT CPGIGraphicSystem::PrivateCreateSurface(DDSURFACEDESC_t& desc,
                                                CPGISurface*& surfaceToBe)
{
#if DIRECTDRAW_VERSION >= 0x700
      return mDD->CreateSurface( &desc, &surfaceToBe, NULL);
#else
      return mDD->CreateSurface( &desc, reinterpret_cast<LPDIRECTDRAWSURFACE FAR*>(&surfaceToBe), NULL);
#endif
}

// Description : takes a screenshot and save the file with the baseFileName + count
// Param       :
// Result      :
// Comments    : count from 0 to 9999, if the file name PPxxxx.bmp does not exists, output it
//               if all screenies from 0 to 9999 are filled, then do nothing
void CPGIGraphicSystem::TakeScreenShot(CPString baseFileName)
{
   for(int lScreenShotsCount = 0; lScreenShotsCount < 99999; lScreenShotsCount++)
   {
     CPString lFileName = baseFileName;

     // make the file name looks nice
     for (int i=0; i<5 - CPString(lScreenShotsCount).Length(); i++)
     {
       lFileName = lFileName + "0";
     }

     // add rest of the file - number and extension
     lFileName = lFileName + CPString(lScreenShotsCount) + CPString(".bmp");

     // does the file exists
     bool lFileExists = (access(lFileName, 0) == 0);

     // do not overwrite existing files
     if (lFileExists)
     {
       continue;
     }

     GBmpSave lBmpSave(lFileName.c_str());

     // lock & save
     SurfaceLocker16bpp locker(mDDSPrimary);

     const DDSURFACEDESC_t& lDDSD = locker.Desc();

     if (!lBmpSave.SaveAligned(static_cast<unsigned char*>(lDDSD.lpSurface), mResolutionX, mResolutionY, lDDSD.lPitch, ApplicationBpp, ApplicationBpp))
     {
       LOG(ERROR) << "CPGIGraphicSystem::TakeScreenShot: error saving image to " << lFileName.c_str();
     }

     // everything went fine, bye bye
     return;
   }

   // it seems that there are 100000 screenshots in the folder; well, do nothing :)
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// SDL compatible interface implementation
//---------------------------------------------------------------------------
void CPGIGraphicSystem::PaintSprite(int whereX, int whereY,
                                    TPGISpriteReference Sprite, int animCount, int animType,
                                    int zoomFactor, int alpha)
{
    PaintSpriteToSurface(NULL, whereX, whereY, Sprite, animCount, animType, zoomFactor, alpha);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::FillRectangle(CPRect rect)
{
    FillRectangle(NULL, rect, mColor);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::Rectangle(CPRect rect)
{
    Rectangle(NULL, rect, mColor);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::Line(CPPoint ptStart, CPPoint ptEnd)
{
    Line(NULL, ptStart, ptEnd, mColor);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::Pie(CPPoint _center, int _radius, CPPoint _dir, int _angle)
{
    Pie(NULL, _center, _radius, _dir, _angle, mColor);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::TextOut2(int X, int Y, CPString text, UINT Alignment)
{
    TextOut(NULL, X, Y, text, Alignment);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::PutPixel(CPPoint ptCenter)
{
    PutPixel(NULL, ptCenter, mColor);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::SelectionRectangle(CPRect rect, int Length)
{
    SelectionRectangle(NULL, rect, Length, mColor);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
