/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZProcess.cpp
 * Application :  IBK Open Class Library
 * Purpose     :  Process and thread related operations
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.18  First implementation                        P.Koch, IBK
 * 2001.12.03  Supressed all external links defines        Karg
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

#include <ZProcess.hpp>
#include <ZTrace.hpp>

#ifdef PPT_PLATFORM_WIN
   #include <windows.h>
#endif

#ifdef PPT_PLATFORM_OS2
   #define INCL_DOSPROCESS
   #include <os2.h>
#endif

#ifdef PPT_FAMILY_UNIX
   #include <sys/types.h>
   #include <sys/time.h>
   #include <unistd.h>
#endif

void ZCurrentThread::sleep(unsigned long aMilliSecs)
{
   ZFUNCTRACE_DEVELOP("ZCurrentThread::sleep(unsigned long aMilliSecs)");
#if PPT_PLATFORM_WIN
   Sleep(aMilliSecs);
#elif PPT_PLATFORM_OS2
   DosSleep(aMilliSecs);
#elif PPT_FAMILY_UNIX
   struct timeval timeout;
   timeout.tv_sec = aMilliSecs / 1000;
   timeout.tv_usec = (aMilliSecs % 1000) * 1000;
   select(0, NULL, NULL, NULL, &timeout);
#else
   Not yet implemented.
#endif
} // sleep
