//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2017 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#ifndef CPGITEXTURE_H
#define CPGITEXTURE_H

#include "SDL.h"

class CPGITexture
{
public:
    CPGITexture();
    virtual ~CPGITexture();

    int Width() const;
    int Height() const;
    unsigned int Format() const;

    class TextureLocker
    {
    public:
        TextureLocker(CPGITexture& surface);
        ~TextureLocker();

    private:
        TextureLocker(const TextureLocker&);     // no impl.
        void operator=(const TextureLocker&);    // no impl.

        CPGITexture&    mTexture;
    };

protected:

private:
    SDL_Texture* impl;

	void** m_pBits;
	int	m_pitch;

    CPGITexture(const CPGITexture& rhs);	// no implementation
    void operator=(const CPGITexture& rhs);	// no implementation

    friend class CPGIGraphicSystem;
    friend class CSprite;
    friend class CPFont;
};

#endif // CPGITEXTURE_H
