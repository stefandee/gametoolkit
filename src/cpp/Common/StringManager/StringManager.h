//---------------------------------------------------------------------------

#ifndef StringManagerH
#define StringManagerH
//---------------------------------------------------------------------------

#include "tinyxml.h"
#include <string>

class StringManager
{
  public:
    StringManager() : mErrors(L"") {}
    ~StringManager() {}

  public:
    bool LoadFile(std::wstring fileName);
	std::wstring GetErrors();

  public:
    int         GetPackageCount();
    std::string GetPackageName(int packageIndex);
    std::string GetPackageLang(int packageIndex);
    int         GetPackageStringCount(int packageIndex);
    std::string GetPackageStringId(int packageIndex, int stringIndex);
    std::string GetPackageStringValue(int packageIndex, int stringIndex);

  protected:
  
  private:
    TiXmlDocument mDocument;
	std::wstring   mErrors;
    
    TiXmlNode* GetPackage(int index);
};

#endif
