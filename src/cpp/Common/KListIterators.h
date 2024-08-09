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
//  Unit              : List Iterators (KListIterators.h)  
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * "design pattern" id : Iterator; itereaza peste o lista 
//
//  Istorie           :
//    [Karg] - [10.04.2000] - l'am mutat din KList.h, caci nu are rost sa stea in
//                            acelasi fisier
//    [Karg] - [11.04.2000] - am adaugat iteratorul circular
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef KListIteratorH
#define KListIteratorH
//---------------------------------------------------------------------------

#include "KList.h"

// iteratorul abstract
template <class T> class CListIterator
{
  protected :
    CListIterator() {};

  public :
    virtual void First() = 0;
    virtual void Next()  = 0;
    virtual bool IsDone() = 0;
    virtual T*   CurrentItem() = 0;
    virtual void Delete() = 0;
};

// iteratorul normal - forward
template <class T> class CForwardListIterator : public CListIterator<T>
{
  private   :

  protected :
    CList<T>*  mList;
    int        mCurrent;

  public    :
    CForwardListIterator(CList<T>* _list);
    virtual void First();
    virtual void Next();
    virtual bool IsDone();
    virtual T*   CurrentItem();
    virtual void Delete();
};

template <class T> CForwardListIterator<T>::CForwardListIterator(CList<T>* _list)
{
  mCurrent = 0;
  mList    = _list;
}
//---------------------------------------------------------------------------

template <class T>void CForwardListIterator<T>::First()
{
  mCurrent = 0;
}
//---------------------------------------------------------------------------

template <class T>void CForwardListIterator<T>::Next()
{
  if (mList->Count() > mCurrent)
  {
    mCurrent++;
  }
}
//---------------------------------------------------------------------------

template <class T>bool CForwardListIterator<T>::IsDone()
{
  return (mCurrent == mList->Count());
}
//---------------------------------------------------------------------------

template <class T>T* CForwardListIterator<T>::CurrentItem()
{
  if (IsDone())
  {
    return NULL;
  }

  return mList->Get(mCurrent);
}
//---------------------------------------------------------------------------

template <class T>void CForwardListIterator<T>::Delete()
{
  mList->Delete(mCurrent);
}
//---------------------------------------------------------------------------

// iteratorul circular - forward
template <class T> class CCircularListIterator : public CForwardListIterator<T>
{
  public    :
    CCircularListIterator(CList<T>* _list) : CForwardListIterator<T>(_list) {};
    virtual void Last();
    virtual void Prev();
    virtual void Next();
    virtual bool IsDone();
};

template <class T>void CCircularListIterator<T>::Last()
{
  mCurrent = mList->Count() - 1;
}
//---------------------------------------------------------------------------

template <class T>void CCircularListIterator<T>::Next()
{
  mCurrent++;
  if (mCurrent >= mList->Count())
  {
    mCurrent = 0;
  }
}
//---------------------------------------------------------------------------

template <class T>void CCircularListIterator<T>::Prev()
{
  mCurrent--;
  if (mCurrent < 0)
  {
    mCurrent = mList->Count() - 1;
  }
}
//---------------------------------------------------------------------------

template <class T>bool CCircularListIterator<T>::IsDone()
{
  return false;
}
//---------------------------------------------------------------------------

// iteratorul normal - forward - lista de pointeri
template <class T> class CForwardPListIterator : public CListIterator<T>
{
  private   :

  protected :
    CPList<T>* mList;
    int        mCurrent;

  public    :
    CForwardPListIterator(CPList<T>* _list);
    virtual void First();
    virtual void Next();
    virtual bool IsDone();
    virtual T*   CurrentItem();
    virtual void Delete();
};

template <class T> CForwardPListIterator<T>::CForwardPListIterator(CPList<T>* _list)
{
  mCurrent = 0;
  mList    = _list;
}
//---------------------------------------------------------------------------

template <class T>void CForwardPListIterator<T>::First()
{
  mCurrent = 0;
}
//---------------------------------------------------------------------------

template <class T>void CForwardPListIterator<T>::Next()
{
  if (mList->Count() > mCurrent)
  {
    mCurrent++;
  }
}
//---------------------------------------------------------------------------

template <class T>bool CForwardPListIterator<T>::IsDone()
{
  return (mCurrent == mList->Count());
}
//---------------------------------------------------------------------------

template <class T>T* CForwardPListIterator<T>::CurrentItem()
{
  if (IsDone())
  {
    return NULL;
  }

  return mList->Get(mCurrent);
}
//---------------------------------------------------------------------------

template <class T>void CForwardPListIterator<T>::Delete()
{
  mList->Delete(mCurrent);
}
//---------------------------------------------------------------------------

// iteratorul circular - lista de pointeri
template <class T> class CCircularPListIterator : public CForwardPListIterator<T>
{
  public    :
    CCircularPListIterator(CPList<T>* _list) : CForwardPListIterator<T>(_list) {};
    virtual void Last();
    virtual void Prev();
    virtual void Next();
    virtual bool IsDone();
};

template <class T>void CCircularPListIterator<T>::Last()
{
  mCurrent = mList->Count() - 1;
}
//---------------------------------------------------------------------------

template <class T>void CCircularPListIterator<T>::Prev()
{
  mCurrent--;
  if (mCurrent < 0)
  {
    mCurrent = mList->Count() - 1;
  }
}
//---------------------------------------------------------------------------

template <class T>void CCircularPListIterator<T>::Next()
{
  mCurrent++;
  if (mCurrent >= mList->Count())
  {
    mCurrent = 0;
  }
}
//---------------------------------------------------------------------------

template <class T>bool CCircularPListIterator<T>::IsDone()
{
  return false;
}
//---------------------------------------------------------------------------

#endif
