//---------------------------------------------------------------------------


#pragma hdrstop

#include "AppConfig.h"
#include "tinyxml2.h"
#include "Vcl.Graphics.hpp"
#include <sstream>

//---------------------------------------------------------------------------

using namespace tinyxml2;
#pragma package(smart_init)

AppConfig::AppConfig() : mState(APP_CONFIG_STATE_CLOSED)
{
  Default();
}
//---------------------------------------------------------------------------

bool AppConfig::Load(std::string fileName)
{
  mDoc.Clear();
  auto loadResult = mDoc.LoadFile(fileName.c_str());

  mState = APP_CONFIG_STATE_CLOSED;

  Default();

  if (loadResult != XML_SUCCESS)
  {
    return false;
  }

  mState = APP_CONFIG_STATE_SAVED;
  
  int tmp;
  std::string strTmp;

  GetStrProperty("general", "SCRIPT_PATH", mPathScripts);
  GetColorProperty("general", "MODULES_BACKGROUND_COLOR", mModulesBackgroundColor);
  GetColorProperty("general", "QUICK_MODULES_BACKGROUND_COLOR", mQuickModulesBackgroundColor);
  GetColorProperty("general", "QUICK_MODULES_ID_TEXT_COLOR", mQuickModulesIdTextColor);

  //
  // Read the file history related items
  //
  GetIntProperty("general", "FILE_HISTORY_INDEX", mFileHistoryIndex);

  if (GetCategoryPropertyCount("filehistory", tmp))
  {
    for(int i = 0; i < tmp; i++)
    {
      std::stringstream myStream;

      myStream << "FILE" << i;

      if (!GetStrProperty("filehistory", myStream.str(), strTmp))
      {
        continue;
      }

      mFileHistory.push_back(strTmp);
    }
  }

  return true;
}
//---------------------------------------------------------------------------

bool AppConfig::Save(std::string fileName)
{
  if (mState == APP_CONFIG_STATE_CLOSED)
  {
    return false;
  }

  mState = APP_CONFIG_STATE_SAVED;

  mDoc.Clear();

  std::string xmlDoc = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";

  xmlDoc += "<sentryconfig version=\"1.0\">\n";

  xmlDoc += "<general>\n";
  xmlDoc += std::string("<property name=\"MODULES_BACKGROUND_COLOR\" value=\"") + UTF8Encode(ColorToString((TColor)mModulesBackgroundColor)).c_str() + "\" />\n";
  xmlDoc += std::string("<property name=\"QUICK_MODULES_BACKGROUND_COLOR\" value=\"") + UTF8Encode(ColorToString((TColor)mQuickModulesBackgroundColor)).c_str() + "\" />\n";
  xmlDoc += std::string("<property name=\"QUICK_MODULES_ID_TEXT_COLOR\" value=\"") + UTF8Encode(ColorToString((TColor)mQuickModulesIdTextColor)).c_str() + "\" />\n";
  xmlDoc += std::string("<property name=\"FILE_HISTORY_INDEX\" value=\"") + IntToStr(mFileHistoryIndex) + "\" />\n";
  xmlDoc += std::string("<property name=\"SCRIPT_PATH\" value=\"") + mPathScripts + "\" />\n";
  xmlDoc += "</general>\n";

  xmlDoc += "<filehistory>\n";

  for(unsigned int i = 0; i < mFileHistory.size(); i++)
  {
    std::ostringstream os;
    os << i;

    xmlDoc += std::string("<property name=\"FILE") + os.str() + "\" value=\"" + mFileHistory[i] + "\"/>\n";
  }

  xmlDoc += "</filehistory>\n";

  xmlDoc += "</sentryconfig>\n";

  mDoc.Parse(xmlDoc.c_str());

  mDoc.SaveFile(fileName.c_str());

  return true;
}
//---------------------------------------------------------------------------

void AppConfig::Default()
{
  mModulesBackgroundColor        = MODULES_BACKGROUND_COLOR;
  mModulesAxisColor              = MODULES_AXIS_COLOR;
  mModulesSelectedColor          = MODULES_SELECTED_COLOR;
  mModulesNotSelectedColor       = MODULES_NOT_SELECTED_COLOR;
  mModulesGridXSpacing           = MODULES_GRID_X_SPACING;  
  mModulesGridYSpacing           = MODULES_GRID_Y_SPACING;  
  mModulesGridColor              = MODULES_GRID_COLOR;
  mModulesSelectionEdge          = MODULES_SELECTION_EDGE; // not used?   
                                                               
  mFramesBackgroundColor         = FRAMES_BACKGROUND_COLOR;
  mFramesAxisColor               = FRAMES_AXIS_COLOR; 
  mFramesGridXSpacing            = FRAMES_GRID_X_SPACING;   
  mFramesGridYSpacing            = FRAMES_GRID_Y_SPACING;   
  mFramesGridColor               = FRAMES_GRID_COLOR;
  mFramesBoundRectColor          = FRAMES_BOUND_RECT_COLOR;

  mAnimsBackgroundColor          = ANIMS_BACKGROUND_COLOR;
  mAnimsAxisColor                = ANIMS_AXIS_COLOR;
  mAnimsSelectColor              = ANIMS_SELECT_COLOR;
  mAnimsGridXSpacing             = ANIMS_GRID_X_SPACING;
  mAnimsGridYSpacing             = ANIMS_GRID_Y_SPACING;
  mAnimsGridColor                = ANIMS_GRID_COLOR;

  mFrameLogicBackgroundColor     = FRAME_LOGIC_BACKGROUND_COLOR;
  mFrameLogicAxisColor           = FRAME_LOGIC_AXIS_COLOR;
  mFrameLogicSelectColor         = FRAME_LOGIC_SELECT_COLOR; // not used?
  mFrameLogicGridXSpacing        = FRAME_LOGIC_GRID_X_SPACING;
  mFrameLogicGridYSpacing        = FRAME_LOGIC_GRID_Y_SPACING;
  mFrameLogicGridColor           = FRAME_LOGIC_GRID_COLOR;
  mFrameLogicGridRectColor       = FRAME_LOGIC_GRID_RECT_COLOR;
  mFrameLogicGridPointColor      = FRAME_LOGIC_GRID_POINT_COLOR;
  mFrameLogicSelectionEdge       = FRAME_LOGIC_SELECTION_EDGE;

  mColorMapBackgroundColor       = COLOR_MAP_BACKGROUND_COLOR;
  mColorMapAxisColor             = COLOR_MAP_AXIS_COLOR;
  mColorMapGridXSpacing          = COLOR_MAP_GRID_X_SPACING;
  mColorMapGridYSpacing          = COLOR_MAP_GRID_Y_SPACING;
  mColorMapGridColor             = COLOR_MAP_GRID_COLOR;

  mQuickModuleSize               = QUICK_MODULE_SIZE;
  mQuickModulesBackgroundColor   = QUICK_MODULES_BACKGROUND_COLOR;
  mQuickModulesIdTextColor       = QUICK_MODULES_ID_TEXT_COLOR;
                                 
  mQuickFrameSize                = QUICK_FRAME_SIZE;      
  mQuickFramesBackgroundColor    = QUICK_FRAMES_BACKGROUND_COLOR;
  mQuickFramesIDTextColor        = QUICK_FRAMES_ID_TEXT_COLOR;
  
  mPathScripts = DEFAULT_PATH_SCRIPTS;

  mFileHistoryIndex = 0;
  mFileHistory.clear();
}
//---------------------------------------------------------------------------
                                 
