//-----------------------------------------------------------------------------
//  Unit              : Linked Menu
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * un meniu cu butoanele interconectate de tevi
//    * exista un soi de bug (de fapt efectul "lenea mare") : pentru spacing mai
//      mare de 40, se nu se deseneaza frumos 
//
//  Istorie           :
//    [26.01.2001] - [Karg] - am creat unitul
//    [28.01.2001] - [Karg] - am adaugat t'urile si conectorii verticali
//-----------------------------------------------------------------------------
#ifndef PP_CSLinkMenuH
#define PP_CSLinkMenuH
//---------------------------------------------------------------------------

#include "PP_CSMenu.h"
#include "PP_CImage.h"

enum TPP_SLinkMenuOrientation { SLMO_UP, SLMO_DOWN };

class CPP_CSLinkMenu : public CPP_CSMenu
{
  private :

  protected:
    CPGISprite* mSprConn;
    TPP_SLinkMenuOrientation mLinkOrientation;

  public:

    CPP_CSLinkMenu(int _number = 1, CPIWinControl* Parent = NULL);

    virtual void Paint();

    // set-get
    void SetLinkOrientation(TPP_SLinkMenuOrientation _o) { mLinkOrientation = _o; };
    TPP_SLinkMenuOrientation GetLinkOrientation() { return mLinkOrientation; };

    ~CPP_CSLinkMenu();
};

#endif
