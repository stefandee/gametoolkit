/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZExcept.hpp
 * Application :  IBK Open Class Library
 * Purpose     :  ZException is the general purpose exception class and is
 *                derived from ZStringlist.
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.20  First implementation                        P.Koch, IBK
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

#ifndef _ZEXCEPT_
#define _ZEXCEPT_

#include <ZTrace.hpp>
#include <ZStrlist.hpp>

#define ZEXCLASSDECLARE(child,parent) \
\
class child : public parent { \
   public: \
      child(); \
      child(const ZString& aMessage); \
      void logException() const; \
      void setLocation(const char* aFile, long aLine); \
   private: \
      ZString iFile; \
      long iLine; \
};

#define ZEXCLASSIMPLEMENT(child, parent) \
\
   child::child() : \
   iLine(0) \
{} \
\
child::child(const ZString& aMessage) : \
   parent(aMessage), \
   iLine(0) \
{} \
\
void child::logException() const \
{ \
   ZTrace trc(iFile, iLine, #child); \
   for (long i = 0; i < count(); i++) \
      ZTrace::writeMsg(iFile, iLine, (*this)[i]); \
} \
\
void child::setLocation(const char* aFile, long aLine) \
{ \
   iFile = aFile; \
   iLine = aLine; \
}

ZEXCLASSDECLARE(ZException, ZStringlist)

#define ZTHROW(exc)\
{ \
   exc.setLocation(__FILE__, __LINE__); \
   exc.logException(); \
   throw exc; \
}

#define ZTHROWEXC(aText) \
{ \
   ZException exc(aText); \
   ZTHROW(exc); \
}

#endif // _ZEXCEPT_
