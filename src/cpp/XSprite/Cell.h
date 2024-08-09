//---------------------------------------------------------------------------

#ifndef CellH
#define CellH
//---------------------------------------------------------------------------

#include <string>

typedef struct
{
  short int left, right, top, bottom;
} TCellRect;

typedef struct
{
  short int x, y;
} TCellPoint;

class CCell
{
  public: // c-d
    CCell();
    ~CCell() {};

  public: // get-set
    TCellRect GetRect() { return mRect; }
    void SetRect(TCellRect _v) { mRect = _v; }

    TCellPoint GetPoint() { return mPoint; }
    void SetPoint(TCellPoint _v) { mPoint = _v; }

    void SetName(std::string _v) { mName = _v; }
    std::string GetName() { return mName; }

  protected:
  private:
    TCellRect  mRect;
    TCellPoint mPoint;
    std::string mName;  
};

#endif
