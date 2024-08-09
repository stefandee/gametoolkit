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
//-----------------------------------------------------------------------------
//  Unit              : String class
//
//  Versiune          : 1.01
//
//  Descriere         :
//    * o clasa de stringuri asemanatoare cu AnsiString'ul din VCL
//    * are cam toate facilitatile, cu exceptia Insert, Pos, Trim, Delimiter
//
//  Istorie           :
//    [15.04.2000] - [grabX] - creat/scris prima data
//    [27.10.2001] - [Karg]  - gasit si corectat bug la operator () -
//                             in loc de delete [] result era delete result (thanks grabX! :)
//    [04.04.2004] - Karh    - added a Replace method
//-----------------------------------------------------------------------------
#include <ctype.h>
#pragma hdrstop

#include "PP_String.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: default constructor
---------------------------------------------------------------------------*/
CPString::CPString()
{
   text = new char[1];
   text[0] = '\0';
}
/*---------------------------------------------------------------------------
 description: copy constructor
---------------------------------------------------------------------------*/
CPString::CPString(const CPString& tocopy)
{
   text = NULL;

   if (tocopy)
   {
     setText(tocopy.text);
   }
}
/*---------------------------------------------------------------------------
 description: custom char * constructor
---------------------------------------------------------------------------*/
CPString::CPString(const char* text)
{
   this->text = NULL;

   if (text)
   {
     setText(text);
   }
}
/*---------------------------------------------------------------------------
 description: custom integer constructor
---------------------------------------------------------------------------*/
CPString::CPString(int value)
{
   text = NULL;
   text = new char[16];
   sprintf(text, "%d", value);
}

/*---------------------------------------------------------------------------
 description: custom long constructor
---------------------------------------------------------------------------*/
CPString::CPString(long value)
{
   text = NULL;
   text = new char[16];
   sprintf(text, "%ld", value);
}
/*---------------------------------------------------------------------------
 description: custom float constructor
---------------------------------------------------------------------------*/
CPString::CPString(float value)
{
   text = NULL;
   text = new char[32];
   sprintf(text, "%f", value);
}
/*---------------------------------------------------------------------------
 description: custom double constructor
---------------------------------------------------------------------------*/
CPString::CPString(double value)
{
   text = NULL;
   text = new char[32];
   sprintf(text, "%lf", value);
}

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPString::~CPString()
{
   deleteText();
}

/*---------------------------------------------------------------------------
 description: cast to const char*
---------------------------------------------------------------------------*/
CPString ::operator const char*() const
{
   return text;
}
/*---------------------------------------------------------------------------
 description: operator =
---------------------------------------------------------------------------*/
CPString& CPString::operator=(const CPString& tocopy)
{
   if (this != &tocopy)
      setText(tocopy.text);
   return *this;
}

/*---------------------------------------------------------------------------
 description: operator + CPString
---------------------------------------------------------------------------*/
CPString CPString::operator+(const CPString& toadd)
{
   CPString result;
   result = *this;
   result.addText(toadd.text);
   return result;
}
/*---------------------------------------------------------------------------
 description: operator + char*
---------------------------------------------------------------------------*/
CPString CPString::operator+(const char* toadd)
{
   CPString result;
   result = *this;
   result.addText(toadd);
   return result;
}
/*---------------------------------------------------------------------------
 description: operator ==
---------------------------------------------------------------------------*/
bool CPString::operator==(CPString tocompare) const
{
   return strcmp(text, tocompare.text) == 0;
}

/*---------------------------------------------------------------------------
 description: operator <
---------------------------------------------------------------------------*/
bool CPString::operator<(CPString tocompare) const
{
	return false;
}

/*---------------------------------------------------------------------------
 description: operator >
---------------------------------------------------------------------------*/
bool CPString::operator>(CPString tocompare) const
{
	return false;
}

