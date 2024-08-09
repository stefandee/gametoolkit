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
#ifndef FONTCACHE_H
#define FONTCACHE_H

#include <map>
#include <string>
#include <SDL_ttf.h>

class FontFileDataEntry
{
   public:
      char* data;
      int   length;
};

/** \brief Font Cache
 *
 * Management to share TTF_Font instances and cache font file data.
 */
class CPFontCache
{
    public:
        CPFontCache();
        ~CPFontCache();

        static CPFontCache* Instance();
        TTF_Font* GetFont(std::string fileName, int pointSize);

    protected:

    private:
      std::string GetFontMapKey(std::string fileName, int pointSize);

    private:
       std::map<std::string,FontFileDataEntry*> mFontFileDataMap;
       std::map<std::string,TTF_Font*> mFontMap;

       static CPFontCache* mInstance;
};

#endif // FONTCACHE_H
