//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Piron Component Interface - Panel
              this is a class and can have instances
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CPanel.h"
#include "PP_Graphic.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: contructor
---------------------------------------------------------------------------*/
CPIPanel::CPIPanel(CPIWinControl *Parent) : CPIWinControl(Parent)
{
}
/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIPanel::~CPIPanel()
{
}

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIPanel::IsDerivedFrom(CPString className)
{
    return ((className == ClassName()) || (CPIWinControl::IsDerivedFrom(className)));
}


/*---------------------------------------------------------------------------
 description: paint implementation
---------------------------------------------------------------------------*/
void CPIPanel::Paint()
{
    if (!GetVisible())
    {
        return;
    }

    VLOG(9) << "CPIPanel::Paint";
    SetClipRegion();

    if (GetSprite() != NULL)
    {
        CPGIGraphicSystem *lGSI;   //local Graphic System Instance
        try
        {
            lGSI = GetGraphicInstance();
        }
        catch(...)
        {
            LOG(FATAL) << "CPIPanel::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
            throw;
        }

        lGSI->PaintSprite(GetAbsoluteLeft(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), GetAnimType(), GetAlpha());
    }
    CPIWinControl::Paint();

    VLOG(9) << "CPIPanel::Paint - over";
}
//---------------------------------------------------------------------------

void CPIPanel::SetVisible(bool value)
{
    CPIWinControl::SetVisible(value);

    bool lVisible = GetVisible();

    VLOG(9) << lVisible;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

