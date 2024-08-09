//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop

#include "Platform.h"

#ifdef PPT_COMPILER_MICROSOFT
#include <memory.h>
#elif PPT_COMPILER_BORLAND
#include <mem.h>
#endif

#include "GJpgLoad.h"
#include "PPIG_Exit.h"
//---------------------------------------------------------------------------

/******************** JPEG DECOMPRESSION SAMPLE INTERFACE *******************/

/* This half of the example shows how to read data from the JPEG decompressor.
 * It's a bit more refined than the above, in that we show:
 *   (a) how to modify the JPEG library's standard error-reporting behavior;
 *   (b) how to allocate workspace using the library's memory manager.
 *
 * Just to make this example a little different from the first one, we'll
 * assume that we do not intend to put the whole image into an in-memory
 * buffer, but to send it line-by-line someplace else.  We need a one-
 * scanline-high JSAMPLE array as a work buffer, and we will let the JPEG
 * memory manager allocate it for us.  This approach is actually quite useful
 * because we don't need to remember to deallocate the buffer separately: it
 * will go away automatically when the JPEG object is cleaned up.
 */


/*
 * Here's the routine that will replace the standard error_exit method:
 */
METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  //(*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
  //GetExitInstance()->SetTerminated(true);
}

/*
METHODDEF(void) my_emit_message (j_common_ptr cinfo, int msg_level)
{
  // really do nothing, we do not want to custom log the png internal msg/warn/errs
}

METHODDEF(void) my_output_message (j_common_ptr cinfo)
{
}

METHODDEF(void) my_reset_error_mgr (j_common_ptr cinfo)
{
  cinfo->err->num_warnings = 0;
  // trace_level is not reset since it is an application-supplied parameter
  cinfo->err->msg_code = 0;	//may be useful as a flag for "no error"
}

METHODDEF(void) my_format_message (j_common_ptr cinfo, char * buffer)
{
}
*/

GJpgLoad::GJpgLoad()
{
}
//---------------------------------------------------------------------------

bool GJpgLoad::Open        (const char* fileName)
{
  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */
  if ((mInFile = fopen(fileName, "rb")) == NULL)
  {
    //fprintf(stderr, "can't open %s\n", filename);
    return false;
  }

  /* We set up the normal JPEG error routines, then override error_exit. */
  mInfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer))
  {
    //If we get here, the JPEG code has signaled an error.
    //We need to clean up the JPEG object, close the input file, and return.
    //
    jpeg_destroy_decompress(&mInfo);
    fclose(mInFile);
    return false;
  }

  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&mInfo);

  /* Step 2: specify data source (eg, a file) */
  jpeg_stdio_src(&mInfo, mInFile);

  if (!ReadHeader())
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool GJpgLoad::Load        (unsigned char* data, int width, int height, TColorFormat dataBpp)
{
  //return false;
  return LoadAligned(data, width, height, width, dataBpp);
}
//---------------------------------------------------------------------------

bool GJpgLoad::LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
  /* More stuff */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* Step 5: Start decompressor */
  (void) jpeg_start_decompress(&mInfo);

  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */
  /* JSAMPLEs per row in output buffer */
  row_stride = mInfo.output_width * mInfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*mInfo.mem->alloc_sarray)
  		((j_common_ptr) &mInfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */

  unsigned char* startPtr = data;

  // speed up ;) - duplicating code results in faster loading
  switch(dataBpp)
  {
    case RGB_555:
      break;

    case RGB_565:
      {
        while (mInfo.output_scanline < mInfo.output_height)
        {
          /* jpeg_read_scanlines expects an array of pointers to scanlines.
           * Here the array is only one element long, but you could ask for
           * more than one scanline at a time if that's more convenient.
           */
          int lScanLinesRead = jpeg_read_scanlines(&mInfo, buffer, 1);

          if (lScanLinesRead == 0)
          {
            continue;
          }

          for(int j = 0; j < width; j++)
          {
            *((__int16*)startPtr + j) =
                    (__int16)((((__int16)buffer[0][j*3] >> 3) << 11) |
                    (((__int16)buffer[0][j*3+1] >> 2) << 5)  |
                    (buffer[0][j*3+2] >> 3));
          }

          startPtr += bytesPitch;
        }
      }
      break;
  }

  /* Step 7: Finish decompression */
  (void) jpeg_finish_decompress(&mInfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  return true; 
}
//---------------------------------------------------------------------------

bool GJpgLoad::ReadHeader()
{
  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 3: read file parameters with jpeg_read_header() */
  (void) jpeg_read_header(&mInfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  // filling the protected member "info" ;)
  // we don't use scaling, so it's no need to use output_width/height members
  info.width  = mInfo.image_width;
  info.height = mInfo.image_height;
  info.bpp    = RGB_24;

  return true;
}
//---------------------------------------------------------------------------

GJpgLoad::~GJpgLoad()
{
  //(void) jpeg_finish_decompress(&mInfo);
  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&mInfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(mInFile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */
}
//---------------------------------------------------------------------------

/*
 * SOME FINE POINTS:
 *
 * In the above code, we ignored the return value of jpeg_read_scanlines,
 * which is the number of scanlines actually read.  We could get away with
 * this because we asked for only one line at a time and we weren't using
 * a suspending data source.  See libjpeg.doc for more info.
 *
 * We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
 * we should have done it beforehand to ensure that the space would be
 * counted against the JPEG max_memory setting.  In some systems the above
 * code would risk an out-of-memory error.  However, in general we don't
 * know the output image dimensions before jpeg_start_decompress(), unless we
 * call jpeg_calc_output_dimensions().  See libjpeg.doc for more about this.
 *
 * Scanlines are returned in the same order as they appear in the JPEG file,
 * which is standardly top-to-bottom.  If you must emit data bottom-to-top,
 * you can use one of the virtual arrays provided by the JPEG memory manager
 * to invert the data.  See wrbmp.c for an example.
 *
 * As with compression, some operating modes may require temporary files.
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 */

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

