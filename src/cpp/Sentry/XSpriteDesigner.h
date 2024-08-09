//---------------------------------------------------------------------------

#ifndef XSpriteDesignerH
#define XSpriteDesignerH
//---------------------------------------------------------------------------

#include <vector>
#include <string>

#include "ImageCache.h"
#include "Manager.h"

class SentryScript;

const int FMODULE_BITFLAG_FLIP_X = 1;
const int FMODULE_BITFLAG_FLIP_Y = 2;
const int FMODULE_BITFLAG_ROT90  = 4;
const int FMODULE_BITFLAG_ROT270 = 8;

const int ANIMFRAME_BITFLAG_FLIP_X = 1;
const int ANIMFRAME_BITFLAG_FLIP_Y = 2;
const int ANIMFRAME_BITFLAG_ROT90  = 4;
const int ANIMFRAME_BITFLAG_ROT270 = 8;

class CRect
{
  public:
    CRect() : mX(0), mY(0), mWidth(-1), mHeight(-1) {}
    CRect(int x, int y, int w, int h) : mX(x), mY(y), mWidth(w), mHeight(h) {}

  public:
    int mX, mY, mWidth, mHeight;
};

class CPoint
{
  public:
    CPoint(int x, int y) : mX(x), mY(y) {}
    CPoint() : mX(0), mY(0) {}

    int mX, mY;
};

/*
class CRGB
{
  public:
    int mR, mG, mB;
};
*/

class CPalette
{
  public:

  protected:

  private:
    //CRGB mColors[256];
};

class CColorMapItem
{
  public:
    CColorMapItem() : mId(0), mSrcColor(0x000000), mDstColor(0x000000) {}

    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    void SetSrcColor(int v) { mSrcColor = v; }
    int GetSrcColor() { return mSrcColor; }

    void SetDstColor(int v) { mDstColor = v; }
    int GetDstColor() { return mDstColor; }

  private:
    int         mId;
    
    // colors are in format 0xAARRGGBB
    int         mSrcColor;
    int         mDstColor;
};

class CColorMap
{
  public:
    CColorMap() : mId(0), mInfo("No info") {}

  public:
    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    //void SetImageId(int v) { mImageId = v; }
    //int GetImageId() { return mImageId; }

    void SetInfo(std::string v) { mInfo = v; }
    std::string GetInfo() { return mInfo; }

  public:
    CManager<CColorMapItem> mItems;

  private:
    int         mId;
    std::string mInfo;
    
    //int         mImageId;
};

class CImage
{
  public:
    CImage() : mId(0), mFileName(""), mTransparentColor(0xFF00FF), mInfo("No Info") {}
    ~CImage();

  public:
    std::string GetInfo() { return mInfo; }
    void SetInfo(std::string v) { mInfo = v; }

    void SetTransparentColor(int v);
    int GetTransparentColor() { return mTransparentColor; }

    int GetId() { return mId; }
    void SetId(int id) { mId = id; }

    void SetFileName(std::string filename);
    std::string GetFileName() { return mFileName; }

    void SetUsed(bool v) { mUsed = v; }
    bool GetUsed() { return mUsed; }

  public:
    CManager<CColorMap> mColorMaps;

  protected:

  private:
    int         mId;
    std::string mFileName;
    std::string mInfo;
    int         mTransparentColor;
    bool        mUsed;
};

class CModule
{
  public:
    CModule() : mId(0), mImageId(-1), mX(0), mY(0), mWidth(-1), mHeight(-1), mInfo("No info") {}

  public:
    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    void SetImageId(int v) { mImageId = v; }
    int GetImageId() { return mImageId; }

    void SetX(int v) { mX = v; }
    int GetX() { return mX; }

    void SetY(int v) { mY = v; }
    int GetY() { return mY; }

    void SetWidth(int v) { mWidth = v; }
    int GetWidth() { return mWidth; }

    void SetHeight(int v) { mHeight = v; }
    int GetHeight() { return mHeight; }

    void SetInfo(std::string v) { mInfo = v; }
    std::string GetInfo() { return mInfo; }

    void SetUsed(bool v) { mUsed = v; }
    bool GetUsed() { return mUsed; }

  protected:

  private:
    int         mId;
    int         mImageId;
    int         mX, mY, mWidth, mHeight;
    std::string mInfo;
    bool        mUsed;
};

class CFrameModule
{
  public:
    CFrameModule() : mModuleId(-1), mPos(CPoint(0, 0)), mFlags(0) {}
    ~CFrameModule() {};

  public:
    CPoint GetPos() { return mPos; }
    void SetPos(CPoint _pos) { mPos = _pos; }

    int GetModuleId() { return mModuleId; }
    void SetModuleId(int _id) { mModuleId = _id; }

    int GetFlags() { return mFlags; }
    void SetFlags(int _id) { mFlags = _id; }

    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    void SetX(int x) { mPos.mX = x; }
    void SetY(int y) { mPos.mY = y; }

  protected:

  private:
    int    mModuleId;
    CPoint mPos;
    int    mFlags;

    // will not be used
    int    mId;
};

