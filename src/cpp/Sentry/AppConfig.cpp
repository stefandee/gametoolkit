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

  GetColorProperty("general", "MODULES_BACKGROUND_COLOR", mModulesBackgroundColor);
  GetColorProperty("general", "MODULES_AXIS_COLOR", mModulesAxisColor);
  GetColorProperty("general", "MODULES_SELECTED_COLOR", mModulesSelectedColor);
  GetColorProperty("general", "MODULES_NOT_SELECTED_COLOR", mModulesNotSelectedColor);
  GetIntProperty("general", "MODULES_GRID_X_SPACING", mModulesGridXSpacing);
  GetIntProperty("general", "MODULES_GRID_Y_SPACING", mModulesGridYSpacing);
  GetColorProperty("general", "MODULES_GRID_COLOR", mModulesGridColor);
  GetIntProperty("general", "MODULES_SELECTION_EDGE", mModulesSelectionEdge);

  GetColorProperty("general", "FRAMES_BACKGROUND_COLOR", mFramesBackgroundColor);
  GetColorProperty("general", "FRAMES_AXIS_COLOR", mFramesAxisColor);
  GetIntProperty("general", "FRAMES_GRID_X_SPACING", mFramesGridXSpacing);
  GetIntProperty("general", "FRAMES_GRID_Y_SPACING", mFramesGridYSpacing);
  GetColorProperty("general", "FRAMES_GRID_COLOR", mFramesGridColor);
  GetColorProperty("general", "FRAMES_BOUND_RECT_COLOR", mFramesBoundRectColor);

  GetColorProperty("general", "ANIMS_BACKGROUND_COLOR", mAnimsBackgroundColor);
  GetColorProperty("general", "ANIMS_AXIS_COLOR", mAnimsAxisColor);
  GetColorProperty("general", "ANIMS_SELECT_COLOR", mAnimsSelectColor);
  GetIntProperty("general", "ANIMS_GRID_X_SPACING", mAnimsGridXSpacing);
  GetIntProperty("general", "ANIMS_GRID_Y_SPACING", mAnimsGridYSpacing);
  GetColorProperty("general", "ANIMS_GRID_COLOR", mAnimsGridColor);

  GetColorProperty("general", "FRAME_LOGIC_BACKGROUND_COLOR", mFrameLogicBackgroundColor);
  GetColorProperty("general", "FRAME_LOGIC_AXIS_COLOR", mFrameLogicAxisColor);
  GetColorProperty("general", "FRAME_LOGIC_SELECT_COLOR", mFrameLogicSelectColor);
  GetIntProperty("general", "FRAME_LOGIC_GRID_X_SPACING", mFrameLogicGridXSpacing);
  GetIntProperty("general", "FRAME_LOGIC_GRID_Y_SPACING", mFrameLogicGridYSpacing);
  GetColorProperty("general", "FRAME_LOGIC_GRID_COLOR", mFrameLogicGridColor);
  GetColorProperty("general", "FRAME_LOGIC_GRID_RECT_COLOR", mFrameLogicGridRectColor);
  GetColorProperty("general", "FRAME_LOGIC_GRID_POINT_COLOR", mFrameLogicGridPointColor);
  GetIntProperty("general", "FRAME_LOGIC_SELECTION_EDGE", mFrameLogicSelectionEdge);

  GetColorProperty("general", "COLOR_MAP_BACKGROUND_COLOR", mColorMapBackgroundColor);
  GetColorProperty("general", "COLOR_MAP_AXIS_COLOR", mColorMapAxisColor);
  GetIntProperty("general", "COLOR_MAP_GRID_X_SPACING", mColorMapGridXSpacing);
  GetIntProperty("general", "COLOR_MAP_GRID_Y_SPACING", mColorMapGridYSpacing);
  GetColorProperty("general", "COLOR_MAP_GRID_COLOR", mColorMapGridColor);

  GetIntProperty("general", "QUICK_MODULE_SIZE", mQuickModuleSize);
  GetColorProperty("general", "QUICK_MODULES_BACKGROUND_COLOR", mQuickModulesBackgroundColor);
  GetColorProperty("general", "QUICK_MODULES_ID_TEXT_COLOR", mQuickModulesIdTextColor);

  GetIntProperty("general", "QUICK_FRAME_SIZE", mQuickFrameSize);
  GetColorProperty("general", "QUICK_FRAMES_BACKGROUND_COLOR", mQuickFramesBackgroundColor);
  GetColorProperty("general", "QUICK_FRAMES_ID_TEXT_COLOR", mQuickFramesIDTextColor);

  GetIntProperty("general", "FILE_HISTORY_INDEX", mFileHistoryIndex);
  GetStrProperty("general", "SCRIPT_PATH", mPathScripts);

  // scripts
  GetStrProperty("general", "SCRIPT_PATH", mPathScripts);

  //
  // Read the file history related items
  //
  GetIntProperty("general", "FILE_HISTORY_INDEX", mFileHistoryIndex);

  int tmp;
  std::string strTmp;

  if (GetCategoryPropertyCount("filehistory", tmp))
  {
    for(int i = 0; i < tmp; i++)
    {
      if (!GetStrProperty("filehistory", "FILE" + std::to_string(i), strTmp))
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

  ConcatColorProperty(xmlDoc, "MODULES_BACKGROUND_COLOR", mModulesBackgroundColor);
  ConcatColorProperty(xmlDoc, "MODULES_AXIS_COLOR", mModulesAxisColor);
  ConcatColorProperty(xmlDoc, "MODULES_SELECTED_COLOR", mModulesSelectedColor);
  ConcatColorProperty(xmlDoc, "MODULES_NOT_SELECTED_COLOR", mModulesNotSelectedColor);
  ConcatIntProperty(xmlDoc, "MODULES_GRID_X_SPACING", mModulesGridXSpacing);
  ConcatIntProperty(xmlDoc, "MODULES_GRID_Y_SPACING", mModulesGridYSpacing);
  ConcatColorProperty(xmlDoc, "MODULES_GRID_COLOR", mModulesGridColor);
  ConcatIntProperty(xmlDoc, "MODULES_SELECTION_EDGE", mModulesSelectionEdge);

  ConcatColorProperty(xmlDoc, "FRAMES_BACKGROUND_COLOR", mFramesBackgroundColor);
  ConcatColorProperty(xmlDoc, "FRAMES_AXIS_COLOR", mFramesAxisColor);
  ConcatIntProperty(xmlDoc, "FRAMES_GRID_X_SPACING", mFramesGridXSpacing);
  ConcatIntProperty(xmlDoc, "FRAMES_GRID_Y_SPACING", mFramesGridYSpacing);
  ConcatColorProperty(xmlDoc, "FRAMES_GRID_COLOR", mFramesGridColor);
  ConcatColorProperty(xmlDoc, "FRAMES_BOUND_RECT_COLOR", mFramesBoundRectColor);

  ConcatColorProperty(xmlDoc, "ANIMS_BACKGROUND_COLOR", mAnimsBackgroundColor);
  ConcatColorProperty(xmlDoc, "ANIMS_AXIS_COLOR", mAnimsAxisColor);
  ConcatColorProperty(xmlDoc, "ANIMS_SELECT_COLOR", mAnimsSelectColor);
  ConcatIntProperty(xmlDoc, "ANIMS_GRID_X_SPACING", mAnimsGridXSpacing);
  ConcatIntProperty(xmlDoc, "ANIMS_GRID_Y_SPACING", mAnimsGridYSpacing);
  ConcatColorProperty(xmlDoc, "ANIMS_GRID_COLOR", mAnimsGridColor);

  ConcatColorProperty(xmlDoc, "FRAME_LOGIC_BACKGROUND_COLOR", mFrameLogicBackgroundColor);
  ConcatColorProperty(xmlDoc, "FRAME_LOGIC_AXIS_COLOR", mFrameLogicAxisColor);
  ConcatColorProperty(xmlDoc, "FRAME_LOGIC_SELECT_COLOR", mFrameLogicSelectColor);
  ConcatIntProperty(xmlDoc, "FRAME_LOGIC_GRID_X_SPACING", mFrameLogicGridXSpacing);
  ConcatIntProperty(xmlDoc, "FRAME_LOGIC_GRID_Y_SPACING", mFrameLogicGridYSpacing);
  ConcatColorProperty(xmlDoc, "FRAME_LOGIC_GRID_COLOR", mFrameLogicGridColor);
  ConcatColorProperty(xmlDoc, "FRAME_LOGIC_GRID_RECT_COLOR", mFrameLogicGridRectColor);
  ConcatColorProperty(xmlDoc, "FRAME_LOGIC_GRID_POINT_COLOR", mFrameLogicGridPointColor);
  ConcatIntProperty(xmlDoc, "FRAME_LOGIC_SELECTION_EDGE", mFrameLogicSelectionEdge);

  ConcatColorProperty(xmlDoc, "COLOR_MAP_BACKGROUND_COLOR", mColorMapBackgroundColor);
  ConcatColorProperty(xmlDoc, "COLOR_MAP_AXIS_COLOR", mColorMapAxisColor);
  ConcatIntProperty(xmlDoc, "COLOR_MAP_GRID_X_SPACING", mColorMapGridXSpacing);
  ConcatIntProperty(xmlDoc, "COLOR_MAP_GRID_Y_SPACING", mColorMapGridYSpacing);
  ConcatColorProperty(xmlDoc, "COLOR_MAP_GRID_COLOR", mColorMapGridColor);

  ConcatIntProperty(xmlDoc, "QUICK_MODULE_SIZE", mQuickModuleSize);
  ConcatColorProperty(xmlDoc, "QUICK_MODULES_BACKGROUND_COLOR", mQuickModulesBackgroundColor);
  ConcatColorProperty(xmlDoc, "QUICK_MODULES_ID_TEXT_COLOR", mQuickModulesIdTextColor);

  ConcatIntProperty(xmlDoc, "QUICK_FRAME_SIZE", mQuickFrameSize);
  ConcatColorProperty(xmlDoc, "QUICK_FRAMES_BACKGROUND_COLOR", mQuickFramesBackgroundColor);
  ConcatColorProperty(xmlDoc, "QUICK_FRAMES_ID_TEXT_COLOR", mQuickFramesIDTextColor);

  ConcatIntProperty(xmlDoc, "FILE_HISTORY_INDEX", mFileHistoryIndex);
  ConcatStrProperty(xmlDoc, "SCRIPT_PATH", mPathScripts);

  xmlDoc += "</general>\n";

  xmlDoc += "<filehistory>\n";

  for(unsigned int i = 0; i < mFileHistory.size(); i++)
  {
    ConcatStrProperty(xmlDoc, std::string("FILE") + std::to_string(i), mFileHistory[i]);
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

void AppConfig::ConcatColorProperty(std::string& src, std::string property, int color)
{
  src += std::string("<property name=\"" + property + "\" value=\"") + UTF8Encode(ColorToString((TColor)color)).c_str() + "\" />\n";
}
//---------------------------------------------------------------------------

void AppConfig::ConcatIntProperty(std::string& src, std::string property, int value)
{
  src += std::string("<property name=\"" + property + "\" value=\"") + std::to_string(value) + "\" />\n";
}
//---------------------------------------------------------------------------

void AppConfig::ConcatStrProperty(std::string& src, std::string property, std::string value)
{
  src += std::string("<property name=\"" + property + "\" value=\"") + value + "\" />\n";
}
//---------------------------------------------------------------------------

