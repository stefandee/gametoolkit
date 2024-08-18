//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as
// // published by the Free Software Foundation; either version 2.1 of the //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License  // for more details. //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Counter
 last modify: 13 06 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#ifndef PP_CounterH
#define PP_CounterH

class CPCounter {
public:
    CPCounter();
    CPCounter(int maxValue);

    CPCounter& operator++();
    CPCounter& operator++(int redundant);
    CPCounter& operator--();
    CPCounter& operator--(int redundant);
    int operator==(CPCounter& toCompare);
    int operator==(int value);
    CPCounter& operator=(const CPCounter& toInit);
    CPCounter& operator=(int value);
    operator int();
    int operator()();

    CPCounter& Reset();
    CPCounter& Set(int count = 0);
    CPCounter& Inc(int count = 1);
    CPCounter& Dec(int count = 1);
    CPCounter& ToValue(int wantedvalue, int step = 1);

    bool MaxReached();
    bool MinReached();

    int GetValueInterval();

    // CPCounter* GetObject();
    CPCounter* GetThis();

public:
    int GetCounter();
    void SetCounter(int value);

    int GetMinValue();
    void SetMinValue(int value);

    int GetMaxValue();
    void SetMaxValue(int value);

    bool GetCircular();
    void SetCircular(bool value);

    bool GetMinReached();
    void SetMinReached(bool value);

    bool GetMaxReached();
    void SetMaxReached(bool value);

    bool GetInfinite();
    void SetInfinite(bool value);

private:
    int mCounter; // the counter value
    int mMinValue; // the counter minimal value
    int mMaxValue; // the counter maximal value
    bool mCircular; // shows if the counter is circular
    bool mMaxReached; // shows if the counter iteration has reached te maximal
        // value
    bool mMinReached; // shows if the counter iteration has reached te maximal
        // value
    bool mInfinite; // show if the counter is an infinite one
        // if it is then every counter set will only modify the
        // counter value, but will not generate Was Min-Max Reached
};

/////////////////////////////////////////////////////////////////////////////
// GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 description: Counter
---------------------------------------------------------------------------*/
inline int CPCounter::GetCounter()
{
    return mCounter;
}

inline void CPCounter::SetCounter(int value)
{
    SetMinReached(false);
    SetMaxReached(false);
    mCounter = value;

    if (GetInfinite() == true) // means it iterates forever
    {
        return;
    }

    if (mCounter == GetMaxValue()) {
        SetMaxReached(true);
    } else {
        if (mCounter > GetMaxValue()) {
            if (GetCircular() == true) {
                mCounter = GetMinValue();
                SetMinReached(true);
            } else {
                mCounter = GetMaxValue();
                SetMaxReached(true);
            }
        } else if (mCounter == GetMinValue()) {
            SetMinReached(true);
        } else if (mCounter < GetMinValue()) {
            if (GetCircular() == true) {
                mCounter = GetMaxValue();
                SetMaxReached(true);
            } else {
                mCounter = GetMinValue();
                SetMinReached(true);
            }
        }
    }
}

/*---------------------------------------------------------------------------
 description: Min Count
---------------------------------------------------------------------------*/
inline int CPCounter::GetMinValue()
{
    return mMinValue;
}

inline void CPCounter::SetMinValue(int value)
{
    mMinValue = value;
    if (GetCounter() <= mMinValue)
        SetCounter(mMinValue);
}

/*---------------------------------------------------------------------------
 description: Max Count
---------------------------------------------------------------------------*/
inline int CPCounter::GetMaxValue()
{
    return mMaxValue;
}

inline void CPCounter::SetMaxValue(int value)
{
    mMaxValue = value;
    if (GetCounter() >= mMaxValue)
        SetCounter(mMaxValue);
}

/*---------------------------------------------------------------------------
 description: Circular
---------------------------------------------------------------------------*/
inline bool CPCounter::GetCircular()
{
    return mCircular;
}
inline void CPCounter::SetCircular(bool value)
{
    mCircular = value;
}

/*---------------------------------------------------------------------------
 description: Min Reached
---------------------------------------------------------------------------*/
inline bool CPCounter::GetMinReached()
{
    return mMinReached;
}

inline void CPCounter::SetMinReached(bool value)
{
    if (value == true)
        mMaxReached = false;
    mMinReached = value;
}

/*---------------------------------------------------------------------------
 description: Max Reached
---------------------------------------------------------------------------*/
inline bool CPCounter::GetMaxReached()
{
    return mMaxReached;
}

inline void CPCounter::SetMaxReached(bool value)
{
    if (value == true)
        mMinReached = false;
    mMaxReached = value;
}

/*---------------------------------------------------------------------------
 description: Infinite
---------------------------------------------------------------------------*/
inline bool CPCounter::GetInfinite()
{
    return mInfinite;
}

inline void CPCounter::SetInfinite(bool value)
{
    mInfinite = value;
}
//---------------------------------------------------------------------------

#endif

