//---------------------------------------------------------------------------

#pragma hdrstop

#include "CfgXml.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CCfgXml::CCfgXml()
{
}
//---------------------------------------------------------------------------

CCfgXml::~CCfgXml()
{
}
//---------------------------------------------------------------------------


bool CCfgXml::GetResource(const char* resName, void* value)
{
  tinyxml2::XMLElement* root = mXmlDoc.RootElement();

  if (!root)
  {
    return false;
  }

  tinyxml2::XMLElement* elem = root->FirstChildElement(resName);

  if (!elem)
  {
    return false;
  }

  // query as int
  int tempInt;

  if (elem->QueryIntAttribute("value", &tempInt) == tinyxml2::XML_SUCCESS)
  {
    memcpy((__int32*)value, (__int32*)(&tempInt), 1 * sizeof(__int32));

    return true;
  }

  const char* tempStr;

  if (tempStr = elem->Attribute("value"))
  {
    strcpy((char*)value, tempStr);

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

TResourceType CCfgXml::GetResourceType(const char* resName)
{
  tinyxml2::XMLElement* root = mXmlDoc.RootElement();

  if (!root)
  {
    return RES_NOTHING;
  }

  tinyxml2::XMLElement* elem = root->FirstChildElement(resName);

  if (!elem)
  {
    return RES_NOTHING;
  }

  // query as int
  int tempInt;

  if (elem->QueryIntAttribute("value", &tempInt) == tinyxml2::XML_SUCCESS)
  {
    return RES_INT;
  }

  return RES_STRING256;
}
//---------------------------------------------------------------------------

bool CCfgXml::SetResource(const char* resName, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfgXml::AddResource(const char* resName, TResourceType resType, void* value)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfgXml::DeleteResource(const char* resName)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfgXml::Open(const char* fileName)
{
  auto loadResult = mXmlDoc.LoadFile(fileName);

  return loadResult == tinyxml2::XML_SUCCESS;
}
//---------------------------------------------------------------------------

bool CCfgXml::Create(const char* fileName)
{
  return false;
}
//---------------------------------------------------------------------------

bool CCfgXml::Empty()
{
  return mXmlDoc.Error();
}
//---------------------------------------------------------------------------

bool CCfgXml::Close()
{
  return false;
  // eventually save?
}
//---------------------------------------------------------------------------

