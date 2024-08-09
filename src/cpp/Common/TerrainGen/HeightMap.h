#ifndef HeightMapH
#define HeightMapH

class HeightMap
{
  public:
    HeightMap(int _width, int _height);
    HeightMap(const HeightMap&);
    HeightMap& operator = (const HeightMap&);
    ~HeightMap();
    
  public:
    float Get(int _x, int _y);
    void  Set(int _x, int _y, float _v);
    void  Normalize();
    void  Scale(float _min, float _max);
    void  Reset(float _v);
    void  Smooth(float _v);
    void  Print();
    
  public:
    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }  
    
  protected:        
    HeightMap();
    
    bool XInRange(int _x);
    bool YInRange(int _y);
    
    void AllocMap();
    void DeleteMap();
    void CopyMap(float** _map);	    
    
  private:
    float** mMap;
    int     mWidth, mHeight;    
};

#endif // HeightMapH
