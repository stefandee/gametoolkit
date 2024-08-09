//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2017 Stefan Dicu & Tudor Girba                         //
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

#include "Stream_PhysFS.h"
#include <string>
#pragma hdrstop

#include "PP_FontCache.h"
#include "easylogging++.h"

CPFontCache* CPFontCache::mInstance = NULL;

CPFontCache::CPFontCache()
{
    //ctor
}

CPFontCache::~CPFontCache()
{
    //dtor
    // TODO free the memory in the mFontFileDataMap
    // TODO close all fonts in the mFontMap
}

CPFontCache* CPFontCache::Instance()
{
    if (mInstance == 0)
    {
        mInstance = new CPFontCache();
    }

    return mInstance;
}

TTF_Font* CPFontCache::GetFont(std::string fileName, int pointSize)
{
    std::string key = GetFontMapKey(fileName, pointSize);

    if (mFontMap.count(key) == 0)
    {
        if (mFontFileDataMap.count(fileName) == 0)
        {
            // load the data from file
            //mFont = TTF_OpenFont(mName, mSize);

            // load the data from vfs
            FontFileDataEntry* entry = new FontFileDataEntry();
            PP::Stream_PhysFS inputStream(fileName.c_str());

            entry->length = inputStream.length();
            entry->data = new char[entry->length];

            inputStream.read(entry->data, entry->length);

            mFontFileDataMap[fileName] = entry;
        }

        // create the font based on the file data
        SDL_RWops* src = SDL_RWFromMem(mFontFileDataMap[fileName]->data, mFontFileDataMap[fileName]->length);

        TTF_Font* newFont = TTF_OpenFontRW(src, 1, pointSize);

        if (!newFont)
        {
            LOG(ERROR) << "Cannot create font cache for " << fileName << " with point size " << pointSize;
        }

        TTF_SetFontStyle(newFont, TTF_STYLE_NORMAL);
        mFontMap[key] = newFont;
    }

    return mFontMap[key];
}

std::string CPFontCache::GetFontMapKey(std::string fileName, int pointSize)
{
    std::ostringstream s;

    s << pointSize;

    return fileName + "_" + s.str();
    //return fileName + "_" + std::to_string(pointSize);
}