enum TFrameLogicItem { FRAME_LOGIC_ITEM_POINT, FRAME_LOGIC_ITEM_RECT };

class CFrameLogicItem
{
  public:
    CFrameLogicItem() : mX(0), mY(0), mWidth(-1), mHeight(-1), mType(FRAME_LOGIC_ITEM_POINT), mId(-1) { }

  public:
    int mX, mY, mWidth, mHeight;

  public:
    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    TFrameLogicItem GetType() { return mType; }
    void SetType(TFrameLogicItem v) { mType = v; }

    void SetX(int x) { mX = x; }
    void SetY(int y) { mY = y; }
    void SetWidth(int w) { mWidth = w; }
    void SetHeight(int h) { mHeight = h; }

    int GetX() { return mX; }
    int GetY() { return mY; }
    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }

  private:
    TFrameLogicItem mType;
    int mId;
};

class CFrameLogic
{
  public:
    CFrameLogic() {}
    ~CFrameLogic() {}

  public:
    CManager<CFrameLogicItem>   mItems;

    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    std::string GetInfo() { return mInfo; }
    void SetInfo(std::string v) { mInfo = v; }

  private:
    int         mId;
    std::string mInfo;
};

class CFrame
{
  public:
    CFrame() : mUsed(false), mId(-1), mInfo("") {}
    ~CFrame() {}

  public:
    CManager<CFrameModule> mFModules;
    CManager<CFrameLogic>  mFLogic;

  public:
    void SetUsed(bool v) { mUsed = v; }
    bool GetUsed() { return mUsed; }

    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    std::string GetInfo() { return mInfo; }
    void SetInfo(std::string v) { mInfo = v; }

    CRect GetBoundRect(CManager<CModule> modulesManager);

  protected:

  private:
    bool        mUsed;
    int         mId;
    std::string mInfo;
};

class CAnimFrame
{
  public:
    CAnimFrame() : mFrameId(-1), mFlags(0), mId(-1), mTime(1), mPos(CPoint(0, 0)) {}
    ~CAnimFrame() {}

  public:
    CPoint GetPos() { return mPos; }
    void SetPos(CPoint _pos) { mPos = _pos; }

    int GetFrameId() { return mFrameId; }
    void SetFrameId(int _id) { mFrameId = _id; }

    int GetFlags() { return mFlags; }
    void SetFlags(int _id) { mFlags = _id; }

    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    void SetX(int x) { mPos.mX = x; }
    void SetY(int y) { mPos.mY = y; }

    void SetTime(int v) { mTime = (v > 0) ? v : 0; }
    int  GetTime() { return mTime; }

  protected:

  private:
    int    mFrameId;
    CPoint mPos;
    int    mFlags;
    int    mTime;

    // will not be used, but needed by the CAnim Manager
    int    mId;
};

class CAnim
{
  public:
    CAnim() : mUsed(false), mId(-1), mInfo("") {}
    ~CAnim() {}

  public:
    void SetUsed(bool v) { mUsed = v; }
    bool GetUsed() { return mUsed; }

    void SetId(int id) { mId = id; }
    int GetId() { return mId; }

    std::string GetInfo() { return mInfo; }
    void SetInfo(std::string v) { mInfo = v; }

  public:
    CManager<CAnimFrame> mAFrames;

  protected:

  private:
    bool        mUsed;
    int         mId;
    std::string mInfo;
};

enum TSpriteDocState { SPR_DOC_STATE_NEW, SPR_DOC_STATE_SAVED, SPR_DOC_STATE_CLOSED };

// used for keeping sprite informations during the editing session
class CXSpriteDesigner
{
  public:
    CXSpriteDesigner() : mState(SPR_DOC_STATE_CLOSED), mFileName(""), mScript(0) {}
    virtual ~CXSpriteDesigner() { delete mScript; }

  public:
    void New();
    bool Save(std::string fileName);
    bool Load(std::string fileName);
    void Close();

  public:
    TSpriteDocState GetDocState() { return mState; }
    std::vector<std::string>& GetFileMessages() { return mFileMessages; }
    std::string GetFileName() { return mFileName; }

  // managers
  public:
    CManager<CImage>  mImagesManager;
    CManager<CModule> mModulesManager;
    CManager<CFrame>  mFramesManager;
    CManager<CAnim>   mAnimsManager;

  public:
    //Graphics::TBitmap* GetModuleBitmap(CModule module);
    Gdiplus::Bitmap* GetModuleBitmap(CModule module);

  // script delegation
  public:
    bool        LoadScriptFromFile(std::string fileName);
    std::string GetScriptVar(std::string name);
    void        SetScriptVar(std::string name, std::string value);
    std::string GetScript();
    
    std::string GetScriptError();
    std::string GetScriptParam(int index, int type);
    std::string SetScriptParam(int index, int type, const char* value);

    bool        InitScript();
    bool        ExecuteScript();

  protected:

  private:
    TSpriteDocState          mState;
    std::vector<std::string> mFileMessages;
    std::string              mFileName;

    SentryScript*            mScript;
};

#endif
