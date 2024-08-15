// Stream.cpp
#include "Stream.h"
#include "Platform.h"
#include "PP_Stdlib.h"

namespace PP {


Stream::Stream()
{
}

Stream::~Stream()
{
}

std::string Stream::FixPath(std::string _path)
{
  // FIXME: just a minor hack ;)
  ReplaceString(_path, "\\", std::string() + PPT_PATHSEPARATOR);
  ReplaceString(_path, "/", std::string() + PPT_PATHSEPARATOR);

  return _path;
}

}	// end of namespace PP