void AppConfig::AddFileToHistory(std::string fileName)
{
  std::vector<std::string>::iterator it = std::find(mFileHistory.begin(), mFileHistory.end(), fileName);

  if (it != mFileHistory.end())
  {
    // the item is already in the list; make it the most recent by moving it to the first vector index
    if (it != mFileHistory.begin())
    {
      mFileHistory.erase(it);
      mFileHistory.insert(mFileHistory.begin(), fileName);
    }
  }
  else
  {
    // the filename was not found, so insert it at the beginning of the list;
    // then erase the last element if the vector size is larger than FILE_HISTORY_MAX_ITEMS
    mFileHistory.insert(mFileHistory.begin(), fileName);

    if (mFileHistory.size() > FILE_HISTORY_MAX_ITEMS)
    {
      mFileHistory.pop_back();
    }

    mState = APP_CONFIG_STATE_MODIFIED;
  }
}
//---------------------------------------------------------------------------

bool AppConfig::GetStrProperty(std::string category, std::string name, std::string& value )
{
  if (mState == APP_CONFIG_STATE_CLOSED)
  {
    return false;
  }

  auto nodeRoot = mDoc.FirstChildElement("sentryconfig");

  if (!nodeRoot)
  {
    return false;
  }

  auto nodeGeneral = nodeRoot->FirstChild();

  if (nodeGeneral)
  {
    do
    {
      if (std::string(nodeGeneral->Value()) == category)
      {
        auto nodeProperty = nodeGeneral->FirstChildElement( "property" );

        if (nodeProperty)
        {
          do
          {
            auto elem = nodeProperty->ToElement();

            if (!elem)
            {
              continue;
            }

            const char* propertyName = elem->Attribute( "name" );
            const char* propertyValue = elem->Attribute( "value" );

            if (propertyName && propertyValue)
            {
              if (std::string(propertyName) == name)
              {
                value = std::string(propertyValue);

                return true;
              }
            }
          }
          while((nodeProperty = nodeProperty->NextSiblingElement( "property" )) != 0);
        }  
      }
    }
    while((nodeGeneral = nodeGeneral->NextSibling()) != 0);
  }

  return false;
}
//---------------------------------------------------------------------------

bool AppConfig::GetIntProperty(std::string category, std::string name, int& value)
{
  std::string tmpStr;

  if (GetStrProperty(category, name, tmpStr))
  {
    value = atoi(tmpStr.c_str());

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool AppConfig::GetColorProperty(std::string category, std::string name, int& value)
{
  std::string tmpStr;

  if (GetStrProperty(category, name, tmpStr))
  {
    value = StringToColor(tmpStr.c_str());

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool AppConfig::GetCategoryPropertyCount(std::string category, int& count)
{
  count = -1;

  if (mState == APP_CONFIG_STATE_CLOSED)
  {
    return false;
  }

  auto nodeRoot = mDoc.FirstChildElement("sentryconfig");

  if (!nodeRoot)
  {
    return false;
  }

  auto nodeGeneral = nodeRoot->FirstChild();

  if (nodeGeneral)
  {
    do
    {
      if (std::string(nodeGeneral->Value()) == category)
      {
        count = 0;
        
        auto nodeProperty = nodeGeneral->FirstChildElement( "property" );

        if (nodeProperty)
        {
          do
          {
            auto elem = nodeProperty->ToElement();

            if (!elem)
            {
              continue;
            }

            const char* propertyName = elem->Attribute( "name" );
            const char* propertyValue = elem->Attribute( "value" );

            if (propertyName && propertyValue)
            {
              count++;
            }
          }
          while((nodeProperty = nodeProperty->NextSiblingElement( "property" )) != 0);
        }
      }
    }
    while((nodeGeneral = nodeGeneral->NextSibling()) != 0);
  }

  return true;
}
//---------------------------------------------------------------------------

std::string AppConfig::IntToStr(int x)
{
  std::ostringstream os;
  os << x;

  return os.str();
}
//---------------------------------------------------------------------------

