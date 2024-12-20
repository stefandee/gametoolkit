//---------------------------------------------------------------------------

#ifndef XmlToCslH
#define XmlToCslH
//---------------------------------------------------------------------------

#include <vector>

#include "GenericCallback.h"
#include "ZCsl.hpp"
#include "StringManager.h"

class CXmlToCsl
{
  public:
    CXmlToCsl();
    ~CXmlToCsl();

  public:
	bool Run(std::wstring inputFile, std::wstring outputDir, std::wstring scriptFile);

	std::wstring ErrorDesc() { return mErrorDesc; }

  private:
    std::wstring mErrorDesc;

    StringManager mStringManager;

  private:
    ZCsl*                 mCompiler;
    ZString               mModuleName;
    std::string           mScript;
	std::wstring           mOutputDir;

	bool ReadScript(std::wstring scriptFile);
    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
	bool InitCsl(std::wstring scriptFile);
    void InitHandlers();
    void InitScriptingInterface();
    void InitScriptingLibs();

    // handles
    ZCsl_callback_t mHandleGetPackageCount;
    ZCsl_callback_t mHandleGetPackageName;
    ZCsl_callback_t mHandleGetPackageLang;
    ZCsl_callback_t mHandleGetPackageStringCount;
    ZCsl_callback_t mHandleGetPackageStringId;
    ZCsl_callback_t mHandleGetPackageStringValue;
    ZCsl_callback_t mHandleGetOutputDir;

    // the actual implementation
    ZString getPackageCount(ZCsl* csl);
    ZString getPackageName(ZCsl* csl);
    ZString getPackageLang(ZCsl* csl);
    ZString getPackageStringCount(ZCsl* csl);
    ZString getPackageStringId(ZCsl* csl);
    ZString getPackageStringValue(ZCsl* csl);
    ZString getOutputDir(ZCsl* csl);
};
//---------------------------------------------------------------------------

#endif
