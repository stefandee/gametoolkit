/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZDate.cpp
 * Application :  IBK Open Class Library
 * Purpose     :  Class with date and time (timestamp).
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.20  First implementation                        P.Koch, IBK
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

#include <time.h>
#include <stdio.h>
#include <ZDate.hpp>
#include <ZExcept.hpp>

ZDateTime::ZDateTime()
{
   ZFUNCTRACE_DEVELOP("ZDateTime::ZDateTime()");
   fromSeconds((long)time(0));
} // ZDateTime

ZDateTime::ZDateTime(const ZDateTime& aDateTime) :
   iYear(aDateTime.iYear),
   iMonth(aDateTime.iMonth),
   iDay(aDateTime.iDay),
   iHour(aDateTime.iHour),
   iMinute(aDateTime.iMinute),
   iSecond(aDateTime.iSecond)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::ZDateTime(const ZDateTime& aDateTime)");
} // ZDateTime

ZDateTime::ZDateTime(int aYear, int aMonth, int aDay, int aHour, int aMinute, int aSecond)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::ZDateTime(int aYear, int aMonth, int aDay, int aHour, int aMinute, int aSecond)");
   setYear(aYear);
   setMonth(aMonth);
   setDay(aDay);
   setHour(aHour);
   setMinute(aMinute);
   setSecond(aSecond);
} // ZDateTime

ZDateTime::~ZDateTime()
{
   ZFUNCTRACE_DEVELOP("ZDateTime::~ZDateTime()");
} // ~ZDateTime

ZDateTime& ZDateTime::setYear(int aYear)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setYear(int aYear)");
   iYear = aYear;
   return *this;
} // setYear

ZDateTime& ZDateTime::setMonth(int aMonth)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setMonth(int aMonth)");
   if (aMonth < 1 || aMonth > 12)
      ZTHROWEXC("Invalid month ("+ZString(aMonth)+")");
   iMonth = aMonth;
   return *this;
} // setMonth

ZDateTime& ZDateTime::setDay(int aDay)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setDay(int aDay)");
   if (aDay < 1 || aDay > 31)
      ZTHROWEXC("Invalid day ("+ZString(aDay)+")");
   iDay = aDay;
   return *this;
} // setDay

ZDateTime& ZDateTime::setHour(int aHour)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setHour(int aHour)");
   if (aHour < 0 || aHour > 23)
      ZTHROWEXC("Invalid hour ("+ZString(aHour)+")");
   iHour = aHour;
   return *this;
} // setHour

ZDateTime& ZDateTime::setMinute(int aMinute)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setMinute(int aMinute)");
   if (aMinute < 0 || aMinute > 59)
      ZTHROWEXC("Invalid month ("+ZString(aMinute)+")");
   iMinute = aMinute;
   return *this;
} // setMinute

ZDateTime& ZDateTime::setSecond(int aSecond)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setSecond(int aSecond)");
   if (aSecond < 0 || aSecond > 59)
      ZTHROWEXC("Invalid month ("+ZString(aSecond)+")");
   iSecond = aSecond;
   return *this;
} // setSecond

int ZDateTime::dayOfWeek() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::dayOfWeek() const");
   //long tt(asSeconds());
   time_t tt(asSeconds());
   struct tm * t2 = localtime(&tt);
   if (t2->tm_wday == 0)
      return sunday;
   else
      return t2->tm_wday;
} // dayOfWeek

int ZDateTime::dayOfYear() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::dayOfYear() const");
   //long tt(asSeconds());
   time_t tt(asSeconds());

   struct tm *t2 = localtime(&tt);
   return t2->tm_yday+1;
} // dayOfYear

int ZDateTime::iFormat = ZDateTime::euro;

int ZDateTime::format()
{
   ZFUNCTRACE_DEVELOP("ZDateTime::format()");
   return iFormat;
} // setFormat

void ZDateTime::setFormat(int aFormat)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::setFormat(int aFormat)");
   if (aFormat < 0 || aFormat > 2)
      ZTHROWEXC("Invalid format ("+ZString(aFormat)+")");
   iFormat = aFormat;
} // setFormat

