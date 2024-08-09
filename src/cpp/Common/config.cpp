#include "config.h"

#ifndef PPT_PLATFORM_WIN

int filelength(int handle)
{
  return 0;
}

int chsize(int handle, int off)
{
  return ftruncate(handle, off);
}

int eof(int handle)
{
  return lseek(handle, 0, SEEK_CUR);
}

long tell(int handle)
{
  return lseek(handle, 0, SEEK_CUR);
}

#endif
