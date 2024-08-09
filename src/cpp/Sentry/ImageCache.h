//---------------------------------------------------------------------------

#ifndef ImageCacheH
#define ImageCacheH
//---------------------------------------------------------------------------

#include <string>
#include <vector>

namespace Gdiplus
{
class Bitmap;
}

class ImageCacheEntry
{
  public:
    ImageCacheEntry() : mBitmap(0), mFileName(""), mCount(0) {}
    ~ImageCacheEntry();

  public:
    //Graphics::TBitmap* mBitmap;
    Gdiplus::Bitmap* mBitmap;

    // switched to DIBs
    //LPBITMAPINFO mBmpInfo;
    //void* mBits;

    std::string mFileName;
    int mCount;
};

class ImageCache
{
  public:
    static ImageCache* Instance();

  public:
    bool Add(std::string path);
    bool Remove(std::string fileName);
    void Clear();

    Gdiplus::Bitmap* GetBitmap(std::string fileName);
    Gdiplus::Bitmap* CloneBitmap(std::string fileName);

  protected:
    ImageCache();
    ~ImageCache();

  private:
    ImageCacheEntry* GetEntry(std::string fileName);

    static ImageCache* mInstance;

    std::vector<ImageCacheEntry*> mEntries;
};

#endif