ZString ZDateTime::asDate() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asDate() const");
   switch (iFormat) {
      case euro: return asEuroDate();
      case us  : return asUsDate();
      default  : ;
   } // switch
   return asIsoDate();
} // asDate

ZString ZDateTime::asIsoDate() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asIsoDate() const");
   char buf[12];
   sprintf(buf, "%04d.%02d.%02d", (int)iYear, (int)iMonth, (int)iDay);
   return ZString(buf);
} // asIsoDate

ZString ZDateTime::asEuroDate() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asEuroDate() const");
   char buf[12];
   sprintf(buf, "%02d.%02d.%04d", (int)iDay, (int)iMonth, (int)iYear);
   return ZString(buf);
} // asEuroDate

ZString ZDateTime::asUsDate() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asUsDate() const");
   char buf[12];
   sprintf(buf, "%02d/%02d/%04d", (int)iMonth, (int)iDay, (int)iYear);
   return ZString(buf);
} // asUsDate

ZString ZDateTime::asTime() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asTime() const");
   switch (iFormat) {
      case euro: return asEuroTime();
      case us  : return asUsTime();
      default  : ;
   } // switch
   return asIsoTime();
} // asTime

ZString ZDateTime::asIsoTime() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asIsoTime() const");
   char buf[12];
   sprintf(buf, "%02d.%02d.%02d", (int)iHour, (int)iMinute, (int)iSecond);
   return ZString(buf);
} // asIsoTime

ZString ZDateTime::asEuroTime() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asEuroTime() const");
   char buf[12];
   sprintf(buf, "%02d:%02d:%02d", (int)iHour, (int)iMinute, (int)iSecond);
   return ZString(buf);
} // asEuroTime

ZString ZDateTime::asUsTime() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asUsTime() const");
   return asIsoTime();
} // asUsTime

long ZDateTime::operator-(const ZDateTime& aDateTime) const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::operator-(const ZDateTime& aDateTime) const");
   return asSeconds()-aDateTime.asSeconds();
} // operator-

ZDateTime ZDateTime::operator-(long aSeconds) const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::operator-(long aSeconds) const");
   ZDateTime t(*this);
   t -= aSeconds;
   return t;
} // operator-

ZDateTime ZDateTime::operator+(long aSeconds) const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::operator+(long aSeconds) const");
   ZDateTime t(*this);
   t += aSeconds;
   return t;
} // operator+

ZDateTime& ZDateTime::operator-=(long aSeconds)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::operator-=(long aSeconds)");
   fromSeconds(asSeconds()-aSeconds);
   return *this;
} // operator-=

ZDateTime& ZDateTime::operator+=(long aSeconds)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::operator+=(long aSeconds)");
   fromSeconds(asSeconds()+aSeconds);
   return *this;
} // operator+=

long ZDateTime::asSeconds() const
{
   ZFUNCTRACE_DEVELOP("ZDateTime::asSeconds() const");
   if (iYear < 1902 || iYear > 2039)
      ZTHROWEXC("Cannot handle year outside 1902-2038 ("+ZString((int)iYear)+")");
   struct tm t;
   t.tm_year = iYear-1900;
   t.tm_mon  = iMonth-1;
   t.tm_mday = iDay;
   t.tm_hour = iHour;
   t.tm_min  = iMinute;
   t.tm_sec  = iSecond;
   return (long)mktime(&t);
} // asSeconds

void ZDateTime::fromSeconds(long aSeconds)
{
   ZFUNCTRACE_DEVELOP("ZDateTime::fromSeconds(long aSeconds)");
   
   time_t tt(aSeconds);

   struct tm *t = localtime(&tt);
   iYear   = t->tm_year+1900;
   iMonth  = t->tm_mon+1;
   iDay    = t->tm_mday;
   iHour   = t->tm_hour;
   iMinute = t->tm_min;
   iSecond = t->tm_sec;
} // fromSeconds
