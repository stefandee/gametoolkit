// Stream.cpp
#include "Stream.h"
#include "Platform.h"

namespace PP {


Stream::Stream()
{
}

Stream::~Stream()
{
}

CPString Stream::FixPath(CPString _path)
{
  // FIXME: just a minor hack ;)
  _path.Replace('\\', PPT_PATHSEPARATOR);
  _path.Replace('/', PPT_PATHSEPARATOR);

  return CPString(_path);
}

}	// end of namespace PP
