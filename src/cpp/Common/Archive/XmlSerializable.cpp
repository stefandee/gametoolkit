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
#include <cassert>
#include "easylogging++.h"
#include "PP_Stdlib.h"
#include "XmlSerializable.h"

/*
template<typename T> void XmlDeSerializer::readElementAttribute(const char* elementName, const char* name, T& value, T defaultValue)
{
    TiXmlNode* node = mCurrentElement ? mCurrentElement->FirstChild(elementName) : mDoc.FirstChild(elementName);

    if (node)
    {
        beginElement(elementName);
        readAttribute(name, value, defaultValue);
        endElement();
    }
    else
    {
        value = defaultValue;
    }
}
*/
//---------------------------------------------------------------------------

XmlDeSerializer::XmlDeSerializer(tinyxml2::XMLDocument& doc) : mCurrentElement(NULL)
{
    doc.DeepCopy(&mDoc);
}
//---------------------------------------------------------------------------

XmlDeSerializer::~XmlDeSerializer()
{

}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, bool& value)
{
    const char* str = mCurrentElement->Attribute(name);

    if (str == NULL)
    {
        LOG(FATAL) << name;
        throw;
    }

    CPString asCPString = CPString(str);
    asCPString.ToLowerCase();

    value = (asCPString == CPString("true"));
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, int& value)
{
    if( mCurrentElement->QueryIntAttribute(name, &value) != tinyxml2::XML_SUCCESS)
    {
        LOG(FATAL) << name;
        throw;
    }
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, CPString& value)
{
    const char* str = mCurrentElement->Attribute(name);

    if (str == NULL)
    {
        LOG(FATAL) << name;
        throw;
    }

    value = CPString(str);
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, std::string& value)
{
    const char* str = mCurrentElement->Attribute(name);

    if (str == NULL)
    {
        LOG(FATAL) << name;
        throw;
    }

    value = std::string(str);
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, bool& value, bool defaultValue )
{
    const char* str = mCurrentElement->Attribute(name);

    if (str != NULL)
    {
        CPString asCPString = CPString(str);
        asCPString.ToLowerCase();

        value = (asCPString == CPString("true"));
    }
    else
    {
        value = defaultValue;
    }
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, int& value, int defaultValue)
{
    if( mCurrentElement->QueryIntAttribute(name, &value) != tinyxml2::XML_SUCCESS)
    {
        value = defaultValue;
    }
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, CPString& value, const char* defaultValue)
{
    const char* str = mCurrentElement->Attribute(name);

    if (str == NULL)
    {
        value = CPString(defaultValue);
    }
    else
    {
        value = CPString(str);
    }
}
//---------------------------------------------------------------------------

void XmlDeSerializer::readAttribute(const char* name, std::string& value, const char* defaultValue)
{
    const char* str = mCurrentElement->Attribute(name);

    if (str == NULL)
    {
        value = std::string(defaultValue);
    }
    else
    {
        value = std::string(str);
    }
}
//---------------------------------------------------------------------------

void XmlDeSerializer::beginElement(const char* name)
{
    if (mCurrentElement == NULL)
    {
        mCurrentElement = mDoc.FirstChildElement(name);
    }
    else
    {
        mCurrentElement = mCurrentElement->FirstChildElement(name);
    }

    assert(mCurrentElement != NULL);
}
//---------------------------------------------------------------------------

void XmlDeSerializer::endElement()
{
    assert(mCurrentElement != NULL);

    if (mCurrentElement && mCurrentElement->Parent() != NULL)
    {
        mCurrentElement = mCurrentElement->Parent()->ToElement();
    }
}
//---------------------------------------------------------------------------

int XmlDeSerializer::readElementCount()
{
    tinyxml2::XMLNode* child = mCurrentElement->FirstChild();
    int count = 0;

    while( child != NULL )
    {
        if( child->ToComment() != NULL )
        {
            count++;
        }

        child = child->NextSibling();
    }

    return count;
}
//---------------------------------------------------------------------------

bool XmlDeSerializer::hasElement(const char* name)
{
    tinyxml2::XMLElement* element = mCurrentElement ? mCurrentElement->FirstChildElement(name) : mDoc.FirstChildElement(name);

    return element != NULL;
}
//---------------------------------------------------------------------------

XmlSerializer::XmlSerializer()
{
    mCurrentElement = NULL;
}
//---------------------------------------------------------------------------

XmlSerializer::~XmlSerializer()
{
    mCurrentElement = NULL;
}
//---------------------------------------------------------------------------

void XmlSerializer::createAttribute(const char* name, int value)
{
    assert(mCurrentElement != NULL);
    mCurrentElement->SetAttribute(name, value);
}
//---------------------------------------------------------------------------

void XmlSerializer::createAttribute(const char* name, bool value)
{
    assert(mCurrentElement != NULL);
    mCurrentElement->SetAttribute(name, value ? "true" : "false");
}
//---------------------------------------------------------------------------

void XmlSerializer::createAttribute(const char* name, const char* value)
{
    assert(mCurrentElement != NULL);
    mCurrentElement->SetAttribute(name, value);
}
//---------------------------------------------------------------------------

void XmlSerializer::rawWrite(int value)
{
    tinyxml2::XMLComment* comment = mDoc.NewComment(toString(value).c_str());

    mCurrentElement->LinkEndChild(comment);
}
//---------------------------------------------------------------------------

void XmlSerializer::beginElement(const char* name)
{
    tinyxml2::XMLElement* child = mDoc.NewElement(name);

    if (mCurrentElement == NULL)
    {
        mDoc.LinkEndChild(child);
    }
    else
    {
        mCurrentElement->LinkEndChild(child);
    }

    mCurrentElement = child;
}
//---------------------------------------------------------------------------

void XmlSerializer::endElement()
{
    assert(mCurrentElement != NULL);

    if (mCurrentElement && mCurrentElement->Parent() != NULL)
    {
        mCurrentElement = mCurrentElement->Parent()->ToElement();
    }
}
//---------------------------------------------------------------------------
