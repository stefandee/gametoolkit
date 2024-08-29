//---------------------------------------------------------------------------

#define STRICT
#include <windows.h>
#include <algorithm>
using std::min;
using std::max;
#include <gdiplus.h>
#pragma hdrstop

#include "ImageCache.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

ImageCache* ImageCache::mInstance = 0;

ImageCacheEntry::~ImageCacheEntry()
{
  delete mBitmap;
}
//---------------------------------------------------------------------------

ImageCache* ImageCache::Instance()
{
  if (!mInstance)
  {
    mInstance = new ImageCache();
  }

  return mInstance;
}
//---------------------------------------------------------------------------

ImageCache::ImageCache()
{
}
//---------------------------------------------------------------------------

ImageCache::~ImageCache()
{
}
//---------------------------------------------------------------------------

bool ImageCache::Add(std::string fileName)
{
  if (fileName.empty())
  {
    return false;
  }

  auto tmpFileName = UTF8ToUnicodeString(fileName.c_str());

  Gdiplus::Bitmap* tmpBmp = Gdiplus::Bitmap::FromFile(tmpFileName.c_str());

  if (!tmpBmp)
  {
    return false;
  }

  ImageCacheEntry* entry = new ImageCacheEntry();

  entry->mFileName = fileName;
  entry->mBitmap = tmpBmp;

  mEntries.push_back(entry);

  return true;
}
//---------------------------------------------------------------------------

bool ImageCache::Remove(std::string fileName)
{
  if (fileName.empty())
  {
    return false;
  }
  
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    if (mEntries[i]->mFileName == fileName)
    {
      mEntries[i]->mCount--;

      if (mEntries[i]->mCount == 0)
      {
        //delete mEntries[i].mBitmap;
        delete mEntries[i];

        std::vector<ImageCacheEntry*>::iterator it = mEntries.begin();

        it += i;

        mEntries.erase(it);

        return true;
      }

      break;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

void ImageCache::Clear()
{
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    //delete mEntries[i].mBitmap;
    delete mEntries[i];
  }

  mEntries.clear();
}
//---------------------------------------------------------------------------

ImageCacheEntry* ImageCache::GetEntry(std::string fileName)
{
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    if (mEntries[i]->mFileName == fileName)
    {
      return mEntries[i];
    }
  }

  return 0;
}
//---------------------------------------------------------------------------

Gdiplus::Bitmap* ImageCache::GetBitmap(std::string fileName)
{
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    if (mEntries[i]->mFileName == fileName)
    {
      return mEntries[i]->mBitmap;
    }
  }

  return 0;
}
//---------------------------------------------------------------------------

Gdiplus::Bitmap* ImageCache::CloneBitmap(std::string fileName)
{
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    if (mEntries[i]->mFileName == fileName)
    {
      Gdiplus::RectF rect(0.0f, 0.0f, mEntries[i]->mBitmap->GetWidth(), mEntries[i]->mBitmap->GetHeight());

      Gdiplus::Bitmap* tmpBmp = mEntries[i]->mBitmap->Clone(rect, mEntries[i]->mBitmap->GetPixelFormat());

      return tmpBmp;
    }
  }

  return 0;
}
//---------------------------------------------------------------------------

