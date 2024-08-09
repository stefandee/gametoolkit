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
 application: PP Mission Editor

 descrition : Class Sprite List
              lista de sprite
 last modify: 17 04 00
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_SpriteListH
#define PP_SpriteListH

#include "PP_Sprite.h"
#include "PP_String.h"

#define MAX_SPRITES 500

class CPGISpriteList
{
   public:
      void WriteFiles();

      CPGISpriteList();
      ~CPGISpriteList();

      int   Add(CPString filename, int wcells, int hcells);
      int   AddCustom(int _width, int _height, int wcells, int hcells);
      void  Remove(int index);
      void  Remove(CPGISprite* _sprite);
      void  RemoveAll();
      void  ReleaseAll();  //face release pe toate suprafetele fara sa sterga sprite-urile
      void  RestoreAll();  //inversul lui releaseAll
      void  Paint(int spriteindex, int x, int y, int zoomlevel,RECT rcClip);
      bool  IsEmpty();
      int   GetSpriteCount();
      CPGISprite*
            GetSprite(int index);   //inline

      int noOfSprites;
      
   private:
      int Links[MAX_SPRITES];          //numarul de obiecte cu un sprite
                                       //traiasca linux :)
      CPGISprite *Sprites[MAX_SPRITES];   //lista sprite-urilor
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//INLINE SECTION
/////////////////////////////////////////////////////////////////////////////
inline CPGISprite* CPGISpriteList::GetSprite(int index)
{
   if (index < 0)
      return NULL;
   return Sprites[index];
}


#endif
