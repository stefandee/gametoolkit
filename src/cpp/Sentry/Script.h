//---------------------------------------------------------------------------

#ifndef ScriptH
#define ScriptH
//---------------------------------------------------------------------------

#include "GenericCallback.h"
#include "ZCsl.hpp"

class CXSpriteDesigner;

const std::string SCRIPT_VAR_NAME = "ScriptName";
const std::string SCRIPT_VAR_INFO = "ScriptInfo";
const std::string SCRIPT_VAR_CATEGORY = "ScriptCategory";
const std::string SCRIPT_VAR_PARAM_COUNT = "ScriptParamCount";

class SentryScript
{
  public:
    SentryScript(CXSpriteDesigner* sprite);
    virtual ~SentryScript();

  public:
    bool LoadScriptFromFile(std::string scriptFile);

    void SetScriptVar(std::string name, std::string value);
    std::string GetScriptVar(std::string name);

    std::string GetParam(int index, int type);
    std::string SetParam(int index, int type, const char* value);
    std::string GetScript();

    std::string GetError();

    bool Init();
    bool Execute();

  protected:

  private:
    ZCsl*                 mCompiler;
    ZString               mModuleName;
    std::string           mScript;
    std::string           mErrorDesc;

    CXSpriteDesigner*     mSprite;

  private:
    SentryScript();

  private:
    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
    bool InitCsl();
    void InitHandlers();
    void InitScriptingInterface();
    void InitScriptingLibs();
    bool RunMethod(const char* method);

  private:
    //
    // handles
    //

    // misc
    ZCsl_callback_t mHandleGetSpritePath;
    ZCsl_callback_t mHandleExtractPath;
    ZCsl_callback_t mHandleExtractFileName;
    ZCsl_callback_t mHandleChangeFileExt;

    // images
    ZCsl_callback_t mHandleGetImageCount;
    ZCsl_callback_t mHandleGetImageFileName;
    ZCsl_callback_t mHandleGetImageId;
    ZCsl_callback_t mHandleGetImageInfo;
    ZCsl_callback_t mHandleGetImageTColor;
    ZCsl_callback_t mHandleGetImageIndexById;

    ZCsl_callback_t mHandleGetImagePixelFormat;
    ZCsl_callback_t mHandleGetImagePixel;
    ZCsl_callback_t mHandleSetImagePixel;

    // color maps
    ZCsl_callback_t mHandleGetColorMapsCount;
    ZCsl_callback_t mHandleGetColorMapId;
    ZCsl_callback_t mHandleGetColorMapItemsCount;
    ZCsl_callback_t mHandleGetColorMapItemId;
    ZCsl_callback_t mHandleGetColorMapItemSrcColor;
    ZCsl_callback_t mHandleGetColorMapItemDstColor;

    // modules
    ZCsl_callback_t mHandleGetModuleCount;
    ZCsl_callback_t mHandleGetModuleId;
    ZCsl_callback_t mHandleGetModuleImageId;
    ZCsl_callback_t mHandleGetModuleIndexById;
    ZCsl_callback_t mHandleGetModuleX;
    ZCsl_callback_t mHandleGetModuleY;
    ZCsl_callback_t mHandleGetModuleWidth;
    ZCsl_callback_t mHandleGetModuleHeight;
    ZCsl_callback_t mHandleGetModuleInfo;

    // frame modules
    ZCsl_callback_t mHandleGetFModuleModuleId;
    ZCsl_callback_t mHandleGetFModuleX;
    ZCsl_callback_t mHandleGetFModuleY;
    ZCsl_callback_t mHandleGetFModuleFlags;

    // frame logic items
    ZCsl_callback_t mHandleGetFLogicItemId;
    ZCsl_callback_t mHandleGetFLogicItemType;
    ZCsl_callback_t mHandleGetFLogicItemX;
    ZCsl_callback_t mHandleGetFLogicItemY;
    ZCsl_callback_t mHandleGetFLogicItemWidth;
    ZCsl_callback_t mHandleGetFLogicItemHeight;

    // frame logics
    ZCsl_callback_t mHandleGetFLogicItemCount;
    ZCsl_callback_t mHandleGetFLogicId;
    ZCsl_callback_t mHandleGetFLogicInfo;

    // frames
    ZCsl_callback_t mHandleGetFrameCount;
    ZCsl_callback_t mHandleGetFModuleCount;
    ZCsl_callback_t mHandleGetFLogicCount;
    ZCsl_callback_t mHandleGetFrameIndexById;
    ZCsl_callback_t mHandleGetFrameId;
    ZCsl_callback_t mHandleGetFrameInfo;
    ZCsl_callback_t mHandleGetFrameBoundX;
    ZCsl_callback_t mHandleGetFrameBoundY;
    ZCsl_callback_t mHandleGetFrameBoundWidth;
    ZCsl_callback_t mHandleGetFrameBoundHeight;

