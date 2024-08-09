//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Template Parameters list
//
//  Version           : 1.0
//
//  Description       :
//    * holds a generic list of parameters that is used for curves and terrain/eco
//      generators
//    * basically it is a STL map with a name :)
//    * the list has a name = identifier; a curve class will check the param list identifier
//      against the identifiers it may process and will change state accordingly
//    * it may also be used to transmit the parameters to generate a curve, for
//      example you may send x if you want y=f(x), or theta, if you curve is represented
//      in polar coordinates
//    * the two points above are, of course, valid if the template is instantiated
//      for a float/double/int/numeral type
//    * each parameter is an association between a string and another type
//    * i'm not a big fan of templates but in this case they prove useful
//
//  History           :
//    [28.10.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef TemplateParamListH
#define TemplateParamListH
//---------------------------------------------------------------------------

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "PP_String.h"
#include "easylogging++.h"

/*
template <class T> class TemplateParamAssociation
{
  public:
    T        mValue;
    CPString mId;
};
*/

template <class T> class TemplateParamList
{
  public: // c-d
    TemplateParamList() { mName = "Empty List"; }
    virtual ~TemplateParamList() {}
    //TemplateParamList(const TemplateParamList&);
    //TemplateParamList& operator =(const TemplateParamList&);

  public:
    virtual void SetName(CPString _name) { mName = _name; }
    virtual CPString GetName() const { return mName; }

    virtual void   SetDefaultValue(T _default) { mDefaultValue = _default; }
    virtual void   AddParam(CPString _id, T _value);
    virtual void   SetParam(CPString _id, T _value);
    virtual const T GetParam(CPString _id);
    virtual bool   Exists(CPString _name);
    std::vector<std::string> GetKeys();
    virtual void   PrintInfo();
    virtual void   Clear();

  private:
    CPString mName;
    T        mDefaultValue;

    typedef std::map<std::string, T, std::less<std::string> > TMap;
    typedef typename TMap::value_type                         TValue;

    //std::vector< TemplateParamAssociation<T> > mParamList;
    TMap mParamList;
};

/*
template <class T> TemplateParamList<T>::TemplateParamList(const TemplateParamList& _source)
{
  mName         = _source.mName;
  mDefaultValue = _source.mDefaultValue;
  mParamList    = _source.mParamList;
}
//---------------------------------------------------------------------------

template <class T> TemplateParamList<T>& TemplateParamList<T>::operator =(const TemplateParamList& _source)
{
  if (this == &_source)
  {
    return (*this);
  }

  mName         = _source.mName;
  mDefaultValue = _source.mDefaultValue;
  mParamList    = _source.mParamList;

  return (*this);
}
//---------------------------------------------------------------------------
*/

template <class T> void TemplateParamList<T>::AddParam(CPString _id, T _value)
{
  mParamList.insert(TValue(std::string(_id.c_str()), _value));
  //mParamList.insert(mParamList.begin(), TValue(_id, _value));
}
//---------------------------------------------------------------------------

template <class T> void TemplateParamList<T>::SetParam(CPString _id, T _value)
{
  typename TMap::iterator cur = mParamList.find(_id.c_str());

  //mParamList.erase(std::string(_id.c_str()));
  //mParamList.insert(TValue(std::string(_id.c_str()), _value));

  //(*cur).second = _value;

  if (cur != mParamList.end())
  {
    mParamList.erase(cur);
    mParamList.insert(TValue(std::string(_id.c_str()), _value));
  }
  else
  {
    mParamList.insert(TValue(std::string(_id.c_str()), _value));
  }
}
//---------------------------------------------------------------------------

template <class T> const T TemplateParamList<T>::GetParam(CPString _id)
{
  typename TMap::iterator cur = mParamList.find(_id.c_str());

  if (cur == mParamList.end())
  {
    // error?
  }

  return (*cur).second;

  /*
  for(int i = 0; i < mParamList.size(); i++)
  {
    if (mParamList[i].mId == _id)
    {
      return mParamList[i].mValue;
    }
  }

  return mDefaultValue;
  */
}
//---------------------------------------------------------------------------

template <class T> bool TemplateParamList<T>::Exists(CPString _name)
{
  return false;

  /*
  for(int i = 0; i < mParamList.size(); i++)
  {
    if (_name == mParamList[i].mId)
    {
      return true;
    }
  }

  return false;
  */
}
//---------------------------------------------------------------------------

template <class T> void TemplateParamList<T>::PrintInfo()
{
  typename TMap::iterator it = mParamList.begin();

  while(it != mParamList.end())
  {
    VLOG(9) << ((*it).first).c_str();
    it++;
  }
}
//---------------------------------------------------------------------------

template <class T> std::vector<std::string> TemplateParamList<T>::GetKeys()
{
  typename TMap::iterator           it = mParamList.begin();
  std::vector<std::string> lKeys;

  while(it != mParamList.end())
  {
    lKeys.push_back((*it).first);
    it++;
  }

  return lKeys;
}
//---------------------------------------------------------------------------

template <class T> void TemplateParamList<T>::Clear()
{
  mParamList.erase(mParamList.begin(), mParamList.end());
}
//---------------------------------------------------------------------------

#endif
