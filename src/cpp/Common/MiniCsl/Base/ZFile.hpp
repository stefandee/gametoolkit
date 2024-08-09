/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZFile.hpp
 * Application :  IBK Open Class Library
 * Purpose     :  File handling utilities
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.06.18  First implementation                        P.Koch, IBK
 * 2001.12.03  Supressed all external links defines        Karg
 *
 * OPEN SOURCE LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to IBK at info@ibk-software.ch.
 */

#ifndef _ZFILE_
#define _ZFILE_

#include <ZString.hpp>

class ZFile : public ZBase {
   public:
      static ZString
         stripExtension(                        // split filename & extension
            const ZString& aFileName,              // file name
            ZString& aExtension);                  // return removed extension
                                                   // returns filename without ext
      static ZString
         stripExtension(                        // remove extension from filename
            const ZString& aFileName);             // file name
                                                   // returns filename without ext
      static ZString
         stripPath(                             // split directory path & filename
            const ZString& aFileName,              // file name
            ZString& aPath);                       // return removed path
                                                   // returns filename without dir
      static ZString
         stripPath(                             // remove directory from filename
            const ZString& aFileName);             // file name
                                                   // returns filename without dir
      static ZString
         addExtension(                          // add extension if none present
            const ZString& aFileName,              // filename
            const ZString& aExtension);            // extension
                                                   // returns filename with extension
      static ZString
         fullPath(                              // get full path of partial name
            const ZString& aFileName);             // partial filename
                                                   // returns full path name
      static ZString
         locateFile(                            // locate file by env var
            const ZString& aFileName,              // file to locate
            const ZString& aEnvVar);               // env var with colon sep. paths
                                                   // returns full path if found,
                                                   //         empty string if not
   private:
      ZFile(); // no instance creation
}; // ZFile

#endif // _ZFILE_
