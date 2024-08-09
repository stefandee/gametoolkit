//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 2004 Stefan Dicu                                            //
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

//-----------------------------------------------------------------------------
//  Unit              : Jpeg file interface
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * loading and saving from/to jpeg files
//    * based on jpeglib v6b
//    * it needs extensive testing before using it in an "industrial" application
//    * code is copied from the examples provided with the jpeg library; i kept the
//      original comments so that everything should be straight forward to understand
//
//  Istorie           :
//    [14.09.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef GJpgLoadH
#define GJpgLoadH

#include "jpeglib.h"
#include "GLoad.h"
#include <setjmp.h>

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

// jpeg loading class
class GJpgLoad : public GLoad
{
  public:
    GJpgLoad();
    virtual bool Open        (const char* fileName);
    virtual bool Load        (unsigned char* data, int width, int height, TColorFormat dataBpp);
    virtual bool LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    virtual ~GJpgLoad();

  protected:
    virtual bool ReadHeader();
    bool Load8Bits  (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool Load16Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);
    bool Load24Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp);

  private:
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct mInfo;
    FILE*  mInFile;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;

};
//---------------------------------------------------------------------------

#endif