    // anim frames
    ZCsl_callback_t mHandleGetAFrameId;
    ZCsl_callback_t mHandleGetAFrameTime;
    ZCsl_callback_t mHandleGetAFrameFrameId;
    ZCsl_callback_t mHandleGetAFrameFlags;
    ZCsl_callback_t mHandleGetAFrameX;
    ZCsl_callback_t mHandleGetAFrameY;

    // anims
    ZCsl_callback_t mHandleGetAnimCount;
    ZCsl_callback_t mHandleGetAFrameCount;
    ZCsl_callback_t mHandleGetAnimId;
    ZCsl_callback_t mHandleGetAnimInfo;

    //
    // the actual implementation
    //

    // misc
    ZString getSpritePath(ZCsl* csl);
    ZString extractPath(ZCsl* csl);
    ZString extractFileName(ZCsl* csl);
    ZString changeFileExt(ZCsl* csl);

    // images
    ZString getImageCount(ZCsl* csl);
    ZString getImageFileName(ZCsl* csl);
    ZString getImageId(ZCsl* csl);
    ZString getImageInfo(ZCsl* csl);
    ZString getImageTColor(ZCsl* csl);
    ZString getImageIndexById(ZCsl* csl);

    ZString getImagePixelFormat(ZCsl* csl);
    ZString getImagePixel(ZCsl* csl);
    ZString setImagePixel(ZCsl* csl);

    // color maps
    ZString getColorMapsCount(ZCsl* csl);
    ZString getColorMapId(ZCsl* csl);
    ZString getColorMapItemsCount(ZCsl* csl);
    ZString getColorMapItemId(ZCsl* csl);
    ZString getColorMapItemSrcColor(ZCsl* csl);
    ZString getColorMapItemDstColor(ZCsl* csl);

    // modules
    ZString getModuleCount(ZCsl* csl);
    ZString getModuleId(ZCsl* csl);
    ZString getModuleImageId(ZCsl* csl);
    ZString getModuleIndexById(ZCsl* csl);
    ZString getModuleX(ZCsl* csl);
    ZString getModuleY(ZCsl* csl);
    ZString getModuleWidth(ZCsl* csl);
    ZString getModuleHeight(ZCsl* csl);
    ZString getModuleInfo(ZCsl* csl);

    // frame modules
    ZString getFModuleModuleId(ZCsl* csl);
    ZString getFModuleX(ZCsl* csl);
    ZString getFModuleY(ZCsl* csl);
    ZString getFModuleFlags(ZCsl* csl);

    // frame logic items
    ZString getFLogicItemId(ZCsl* csl);
    ZString getFLogicItemType(ZCsl* csl);
    ZString getFLogicItemX(ZCsl* csl);
    ZString getFLogicItemY(ZCsl* csl);
    ZString getFLogicItemWidth(ZCsl* csl);
    ZString getFLogicItemHeight(ZCsl* csl);

    // frame logics
    ZString getFLogicItemCount(ZCsl* csl);
    ZString getFLogicId(ZCsl* csl);
    ZString getFLogicInfo(ZCsl* csl);

    // frames
    ZString getFrameCount(ZCsl* csl);
    ZString getFModuleCount(ZCsl* csl);
    ZString getFLogicCount(ZCsl* csl);
    ZString getFrameIndexById(ZCsl* csl);
    ZString getFrameId(ZCsl* csl);
    ZString getFrameInfo(ZCsl* csl);
    ZString getFrameBoundX(ZCsl* csl);
    ZString getFrameBoundY(ZCsl* csl);
    ZString getFrameBoundWidth(ZCsl* csl);
    ZString getFrameBoundHeight(ZCsl* csl);

    // anim frames
    ZString getAFrameId(ZCsl* csl);
    ZString getAFrameTime(ZCsl* csl);
    ZString getAFrameFrameId(ZCsl* csl);
    ZString getAFrameFlags(ZCsl* csl);
    ZString getAFrameX(ZCsl* csl);
    ZString getAFrameY(ZCsl* csl);

    // anims
    ZString getAnimCount(ZCsl* csl);
    ZString getAFrameCount(ZCsl* csl);
    ZString getAnimId(ZCsl* csl);
    ZString getAnimInfo(ZCsl* csl);
};

#endif
