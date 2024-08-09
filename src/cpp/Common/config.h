#ifndef __PPT_CONFIGH__
#define __PPT_CONFIGH__

#include "Platform.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef PPT_PLATFORM_WIN
  #include <unistd.h>

  extern int filelength(int handle);
  extern int chsize(int handle, int off);
  extern int eof(int handle);
  extern long tell(int handle);
#endif

#endif
