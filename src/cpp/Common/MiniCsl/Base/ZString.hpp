/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZString.hpp
 * Application :  IBK Open Class Library
 * Purpose     :  ZString is the string type. Besides of text it is also
 *                capable of handling binary data.
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.20  First implementation                        P.Koch, IBK
 * 2001.12.03  Supressed all extern link defines           Karg
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

#ifndef _ZSTRING_
#define _ZSTRING_

#include <istream>
#include <ostream>
#include <iostream>
#include "ZBase.hpp"

class ZStringBuffer : public ZBase
{
   public:
      // default constructor
      ZStringBuffer();

      // constructor with size specification
      ZStringBuffer(long aSize);

      // copy constructor
      ZStringBuffer(ZStringBuffer& aBuffer);

      // destructor
      ~ZStringBuffer();

      // add a reference
      void addRef() { iRefCount++; }

      // remove a reference
      void removeRef();

      // get # of references
      long refs() const { return iRefCount; }

      // get buffer logical size
      long size() const { return iLogSize; }

      // rezize buffer
      void setSize(long aSize);

      // data buffer access
      operator char*() const { return iData; }
      char* data() const { return iData; }

   private:
      long iRefCount;           // # of references
      long iLogSize;            // logical buffer size
      long iPhySize;            // physical buffer size
      char* iData;              // data buffer
}; // ZStringBuffer


class ZString : public ZBase
{
   public:
      // default constructor
      ZString();

      // copy constructor
      ZString(const ZString& aString);

      // constructor with given text
      ZString(const char *aText);
      ZString(const signed char *aText);
      ZString(const unsigned char *aText);

      // constructor with given binary data
      ZString(const void *aData, long aSize);

      // constructor with given char
      ZString(char aChar);
      ZString(signed char aChar);
      ZString(unsigned char aChar);

      // construct from integer value
      ZString(signed short aValue);
      ZString(unsigned short aValue);
      ZString(signed aValue);
      ZString(unsigned aValue);
      ZString(signed long aValue);
      ZString(unsigned long aValue);

      // construct from floating value
      ZString(double aValue);

      // destructor
      ~ZString();

      // get size
      long size() const { return iBuffer->size(); }
      long length() const { return iBuffer->size(); }

      // data buffer access
      operator char*();
      operator const char*() const { return *iBuffer; };
      char* buffer();
      const char* constBuffer() const { return *iBuffer; };

      char& operator[](int aIndex);
      char& operator[](unsigned aIndex);
      char& operator[](long aIndex);
      char& operator[](unsigned long aIndex);

      const char& operator[](int aIndex) const;
      const char& operator[](unsigned aIndex) const;
      const char& operator[](long aIndex) const;
      const char& operator[](unsigned long aIndex) const;

      // numeric queries
      int asInt() const;
      long asLong() const;
      unsigned asUnsigned() const;
      unsigned long asUnsignedLong() const;
      double asDouble() const;

      // other queries & testing
      long
         indexOf(
            const ZString& aString,
            long aStartPos = 1,
            ZBoolean aIgnoreCase = zFalse) const;

      long
         indexOf(
            char aChar,
            long aStartPos = 1) const;

      long
         indexOf(
            signed char aChar,
            long aStartPos = 1) const;

      long
         indexOf(
            unsigned char aChar,
            long aStartPos = 1) const;

      long
         lastIndexOf(
            const ZString& aString,
            long aStartPos = maxLong,
            ZBoolean aIgnoreCase = zFalse) const;

      long
         lastIndexOf(
            char aChar,
            long aStartPos = maxLong) const;

      long
         lastIndexOf(
            signed char aChar,
            long aStartPos = maxLong) const;

      long
         lastIndexOf(
            unsigned char aChar,
            long aStartPos = maxLong) const;

      long
         occurrencesOf(
            char aChar) const;

      ZBoolean
         isPrintable() const;

      ZString
         subString(
            long aStart,
            long aCount = -1,
            char aPadChar = ' ') const;

      // compares
      friend ZBoolean
         operator == (
            const ZString &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator == (
            const ZString &aString1,
            const char* aString2);

      friend ZBoolean
         operator == (
            const char* &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator != (
            const ZString &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator != (
            const ZString &aString1,
            const char* aString2);

      friend ZBoolean
         operator != (
            const char* &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator <  (
            const ZString &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator <  (
            const ZString &aString1,
            const char* aString2);

      friend ZBoolean
         operator <  (
            const char* &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator <= (
            const ZString &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator <= (
            const ZString &aString1,
            const char* aString2);

      friend ZBoolean
         operator <= (
            const char* &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator >  (
            const ZString &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator >  (
            const ZString &aString1,
            const char* aString2);

      friend ZBoolean
         operator >  (
            const char* &aString1,
            const ZString &aString2);

      friend ZBoolean
         operator >= (
            const ZString &aString1,
            const ZString& aString2);

      friend ZBoolean
         operator >= (
            const ZString &aString1,
            const char* aString2);

      friend ZBoolean
         operator >= (
            const char* &aString1,
            const ZString& aString2);

      // assignment and concatination
      ZString& operator = (const ZString& aString);
      ZString& operator = (const char* aString);

      ZString& operator += (const ZString& aString);
      ZString& operator += (const char* aString);
      ZString& operator += (char aChar);

      ZString operator + (const ZString& aString) const;
      ZString operator + (const char* aString) const;
      ZString operator + (char aChar) const;

      friend ZString
         operator + (
            const char* aString1,
            const ZString& aString2);

      friend ZString
         operator + (
            char aChar,
            const ZString& aString2);

      // manipulations
      ZString& lowerCase();
      ZString& upperCase();

      static ZString
         lowerCase(
            const ZString& aString);

      static ZString
         upperCase(
            const ZString& aString);

      ZString&
         center(
            long aLength,
            char aPadChar = ' ');

      ZString&
         leftJustify(
            long aLength,
            char aPadChar = ' ');

      ZString&
         rightJustify(
            long aLength,
            char aPadChar = ' ');

      ZString&
         strip(
            char aChar = ' ');

      ZString&
         stripLeading(
            char aChar = ' ');

      ZString&
         stripTrailing(
            char aChar = ' ');

      ZString&
         change(
            const ZString &aFrom,
            const ZString &aTo,
            long aStart = 1,
            long aMaxChanges = maxLong,
            ZBoolean aIgnoreCase = zFalse);

      // word operations
      long wordCount() const;

      ZString word(long aIndex) const
      {
         return words(aIndex,1);
      } // word

      ZString
         words(
            long aFirst,
            long aCount = maxLong) const;

      static ZString
         removeWords(
            const ZString& aString,
            long aFirst,
            long aCount = maxLong);

      ZString&
         removeWords(
            long aFirst,
            long aCount = maxLong);

      // conversions
      ZString& imp();
      ZString& exp();
      static ZString imp(const ZString& aString);
      static ZString exp(const ZString& aString);

      static ZString hex(char aValue);
      static ZString hex(signed char aValue);
      static ZString hex(unsigned char aValue);
      static ZString hex(signed aValue);
      static ZString hex(unsigned aValue);
      static ZString hex(signed long aValue);
      static ZString hex(unsigned long aValue);
      static ZString hex(void* aPointer);

      // stream i/o
      friend std::ostream& operator << (std::ostream &aStream, const ZString& aString);
      friend std::istream& operator >> (std::istream &aStream, ZString &aString);
      static ZString lineFrom(std::istream &aStream, char aDelim = '\n');

   private:
      ZStringBuffer *iBuffer;            // string buffer
}; // ZString

#endif // _ZSTRING_
