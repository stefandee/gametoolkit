/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZFile.cpp
 * Application :  IBK Open Class Library
 * Purpose     :  File handling utilities
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.06.18  First implementation                        P.Koch, IBK
 *
 * OPEN SOURCE LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2.1 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; if not, write to IBK at info@ibk-software.ch.
 */

//#define ZC_BUILDING_ZBASE

#include <ZFile.hpp>
#include <ZTrace.hpp>
#include <stdlib.h>
#if PPT_FAMILY_UNIX
   #include <unistd.h>
#endif
#if PPT_PLATFORM_WIN || PPT_PLATFORM_OS2
   #include <io.h>
#endif

ZString ZFile::stripExtension(const ZString& aFileName, ZString& aExtension)
{
   ZFUNCTRACE_DEVELOP("stripExtension(const ZString& aFileName, const ZString& aExtension)");
   long pos = aFileName.lastIndexOf('.');
   if (pos > aFileName.lastIndexOf(PPT_PATHSEPARATOR)) {
      aExtension = aFileName.subString(pos);
      return aFileName.subString(1,pos-1);
   } // if
   aExtension = "";
   return aFileName;
} // stripExtension

ZString ZFile::stripExtension(const ZString& aFileName)
{
   ZFUNCTRACE_DEVELOP("stripExtension(const ZString& aFileName)");
   ZString ext;
   return stripExtension(aFileName, ext);
} // stripExtension

ZString ZFile::stripPath(const ZString& aFileName, ZString& aPath)
{
   ZFUNCTRACE_DEVELOP("stripPath(const ZString& aFileName, const ZString& aPath)");
   long pos = aFileName.lastIndexOf(PPT_PATHSEPARATOR);
   if (pos) {
      aPath = aFileName.subString(1,pos);
      return aFileName.subString(pos+1);
   } // if
   aPath = "";
   return aFileName;
} // stripPath

ZString ZFile::stripPath(const ZString& aFileName)
{
   ZFUNCTRACE_DEVELOP("stripPath(const ZString& aFileName)");
   ZString path;
   return stripPath(aFileName, path);
} // stripPath

ZString ZFile::addExtension(const ZString& aFileName, const ZString& aExtension)
{
   ZFUNCTRACE_DEVELOP("addExtension(const ZString& aFileName, const ZString& aExtension)");
   ZString ext;
   ZString file = stripExtension(aFileName, ext);
   if (ext.size()) return aFileName; // has allready an extension
   return file+aExtension;
} // addExtension

ZString ZFile::fullPath(const ZString& aFileName)
{
   ZFUNCTRACE_DEVELOP("fullPath(const ZString& aFileName)");
#if PPT_PLATFORM_WIN || PPT_PLATFORM_OS2
   char path[2048];
 #if PPT_COMPILER_GNU
    // TODO find a replacement for _fullpath
   //_fullpath(path, aFileName.constBuffer(), sizeof(path));
 #else
   ZString file(aFileName);
   _fullpath(path, file, sizeof(path));
 #endif
   return ZString(path);
#endif
#if PPT_FAMILY_UNIX
   long len(aFileName.length());
   if (len >= 1 && aFileName[1] == PPT_PATHSEPARATOR) return aFileName;
   char cwd[2048];
   getcwd(cwd, sizeof(cwd));
   return ZString(cwd).stripTrailing(PPT_PATHSEPARATOR)+PPT_PATHSEPARATOR+aFileName;
#endif
} // fullPath

ZString ZFile::locateFile(const ZString& aFileName, const ZString& aEnvVar)
{
   ZFUNCTRACE_DEVELOP("locateFile(const ZString& aFileName, const ZString& aEnvVar)");

   // try from current path
   if (access(aFileName, 0)==0) return fullPath(aFileName);

   // don't try further if path enclosed
   if (aFileName.indexOf(PPT_PATHSEPARATOR)) return ZString();

   // try each path in colon separated env var
   const char* e = getenv(aEnvVar.constBuffer());
   while (e && *e) {
      ZString buf;
#if PPT_PLATFORM_WIN || PPT_PLATFORM_OS2
      while (*e && *e != ';') buf += *e++;
#endif
#if PPT_UNIX_FAMILY
      while (*e && *e != ':') buf += *e++;
#endif
      if (buf.size()) {
         if (buf[buf.size()] != PPT_PATHSEPARATOR)
            buf += PPT_PATHSEPARATOR;
         buf += aFileName;
         if (access(buf.constBuffer(), 0)==0) return fullPath(buf);
      } // if
      if (*e) e++;
   } // while
   return ZString();
} // locateFile
