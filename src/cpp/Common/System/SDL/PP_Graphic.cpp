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
              Singleton
 last modify: 13 07 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <regex>
#include <string>
#pragma hdrstop

#include "PP_Graphic.h"
#include "PP_Texture.h"
#include "basic3d.h"
#include "SDL_image.h"
#include "Stream_PhysFS.h"
#include "fixed.h"
#include "ColorConv.h"
#include "extras/physfsrwops.h"
#include "PP_VfsSystem.h"
#include "easylogging++.h"

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
    PGIModeType mode,
    std::string windowTitle)
{
    return CPGIGraphicSystem::Instance(/*hWnd,*/ resX, resY, mode, windowTitle);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creaza o instanta daca nu exista sau returneaza this daca ea exista
 parameters : resX, resY - rezolutia dorita
              daca resX si resy == 0 => se pastreaza rezolutia curenta
---------------------------------------------------------------------------*/
CPGIGraphicSystem* CPGIGraphicSystem::Instance(/*HWND hWnd,*/
    int resX, int resY,
    PGIModeType mode,
    std::string windowTitle)
{
    if (mInstance == 0)
    {
        mInstance = new CPGIGraphicSystem(/*hwnd, */resX, resY, mode, windowTitle);
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
        SDL_DestroyRenderer(mInstance->mRenderer);
        SDL_DestroyWindow(mInstance->mWindow->impl);

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
bool CPGIGraphicSystem::SetClipRegion(CPRect clipRegion)
{
    SDL_Rect rect;

    rect.x = clipRegion.left;
    rect.y = clipRegion.top;
    rect.w = clipRegion.Width();
    rect.h = clipRegion.Height();

    return SDL_RenderSetClipRect(mRenderer, &rect) == 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: disable clipping (can also work as setting the clip to cover all the surface
 parameters :
---------------------------------------------------------------------------*/
bool CPGIGraphicSystem::ResetClipRegion()
{
    return SDL_RenderSetClipRect(mRenderer, NULL) == 0;
    //return SetClipRegion(mRenderer, CPRect(0, 0, mResolutionX, mResolutionY));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::SetColor(TColor32 Color)
{
//   mColor = makeRGB16(GetRValue(Color), GetGValue(Color), GetBValue(Color));
    mColor = Color;
    SDL_SetRenderDrawColor(mRenderer, gfGetRValue(Color), gfGetGValue(Color), gfGetBValue(Color), gfGetAValue(Color));
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::BeginRender()
{
    SDL_RenderClear(mRenderer);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adica face flip daca este double buffered
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::EndRender()
{
    //CPGISurface::SurfaceLocker locker(*mDDSPrimary);
    //SDL_UpdateTexture(mTexture, NULL, mDDSPrimary->Bits(), mDDSPrimary->Pitch());
    //SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
    SDL_RenderPresent(mRenderer);
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
void CPGIGraphicSystem::PaintSprite(int whereX, int whereY, TPGISpriteReference Sprite, int animCount, int animType, int alpha)
{
    if (Sprite == NULL)
    {
        LOG(FATAL) << "CPGIGraphicSystem::PaintSprite - Sprite is NULL";
        return;
    }

    CPRect spriteAnimRect = Sprite->GetAnimRect(animCount, animType);

    SDL_Rect lSrc = {spriteAnimRect.left, spriteAnimRect.top, spriteAnimRect.Width(), spriteAnimRect.Height()};
    SDL_Rect lDest = {whereX, whereY, lSrc.w, lSrc.h};

    unsigned char prevAlpha;
    SDL_GetTextureAlphaMod(Sprite->mTexture->impl, &prevAlpha);
    SDL_SetTextureAlphaMod(Sprite->mTexture->impl, alpha);

    SDL_RenderCopy(mRenderer, Sprite->mTexture->impl, &lSrc, &lDest);

    SDL_SetTextureAlphaMod(Sprite->mTexture->impl, prevAlpha);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::PaintTexture(int whereX, int whereY, CPGITexture* texture)
{
    if (texture == NULL)
    {
        LOG(FATAL) << "CPGIGraphicSystem::PaintTexture - texture is NULL";
        return;
    }

    SDL_Rect lSrc = {0, 0, texture->Width(), texture->Height()};
    SDL_Rect lDest = {whereX, whereY, lSrc.w, lSrc.h};

    SDL_RenderCopy(mRenderer, texture->impl, &lSrc, &lDest);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza un dreptunghi gol cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              rect    - dim dreptunghiului
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Rectangle(CPRect rect)
{
    SDL_Rect sdlRect = {rect.left, rect.top, rect.Width(), rect.Height()};

    SDL_RenderDrawRect(mRenderer, &sdlRect);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: draws the corners of a rectangle
 parameters : rect    - the region of the selection
              Length  - the length of the corner lines
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::SelectionRectangle(CPRect rect, int Length)
{
    SDL_RenderDrawLine(mRenderer, rect.left, rect.top, rect.left + Length, rect.top);
    SDL_RenderDrawLine(mRenderer, rect.left, rect.top, rect.left, rect.top + Length);

    SDL_RenderDrawLine(mRenderer, rect.right - Length, rect.top, rect.right, rect.top);
    SDL_RenderDrawLine(mRenderer, rect.right, rect.top, rect.right, rect.top + Length);

    SDL_RenderDrawLine(mRenderer, rect.left, rect.bottom, rect.left + Length, rect.bottom);
    SDL_RenderDrawLine(mRenderer, rect.left, rect.bottom - Length, rect.left, rect.bottom);

    SDL_RenderDrawLine(mRenderer, rect.right - Length, rect.bottom, rect.right, rect.bottom);
    SDL_RenderDrawLine(mRenderer, rect.right, rect.bottom - Length, rect.right, rect.bottom);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: renders a filled rectangle
 parameters : rect - rectangle to fill
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::FillRectangle(CPRect rect)
{
    SDL_Rect sdlRect = {rect.left, rect.top, rect.Width(), rect.Height()};

    SDL_RenderFillRect(mRenderer, &sdlRect);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deseneaza o linie cu o culoare data
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              color   - culoarea
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Line(CPPoint ptStart, CPPoint ptEnd)
{
    SDL_RenderDrawLine(mRenderer, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: tipareste un cerc
 parameters : ptCenter- centrul cercului
              radius  - raza cercului
 others     :
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::Circle(CPPoint ptCenter, int radius)
{
    int d, xi, yi, x = ptCenter.x, y = ptCenter.y;

    radius = abs(radius);
    if (radius == 0)
    {
        return;
    }
    else if (radius == 1)
    {
        SDL_RenderDrawPoint(mRenderer, ptCenter.x, ptCenter.y);
        return;
    }

    xi = radius;
    yi = 0;
    d  = 3 - 2 * radius;
    std::vector<SDL_Point> points;

    while (xi >= yi)
    {
        // could render each point
        /*
        SDL_RenderDrawPoint(mRenderer, x+xi, y+yi);
        SDL_RenderDrawPoint(mRenderer, x-xi, y+yi);
        SDL_RenderDrawPoint(mRenderer, x+yi, y+xi);
        SDL_RenderDrawPoint(mRenderer, x-yi, y+xi);

        SDL_RenderDrawPoint(mRenderer, x+xi, y-yi);
        SDL_RenderDrawPoint(mRenderer, x-xi, y-yi);
        SDL_RenderDrawPoint(mRenderer, x+yi, y-xi);
        SDL_RenderDrawPoint(mRenderer, x-yi, y-xi);
        */

        // also could push to a vector, then render all at once
        points.push_back({x+xi, y+yi});
        points.push_back({x-xi, y+yi});
        points.push_back({x+yi, y+xi});
        points.push_back({x-yi, y+xi});

        points.push_back({x+xi, y-yi});
        points.push_back({x-xi, y-yi});
        points.push_back({x+yi, y-xi});
        points.push_back({x-yi, y-xi});

        if (d<0)
        {
            yi++;
            d = d + (yi << 2) + 2;
        }
        else
        {
            yi++;
            xi--;
            d = d - (xi << 2) + (yi << 2) + 2;
        }
    }

    SDL_RenderDrawPoints(mRenderer, points.data(), points.size());
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::FillCircle(CPPoint ptCenter, int radius)
{
    int d, xi, yi, x = ptCenter.x, y = ptCenter.y;

    radius = abs(radius);
    if (radius == 0)
    {
        return;
    }
    else if (radius == 1)
    {
        SDL_RenderDrawPoint(mRenderer, ptCenter.x, ptCenter.y);
        return;
    }

    HLine(x-radius, x+radius, y);

    xi = radius;
    yi = 1;
    d  = 3 - 2 * radius;

    while (xi > yi)
    {
        HLine(x-xi, x+xi, y+yi);
        HLine(x-xi, x+xi, y-yi);

        if (d<0)
        {
            yi++;
            d = d + (yi << 2) + 2;
        }
        else
        {
            HLine(x-yi, x+yi, y+xi);
            HLine(x-yi, x+yi, y-xi);

            yi++;
            xi--;
            d = d - (xi << 2) + (yi << 2) + 2;
        }
    }
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
// Comments    : Don't use this function with angle value higher than 180
// Comments comment: TMN: Why not? It seems to work just fine.
void CPGIGraphicSystem::Pie(CPPoint      _center,
                            int          _radius,
                            CPPoint      _dir,
                            int          _angle)
{
    // normalize the direction
    float lValue = sqrt((float)(_dir.x * _dir.x + _dir.y * _dir.y));

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
    FillTriangle( _center.x + x1, _center.y + y1, _center.x + x2, _center.y + y2, _center.x, _center.y);

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
            t = y1;
            y1 = y2;
            y2 = t;
            t = x1;
            x1 = x2;
            x2 = t;
        }

        // prepare to scan the line - don't care for a line Bresenham for now
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
            float x0 = sqrt(fabs((float)(_radius * _radius - y * y)));

            HLine(
                _center.x + x + lSign,
                _center.x + lSign * x0,
                _center.y + y
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
        t = y1;
        y1 = y2;
        y2 = t;
        t = x1;
        x1 = x2;
        x2 = t;
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
        float y0 = sqrt(fabs((float)(_radius * _radius - x * x)));

        VLine(_center.y + lSign * y0, _center.y + y + lSign, _center.x + x);

        x++;
        y += lM;
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: draws a point.
 parameters : ptCenter- centrul cercului
 others     : SetColor and SetBlendMode to control the color and blend of pixel
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::PutPixel(CPPoint ptCenter)
{
    SDL_RenderDrawPoint(mRenderer, ptCenter.x, ptCenter.y);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::HLine(int startX, int endX, int y)
{
    //int actualStartX = std::min(startX, endX);
    //int actualEndX = std::max(startX, endX);

    SDL_RenderDrawLine(mRenderer, startX, y, endX, y);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::VLine(int startY, int endY, int x)
{
    //int actualStartY = std::min(startY, endY);
    //int actualEndY = std::max(startY, endY);

    SDL_RenderDrawLine(mRenderer, x, startY, x, endY);
}
//---------------------------------------------------------------------------

void CPGIGraphicSystem::FillTriangle(int p0x, int p0y, int p1x, int p1y, int p2x, int p2y)
{
    Fixed   d,d0;
    Fixed   x,x0;
    int     y;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y)
    {
        std::swap(p0x,p1x);
        std::swap(p0y, p1y);
    }
    if (p0y > p2y)
    {
        std::swap(p0x,p2x);
        std::swap(p0y, p2y);
    }
    if (p1y > p2y)
    {
        std::swap(p1x,p2x);
        std::swap(p1y, p2y);
    }

    //
    //  check for quick out?
    //
    if (p2y - p0y == 0)
    {
        return;
    }

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0) d = (Fixed)(p2x - p0x) / (Fixed)(p2y - p0y);
    else d = (p2x - p0x) * 100;


    x  = p0x;
    y  = p0y;
    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0) d0 = (Fixed)(p1x - p0x) / (Fixed)(p1y - p0y);
        else d0 = (p1x - p0x) * 100;

        x0 = p0x;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                //FillSpecial(p, x, x0, c);
                HLine(x, x0, y);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
        else
        {
            while (y < p1y)
            {
                HLine(x, x0, y);
                //FillSpecial(p, x0, x, c);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
    }

    //
    // do the bottom.
    //

    if (p2y - p1y == 0)
    {
        return;
    }

    if ((p2y - p1y) != 0) d0 = (Fixed)(p2x - p1x) / (Fixed)(p2y - p1y);
    else d0 = (p2x - p1x) * 100;

    x0 = p1x;

    //
    // check for left or right fill
    //
    if (x < x0)
    {
        while (y < p2y)
        {
            HLine(x, x0, y);
            //FillSpecial(p, x, x0, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
    else
    {
        while (y < p2y)
        {
            HLine(x, x0, y);
            //FillSpecial(p, x0, x, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
}
/*---------------------------------------------------------------------------
 description: tipreste un text intr-un chenar
 parameters : Surface - daca este NULL => se tiparestein back sau in primary dupa
                        mDoubleBuffered
              Text    - textul de tiparit
              ClipRect- dreptunghiul de clip
              Format  - flag care arata formatul aliniamentului
 others     : culoarea trebuie setata in prealabil (mColor)
---------------------------------------------------------------------------*/
void CPGIGraphicSystem::TextOutWrapped(int X, int Y, CPString text, int width)
{
    if (!mFont)
    {
        return;
    }

    mFont->PaintTextWrapped(X, Y, text, width);
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
void CPGIGraphicSystem::TextOut2(int X, int Y,
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

    mFont->PaintText(X, Y, Text);
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

CPGITexture* CPGIGraphicSystem::CreateTextureFromFile(CPString fileName)
{
    CPGITexture* texture = NULL;

    if (GetVfsInstance()->FileExists(fileName))
    {
        PP::Stream_PhysFS inputStream(fileName);

        size_t fileSize = inputStream.length();
        char* fileData = new char[fileSize];
        inputStream.read(fileData, fileSize);

        texture = new CPGITexture();

        SDL_RWops* src = SDL_RWFromMem(fileData, fileSize);

        texture->impl = IMG_LoadTexture_RW(mRenderer, src, 1);

        delete [] fileData;
    }

    return texture;
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

/** \brief Checks that the SDL window has input and mouse focus
 *
 * \return true if the SDL window has input and mouse focus
 *
 */
bool CPGIGraphicSystem::IsWindowActive()
{
    return (SDL_GetWindowFlags(mWindow->impl) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS)) != 0;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructorul clasei
 parameters : resX, resY - rezolutia dorita
---------------------------------------------------------------------------*/
CPGIGraphicSystem::CPGIGraphicSystem(int resX, int resY, PGIModeType mode, std::string windowTitle)
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

    mPixelFormat = SDL_PIXELFORMAT_RGBA32;

    InitGraphics(/*hwnd, */resX, resY, mWindowed, windowTitle);
}
//---------------------------------------------------------------------------

CPGIGraphicSystem::CPGIGraphicSystem(/*HWND hWnd,*/ int resX, int resY, std::string windowTitle)
{
    mResolutionX = 0;
    mResolutionY = 0;

    mGenericSprites = new CPGISpriteList();
    mMissionSprites = new CPGISpriteList();

    mColor      = 0xFFFFFFFF;
    //mPen        = CreatePen(PS_SOLID, 0, mColor);
    //mFont       = new CPFont(getGamePath() + PATH_FONTS + FILE_FONT_SMALL);
    mFont = 0;

    mWindowed         = false;

    InitGraphics(/*hwnd, */resX, resY, mWindowed, windowTitle);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructorul clasei
 others     : elibereaza obiectele DX si lista de sprite-uri
---------------------------------------------------------------------------*/
CPGIGraphicSystem::~CPGIGraphicSystem()
{
    //DeleteObject(mPen);
    VLOG(1) << "CPGIGraphicSystem::~CPGIGraphicSystem()";

    if (mGenericSprites != NULL)
    {
        delete mGenericSprites;
    }

    if (mMissionSprites != NULL)
    {
        delete mMissionSprites;
    }

    VLOG(1) << "CPGIGraphicSystem::~CPGIGraphicSystem() - sprites deleted";

    delete mWindow;

    VLOG(1) << "CPGIGraphicSystem::~CPGIGraphicSystem() - end";
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
bool CPGIGraphicSystem::InitGraphics(/*HWND hWnd,*/ int resX, int resY, bool windowed, std::string windowTitle)
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

    //SDL_WM_SetCaption("PPTactical Engine 0.9.6", "PPTactical Engine");

    if (windowed)
    {
        mWindow = new CPWindow(SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mResolutionX, mResolutionY, 0));
    }
    else
    {
        mWindow = new CPWindow(SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mResolutionX, mResolutionY, SDL_WINDOW_FULLSCREEN_DESKTOP));
    }

    if (!mWindow->Valid())
    {
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow->impl, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC/*SDL_RENDERER_SOFTWARE*/);

    if (!mRenderer)
    {
        return false;
    }

    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

    return true;

}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: takes a screenshot and save the file with the baseFileName + count
 --------------------------------------------------------------------------*/
void CPGIGraphicSystem::TakeScreenShot(CPString fileName)
{
    CPGISurface* screenshot = new CPGISurface();

    // create a SDL_Surface with the same size as the current renderer
    screenshot->pimpl = SDL_CreateRGBSurface(0, GetResX(), GetResY(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    // lock the screenshot surface to gain access to the "bits"
    CPGISurface::SurfaceLocker* locker = new CPGISurface::SurfaceLocker(*screenshot);

    // read the pixels from the renderer
    SDL_RenderReadPixels(mRenderer, NULL, SDL_GetWindowPixelFormat(mWindow->impl), screenshot->Bits(), screenshot->Pitch());

    // unlock the screenshot surface
    delete locker;

    SDL_RWops* rwops = PHYSFSRWOPS_openWrite(fileName.c_str());

    // save to file
    //int result = IMG_SavePNG(screenshot->pimpl, fileName.c_str());
    int result = IMG_SavePNG_RW(screenshot->pimpl, rwops, 1); // also free the SDL_RWops instance

    if (!result)
    {
        LOG(ERROR) << "CPGIGraphicSystem::TakeScreenShot - error saving image " << fileName.c_str();
    }

    // cleanup
    delete screenshot;
}
//---------------------------------------------------------------------------

CPGITexture* CPGIGraphicSystem::CreateTexture(int Width, int Height)
{
    CPGITexture* texture = new CPGITexture();

    texture->impl = SDL_CreateTexture(mRenderer, SDL_TEXTUREACCESS_STATIC, SDL_PIXELFORMAT_ARGB32, Width, Height);

    return texture;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
