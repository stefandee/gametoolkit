// a script interface, for generating the terrain through a script

#ifndef TGENSCRIPTALGH
#define TGENSCRIPTALGH

#include <vector>
#include "TGenAlgorithm.h"
#include "ZCsl.hpp"

// constants for uniform accesing of get/set script functions
const int TGEN_MAP_OUT = 0;
const int TGEN_MAP_IN  = 1;

class TGenScriptAlg : public TGenAlgorithm
{
  public:
    TGenScriptAlg();
    virtual HeightMap* Generate();
    virtual ~TGenScriptAlg();

  public:
    bool SetScriptFromFile(const char* _fileName);
    void SetScript(ZString _script);
    ZString GetScript() { return mScript; }

  private: // script handlers & script methods
    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
    
    ZCsl_callback_t  mHandleGetWidth;
    ZCsl_callback_t  mHandleGetHeight;
    ZCsl_callback_t  mHandleSet;
    ZCsl_callback_t  mHandleGet;
    ZCsl_callback_t  mHandleHasInputMap;
    ZCsl_callback_t  mHandleCreateMap;
    ZCsl_callback_t  mHandleNormalize;
    ZCsl_callback_t  mHandleScale;
    ZCsl_callback_t  mHandleReset;
    ZCsl_callback_t  mHandleSmooth;

    ZString mcGetWidth(ZCsl* csl);
    ZString mcGetHeight(ZCsl* csl);
    ZString mcSet(ZCsl* csl);
    ZString mcGet(ZCsl* csl);
    ZString mcHasInputMap(ZCsl* csl);
    ZString mcCreateMap(ZCsl* csl);
    ZString mcNormalize(ZCsl* csl);
    ZString mcScale(ZCsl* csl);
    ZString mcReset(ZCsl* csl);
    ZString mcSmooth(ZCsl* csl);

  private:
    void InitCsl();
    void InitHandlers();

    int  CreateMap(int _width, int _height);
    void CleanMaps();

    HeightMap* GetMap(unsigned int _mapId);

    std::vector<HeightMap*> mMaps;
    ZCsl*                   mCompiler;
    ZString                 mModuleName;
    ZString                 mScript;
    HeightMap*              mOutputMap;  // this class do not manage the map it generates
                                         // it just creates it, leaving up to the
                                         // user to destroy it
};

#endif // TGENSCRIPTALGH
