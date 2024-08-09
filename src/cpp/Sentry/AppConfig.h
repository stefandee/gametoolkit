//---------------------------------------------------------------------------

#ifndef AppConfigH
#define AppConfigH
//---------------------------------------------------------------------------

#include <string>
#include <vector>
#include "tinyxml.h"
//---------------------------------------------------------------------------

enum TAppConfigState { APP_CONFIG_STATE_NEW, APP_CONFIG_STATE_SAVED, APP_CONFIG_STATE_CLOSED };
//---------------------------------------------------------------------------

const int MODULES_BACKGROUND_COLOR = 0xFFFFFF;
const int MODULES_AXIS_COLOR = 0xFF0000;
const int MODULES_SELECTED_COLOR = 0x0000FF;
const int MODULES_NOT_SELECTED_COLOR = 0x00AA66;
const int MODULES_GRID_X_SPACING = 16;
const int MODULES_GRID_Y_SPACING = 16;
const int MODULES_GRID_COLOR = 0x440011;
const int MODULES_SELECTION_EDGE = 5;

const int FRAMES_BACKGROUND_COLOR = 0xFFFFFF;
const int FRAMES_AXIS_COLOR = 0xFF0000;
const int FRAMES_GRID_X_SPACING = 16;
const int FRAMES_GRID_Y_SPACING = 16;
const int FRAMES_GRID_COLOR = 0x440011;
const int FRAMES_BOUND_RECT_COLOR = 0xFF00FF;

const int ANIMS_BACKGROUND_COLOR = 0xFFFFFF;
const int ANIMS_AXIS_COLOR = 0xFF0000;
const int ANIMS_SELECT_COLOR = 0x0000FF;
const int ANIMS_GRID_X_SPACING = 16;
const int ANIMS_GRID_Y_SPACING = 16;
const int ANIMS_GRID_COLOR = 0x440011;

const int FRAME_LOGIC_BACKGROUND_COLOR = 0xFFFFFF;
const int FRAME_LOGIC_AXIS_COLOR = 0xFF0000;
const int FRAME_LOGIC_SELECT_COLOR = 0x0000FF;
const int FRAME_LOGIC_GRID_X_SPACING = 16;
const int FRAME_LOGIC_GRID_Y_SPACING = 16;
const int FRAME_LOGIC_GRID_COLOR = 0x440011;
const int FRAME_LOGIC_GRID_RECT_COLOR  = 0x44FF00;
const int FRAME_LOGIC_GRID_POINT_COLOR = 0x0044FF;
const int FRAME_LOGIC_SELECTION_EDGE = 5;

const int COLOR_MAP_BACKGROUND_COLOR = 0xFFFFFF;
const int COLOR_MAP_AXIS_COLOR       = 0xFF0000;
const int COLOR_MAP_GRID_X_SPACING   = 16;
const int COLOR_MAP_GRID_Y_SPACING   = 16;
const int COLOR_MAP_GRID_COLOR       = 0x440011;

const int QUICK_MODULE_SIZE = 80;
const int QUICK_MODULES_BACKGROUND_COLOR = 0xFFFFFF;
const int QUICK_MODULES_ID_TEXT_COLOR = 0x00EE00;

const int QUICK_FRAME_SIZE  = 80;
const int QUICK_FRAMES_BACKGROUND_COLOR = 0xFFFFFF;
const int QUICK_FRAMES_ID_TEXT_COLOR = 0x00EE00;

const int FILE_HISTORY_MAX_ITEMS = 8;

const char* const DEFAULT_PATH_SCRIPTS = ".\\scripts";
//---------------------------------------------------------------------------

class AppConfig
{
  public:
    AppConfig();
    virtual ~AppConfig() {}

  public:
    bool Load(std::string fileName);
    bool Save(std::string fileName);
    void Default();

  public:
    int mModulesBackgroundColor;
    int mModulesAxisColor;
    int mModulesSelectedColor;
    int mModulesNotSelectedColor;
    int mModulesGridXSpacing;
    int mModulesGridYSpacing;
    int mModulesGridColor;
    int mModulesSelectionEdge;

    int mFramesBackgroundColor;
    int mFramesAxisColor;
    int mFramesGridXSpacing;
    int mFramesGridYSpacing;
    int mFramesGridColor;
    int mFramesBoundRectColor;

    int mAnimsBackgroundColor;
    int mAnimsAxisColor;
    int mAnimsSelectColor;
    int mAnimsGridXSpacing;
    int mAnimsGridYSpacing;
    int mAnimsGridColor;

    int mFrameLogicBackgroundColor;
    int mFrameLogicAxisColor;
    int mFrameLogicSelectColor;
    int mFrameLogicGridXSpacing;
    int mFrameLogicGridYSpacing;
    int mFrameLogicGridColor;
    int mFrameLogicGridRectColor;
    int mFrameLogicGridPointColor;
    int mFrameLogicSelectionEdge;

    int mColorMapBackgroundColor;
    int mColorMapAxisColor;
    int mColorMapGridXSpacing;
    int mColorMapGridYSpacing;
    int mColorMapGridColor;

    int mQuickModuleSize;
    int mQuickModulesBackgroundColor;
    int mQuickModulesIdTextColor;

    int mQuickFrameSize;
    int mQuickFramesBackgroundColor;
    int mQuickFramesIDTextColor;

    int mFileHistoryIndex;
    std::vector<std::string> mFileHistory;

    std::string mPathScripts;

    void AddFileToHistory(std::string fileName);

  protected:
    bool GetStrProperty(std::string category, std::string name, std::string& value);
    //bool SetStrProperty(std::string category, std::string name, std::string value);

    bool GetIntProperty(std::string category, std::string name, int& value);
    //bool SetIntProperty(std::string category, std::string name, int value);

    bool GetCategoryPropertyCount(std::string category, int& count);

  private:
    std::string IntToStr(int x);

    TiXmlDocument mDoc;
    TAppConfigState mState;
};

#endif
