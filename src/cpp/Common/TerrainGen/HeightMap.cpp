#include "HeightMap.h"
#include <string.h>
#include <iostream>
using namespace std;

HeightMap::HeightMap(int _width, int _height)
{
  mWidth = _width;
  mHeight = _height;
  AllocMap();
  
  Reset(0.);
}

HeightMap::HeightMap(const HeightMap& _what)
{
  mMap = 0;
  
  mWidth = _what.mWidth;
  mHeight = _what.mHeight;
  
  AllocMap();

  CopyMap(_what.mMap);  
}

HeightMap& HeightMap::operator = (const HeightMap& _what)
{  
  if (this != &_what)
  {
    //cout << "operator = " << endl; 
    
    // delete data
    DeleteMap();
    
    // assign w/h
    mWidth = _what.mWidth;
    mHeight = _what.mHeight;
    
    AllocMap();
    
    CopyMap(_what.mMap);
  }
  
  return *this;
}

void HeightMap::CopyMap(float** _map)
{
  
  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      mMap[y][x] = _map[y][x];
    }
  } 
  
  
  //memcpy(mMap, _map, mWidth * mHeight * sizeof(float));  
}

void HeightMap::AllocMap()
{
  mMap = new float*[mHeight];
  
  for(int i = 0; i < mHeight; i++)
  {
    mMap[i] = new float[mWidth];
  }
}

void HeightMap::DeleteMap()
{
  for(int i = 0; i < mWidth; i++)
  {
    delete [] mMap[i];
  }
  
  delete [] mMap;
  
  mMap = 0;
}

HeightMap::~HeightMap()
{
  DeleteMap();
}

bool HeightMap::XInRange(int _x)
{
  return (_x >= 0 && _x < mWidth);
}

bool HeightMap::YInRange(int _y)
{
  return (_y >= 0 && _y < mHeight);
}

float HeightMap::Get(int _x, int _y)
{
  if (XInRange(_x) && YInRange(_y))
  {
    return mMap[_y][_x];
  }  
    
  return 0.;
}

void  HeightMap::Set(int _x, int _y, float _v)
{
  if (XInRange(_x) && YInRange(_y))
  {
    mMap[_y][_x] = _v;
  }  
}

void  HeightMap::Normalize()
{
  float lMin = mMap[0][0], lMax = mMap[0][0];

  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      if (lMin > mMap[y][x])
      {
        lMin = mMap[y][x];
      }
      
      if (lMax < mMap[y][x])
      {
        lMax = mMap[y][x];
      }
    }
  }
  
  if (lMax - lMin == 0)
  {
    return;
  }
  
  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      mMap[y][x] = (mMap[y][x] - lMin) / (lMax - lMin);
    }
  }        
}

void  HeightMap::Scale(float _min, float _max)
{
  if (_max - _min == 0.)
  {
    return;
  }

  float lMin = mMap[0][0], lMax = mMap[0][0];

  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      if (lMin > mMap[y][x])
      {
        lMin = mMap[y][x];
      }
      
      if (lMax < mMap[y][x])
      {
        lMax = mMap[y][x];
      }
    }
  }
  
  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      mMap[y][x] = _min + (_max - _min) * (mMap[y][x] - lMin) / (lMax - lMin);
    }
  }  
}

void  HeightMap::Reset(float _v)
{
  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      mMap[y][x] = _v;
    }
  }  
}

void HeightMap::Print()
{
  cout << "printing matrix" << endl;

  for(int y = 0; y < mHeight; y++)
  {
    for(int x = 0; x < mWidth; x++)
    {
      cout << mMap[y][x] << " ";
    }
    
    cout << endl;
  }  
}

void HeightMap::Smooth(float _v)
{
  if (_v > 1.)
  {
    _v = 1.;
  }
  
  if (_v < 0.)
  {
    _v = 0.;
  }

  for(int y = 1; y < mHeight; y++)
  {
    for(int x = 1; x < mWidth; x++)
    {
      mMap[y][x] = mMap[y][x-1] * (1. - _v) + mMap[y][x] * _v;
    }
  }  
}