/*---------------------------------------------------------------------------
 description: operator !=
---------------------------------------------------------------------------*/
bool CPString::operator!=(CPString tocompare) const
{
   return strcmp(text, tocompare.text) != 0;
}
/*---------------------------------------------------------------------------
 description: gets count chars from start
---------------------------------------------------------------------------*/
CPString CPString::operator()(int start, int count)
{
   CPString res;
   char *result;
   if (start < 0)
      start = 0;
   if (start >= (int)strlen(text))
   {
      result = new char[1];
      result[0] = '\0';
   }
   else
   {
      if (count - start >=(int) strlen(text))
         count = strlen(text) - start;
      result = new char[count+1];
      strncpy(result, &text[start], count);
      result[count] = '\0';
   }
   res = result;
   delete [] result;
   return res;
}
/*---------------------------------------------------------------------------
 description: gets the index char
---------------------------------------------------------------------------*/
char& CPString::operator[](int index)
{
   if (index < 0)
      index = 0;
   if (index >=(int) strlen(text))
      index = strlen(text)-1;
   return text[index];
}

/*---------------------------------------------------------------------------
 description: converts the string to int
---------------------------------------------------------------------------*/
int CPString::ToInt() const
{
   return atoi(text);
}
/*---------------------------------------------------------------------------
 description: converts the string to double
---------------------------------------------------------------------------*/
double CPString::ToDouble() const
{
   return atof(text);
}

void CPString::ToLowerCase()
{
  int lLength = Length();

  for(int i = 0; i < lLength; i++)
  {
    text[i] = tolower(text[i]);
  }
}

void CPString::ToUpperCase()
{
  int lLength = Length();

  for(int i = 0; i < lLength; i++)
  {
    text[i] = toupper(text[i]);
  }
}

/*---------------------------------------------------------------------------
 description: same as operator ()
 parameters : same as operator ()
---------------------------------------------------------------------------*/
CPString CPString::SubString(int start, int length)
{
   return operator()(start, length);
}

/*---------------------------------------------------------------------------
 description: adds text into
---------------------------------------------------------------------------*/
void CPString::Add(const CPString &toadd)
{
   addText(toadd.text);
}
/*---------------------------------------------------------------------------
 description: gets the text length
---------------------------------------------------------------------------*/
int CPString::Length() const
{
   return strlen(text);
}
/*---------------------------------------------------------------------------
 description: gets the char*
---------------------------------------------------------------------------*/
const char* CPString::c_str() const
{
   return text;
}
/*---------------------------------------------------------------------------
 description: gets the char*
---------------------------------------------------------------------------*/
const char* CPString::Chars() const
{
   return text;
}

/*---------------------------------------------------------------------------
 description: true if empty
---------------------------------------------------------------------------*/
bool CPString::isEmpty() const
{
   return (Length() == 0);
}

/*---------------------------------------------------------------------------
 description: replaces all occurences of the character _what with caracter _with
---------------------------------------------------------------------------*/
void CPString::Replace(char _what, char _with)
{
  int lLength = Length();

  for(int i = 0; i < lLength; i++)
  {
    if (text[i] == _what)
    {
      text[i] = _with;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
void CPString::deleteText()
{
   if (text != NULL)
   {
      delete [](this->text);
      text = 0;
   }
}
/*---------------------------------------------------------------------------
 description: sets the text and length values
---------------------------------------------------------------------------*/
void CPString::setText(const char *text)
{
   int length;
   deleteText();
   length = strlen(text);
   this->text = new char[length + 1];
   strcpy(this->text, text);
}
/*---------------------------------------------------------------------------
 description: adds the char* to the current text data
---------------------------------------------------------------------------*/
void CPString::addText(const char *text)
{
   char* tmptext = new char[strlen(this->text)+strlen(text)+1];
   strcpy(tmptext, this->text);
   strncat(tmptext, text, strlen(text));
   deleteText();
   setText(tmptext);
   delete []tmptext;
}
/////////////////////////////////////////////////////////////////////////////
//REVERSE OPERATORS SECTION
/////////////////////////////////////////////////////////////////////////////
CPString operator+(char *texttoadd, CPString& objecttoadd)
{
   CPString result;
   result = texttoadd;
   return result + objecttoadd;
}
//---------------------------------------------------------------------------

CPString operator+(const CPString& lhs, const CPString& rhs)
{
   CPString result;
   result.Add(lhs);
   result.Add(rhs);

   return result;
}
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

