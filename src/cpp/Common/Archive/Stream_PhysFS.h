//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Stream from Physics VFS
//
//  Version           : 1.0
//
//  Description       :
//    * obtains a file handle from the vfs
//
//  History           :
//    [18.11.2005] - [Karg] - created
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef Stream_PhysFSH
#define Stream_PhysFSH
//---------------------------------------------------------------------------

#include "Stream.h"
#include "PP_String.h"

namespace PP
{
  class Stream_PhysFS : public Stream
  {
    public:
            Stream_PhysFS(const char* filename, bool _empty = false);
            virtual ~Stream_PhysFS();

            // returns number of bytes read.
            virtual size_t read(void* pMem, size_t nBytes);

            // TMN: If we can't rewind, then the brown stuff hit the rotating
            // ventilation device.
            virtual bool   rewind();
            virtual bool   seek(size_t offset);
            virtual bool   skip(size_t size);
            virtual size_t length();
            virtual int    tell();
            virtual size_t write(void* pMem, size_t nBytes);
            virtual bool valid();

    public: // typed io
            virtual bool  readInt   (int*    _v);
            virtual bool  readShort (short*  _v);
            virtual bool  readDouble(double* _v);
            virtual bool  readFloat (float*  _v);
            virtual bool  readBool  (bool*   _v);

            virtual bool  writeInt   (int*    _v);
            virtual bool  writeDouble(double* _v);
            virtual bool  writeFloat (float*  _v);
            virtual bool  writeBool  (bool*   _v);

            // FIXME
            static CPString FixPath2(CPString _path);

    private:
            Stream_PhysFS(const Stream_PhysFS& rhs);	// no impl.
            void operator=(const Stream_PhysFS& rhs);	// no impl.

            class Stream_PhysFS_impl* mPimpl;
  };
}	// end of namespace PP

#endif
