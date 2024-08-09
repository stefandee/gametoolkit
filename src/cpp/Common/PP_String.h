//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class String

 last modify: 15 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPSTRING_H
#define PPSTRING_H

class CPString
{
   public:
      CPString();
      CPString(const CPString& tocopy);
      CPString(const char*     text);
      CPString(int             value);
      CPString(long            value);
      CPString(float           value);
      CPString(double          value);
      ~CPString();

      operator const char*() const;

      CPString& operator= (const CPString &tocopy);
      CPString  operator+ (const CPString& toadd);
      CPString  operator+ (const char* toadd);
      bool      operator==(CPString tocompare) const;
      bool      operator<(CPString tocompare) const;
      bool      operator>(CPString tocompare) const;
      bool      operator!=(CPString tocompare) const;
      CPString  operator()(int start, int length);
      char&     operator[](int index);

      int       ToInt() const;
      double    ToDouble() const;

      void      ToLowerCase();
      void      ToUpperCase();

      CPString    SubString (int start, int length);
      void        Add       (const CPString& toadd);
      int         Length    () const;
      const char* c_str     () const;
      const char* Chars     () const;
      bool        isEmpty   () const;

      void        Replace(char _what, char _with);

   private:
      char     *text;
      void     deleteText();
      void     setText(const char* text);
      void     addText(const char* text);
};

CPString operator+(char *texttoadd, CPString& objecttoadd);
CPString operator+(const CPString& lhs, const CPString& rhs);

//---------------------------------------------------------------------------
#endif

