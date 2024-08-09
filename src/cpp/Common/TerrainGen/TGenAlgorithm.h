#ifndef TGENALGORITHMH
#define TGENALGORITHMH

#include "HeightMap.h"

class TGenAlgorithm
{
  public: // c-d
    TGenAlgorithm();    
  
  public: // param interface
  
  public: // get-set
    void SetWidth(int _width);
    void SetHeight(int _height);
    int  GetWidth() { return mWidth; }
    int  GetHeight() { return mHeight; }
    
    void SetInputMap(HeightMap* _map);
    HeightMap* GetInputMap() { return mInputMap; }
    
  public: // ops
    virtual HeightMap* Generate() = 0;
  
  protected:
  
  private:
    int mWidth, mHeight;
    HeightMap* mInputMap;
};

#endif // TGENALGORITHMH
