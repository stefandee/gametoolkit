/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZPlatfrm.hpp
 * Application :  IBK Open Class Library
 * Purpose     :  Information about the platform the program is running on.
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.20  First implementation                        P.Koch, IBK
 * 2001.07.11  Add OS/2 and Unixish platforms              P.Koch, IBK
 * 2001.10.27  Add FreeBSD and NetBSD                      P.Koch, IBK
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

#ifndef _ZPLATFRM_
#define _ZPLATFRM_

#include <ZBase.hpp>

class ZPlatform : public ZBase {
   public:
      static ZBoolean isOS2();
      static ZBoolean isLinux();
      static ZBoolean isBSD();
      static ZBoolean isFreeBSD();
      static ZBoolean isNetBSD();
      static ZBoolean isAIX();
      static ZBoolean isSolaris();
      static ZBoolean isUnix();
      static ZBoolean isUnixFamily();
      static ZBoolean isWin95();
      static ZBoolean isWin98();
      static ZBoolean isWinME();
      static ZBoolean isWinNT3();
      static ZBoolean isWinNT4();
      static ZBoolean isWin2000();
      static ZBoolean isWinXP();
      static ZBoolean isWinServer();
      static ZBoolean isWin95Family();
      static ZBoolean isWinNTFamily();
      static ZBoolean isWindows();

      static int numProcessors();

   private:
      ZPlatform();
      ZPlatform(const ZPlatform& aPlatform);
      ZPlatform& operator=(const ZPlatform&);
      static void init();

      static ZBoolean iOS2;
      static ZBoolean iLinux;
      static ZBoolean iFreeBSD;
      static ZBoolean iNetBSD;
      static ZBoolean iAIX;
      static ZBoolean iSolaris;
      static ZBoolean iUnix;
      static ZBoolean iWin95;
      static ZBoolean iWin98;
      static ZBoolean iWinME;
      static ZBoolean iWinNT3;
      static ZBoolean iWinNT4;
      static ZBoolean iWin2000;
      static ZBoolean iWinXP;
      static ZBoolean iWinServer;

      static int iNumProcessors;
};

#endif // _ZPLATFRM_
