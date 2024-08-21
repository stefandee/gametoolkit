//---------------------------------------------------------------------------

#ifndef ActionH
#define ActionH
//---------------------------------------------------------------------------

#include <vector>
#include <string>


class CSpeedIndex
{
  public:
    CSpeedIndex() { mStart = mEnd = 0; }

  public:
    int mStart, mEnd;
};

class CSpeed
{
  public:
    CSpeed(int _x, int _y) { mX = _x; mY = _y; } 
    CSpeed() { mX = 0; mY = 0; } 

  public:
    int GetX() { return mX; }
    int GetY() { return mY; }

    void SetX(int _v) { mX = _v; }
    void SetY(int _v) { mY = _v; }

  private:
    int mX, mY;
};

class CAction
{
  public:
    CAction();
    ~CAction();

  public:
    void SetStartFrame(int _v);
    int  GetStartFrame() { return mStartFrame; }

    void SetEndFrame(int _v);
    int  GetEndFrame() { return mEndFrame; }

  protected:
    int  mStartFrame, mEndFrame;

    void RecomputeSpeeds();

  public:
    int  mId;
    int  mUpdate;
    //int  mXAlign, mYAlign;
    UnicodeString mXAlign, mYAlign, mInactive, mEffect;
    int  mRepeat;
    CSpeedIndex  mSpeedIndex;

    UnicodeString mName;
    std::vector<CSpeed> mSpeeds;
};

#endif
