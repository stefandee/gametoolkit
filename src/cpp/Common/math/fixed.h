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
/**************************************************************************

    Fixed.h - 16.16 Fixed point class

 **************************************************************************/

#ifndef FIXEDH
#define FIXEDH

//#include <windows.h>

// these two functions are not portable because of the MulDiv; lucky float ops
// got faster than they were 6 yrs ago :>
// eventually find an equivalent of MulDiv for *nix? 
inline long fixed_mul(long a, long b) { return (long)((float)a * b / 65536); /*return MulDiv(a, b, 65536);*/ }
inline long fixed_div(long a, long b) { return (long)((float)a * 65536 / b); /*return MulDiv(a, 65536, b);*/ }

class Fixed {                                      
    private:
        long fx;
    public:
        Fixed()             {}
        ~Fixed()            {}

        Fixed(long l)       {fx = l<<16;}
        Fixed(int i)        {fx = (long)i<<16;}
        Fixed(double d)     {fx = (long)(d * 65536.0);}

        inline int Int()           {return (int)(fx >> 16);}
        inline int Frac()          {return (int)(fx & 0xFFFF);}

        inline operator int()      {return (int)(fx >> 16);}
        inline operator double()   {return (double)fx / 65536.0;}

        Fixed operator +(Fixed a) {Fixed c; c.fx = fx + a.fx; return c;}
        Fixed operator -(Fixed a) {Fixed c; c.fx = fx - a.fx; return c;}
        Fixed operator *(Fixed a) {Fixed c; c.fx = fixed_mul(fx,a.fx); return c;}
        Fixed operator /(Fixed a) {Fixed c; c.fx = fixed_div(fx,a.fx); return c;}

        int operator <(Fixed a)  {return fx < a.fx;}
        int operator >(Fixed a)  {return fx > a.fx;}
        int operator ==(Fixed a) {return fx == a.fx;}
        int operator !=(Fixed a) {return fx != a.fx;}

//      Fixed& operator  =(Fixed a) {fx  = a.fx; return *this;}
        Fixed& operator +=(Fixed a) {fx += a.fx; return *this;}
        Fixed& operator -=(Fixed a) {fx -= a.fx; return *this;}
        Fixed& operator *=(Fixed a) {fx  = fixed_mul(fx,a.fx); return *this;}
        Fixed& operator /=(Fixed a) {fx  = fixed_div(fx,a.fx); return *this;}
};

#endif
