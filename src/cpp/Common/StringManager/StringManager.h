//---------------------------------------------------------------------------

#ifndef StringManagerH
#define StringManagerH
//---------------------------------------------------------------------------

#include "tinyxml2.h"
#include <string>

class StringManager
{
  public:
    StringManager() : mErrors("") {}
    ~StringManager() {}

  public:
    bool LoadFile(std::string fileName);
	std::string GetErrors();

  public:
    int         GetPackageCount();
    std::string GetPackageName(int packageIndex);
    std::string GetPackageLang(int packageIndex);
    int         GetPackageStringCount(int packageIndex);
    std::string GetPackageStringId(int packageIndex, int stringIndex);
    std::string GetPackageStringValue(int packageIndex, int stringIndex);

  protected:
  
  private:
	tinyxml2::XMLDocument mDocument;
	std::string   mErrors;

	tinyxml2::XMLNode* GetPackage(int index);
};

#endif
