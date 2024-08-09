//-----------------------------------------------------------------------------
//  Unit              : Scroll Bar basic (pp_cscrollbar.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * ar trebui sa existe o clasa de baza pentru cele doua scrollbaruri (h si v)
//    * din cauze de lene, nu exista inca, dar cind va exista timp, voi aduna interfetele
//    * momentan nu exista decit definitii de enumuri
//
//  Istorie           :
//    [11.03.2001] - [Karg] - unit creat
//-----------------------------------------------------------------------------

#ifndef PP_CScrollBarH
#define PP_CScrollBarH
//---------------------------------------------------------------------------

#include "PP_CControl.h"

//enum TPP_ScrollBarStyle  { SBS_HORIZONTAL, SBS_VERTICAL };
enum TPP_ScrollBarThumb  { SBT_FIXED, SBT_VARIABLE};
enum TPP_ScrollBarState  { SBST_NONE, SBST_LINEUP, SBST_LINEDOWN, SBST_PAGEUP, SBST_PAGEDOWN, SBST_TRACK };
enum TPP_ScrollBarCode   { SBC_LINEUP,   SBC_LINEDOWN,
                           SBC_PAGEUP, SBC_PAGEDOWN, SBC_POSITION,
                           SBC_TRACK,  SBC_TOP,      SBC_BOTTOM,
                           SBC_ENDSCROLL };

typedef std::function <void(CPIObject*,TPP_ScrollBarCode,int&)> ScrollBarCallback; /**< Scroll bar related events */

#endif
