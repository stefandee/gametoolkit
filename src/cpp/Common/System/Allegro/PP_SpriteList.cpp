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
              bug: nu sterge din lista ca sa nu schimbe indecsii
              solutie: nu sterg decat sprite-ul si o initializez cu NULL
                       si la adaugare nu mai adaug in ultima pozitie, ci
                       in primul loc gasit liber
                       cat despre salvarea in fisier, momentan voi salva
                       si casutele goale, redundante, da'-i mai simplu;)
 last modify: 17 04 00
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_SpriteList.h"

#include "logfile.h"

//TEST FUNCTION
void CPGISpriteList::WriteFiles()
{
   for (int i=0; i<noOfSprites; i++)
   {
      if (Sprites[i] != NULL)
         logWriteLn(Sprites[i]->mFileName);
   }
}

/*---------------------------------------------------------------------------
 descrition: constructorul
             initializeaza lista
---------------------------------------------------------------------------*/
CPGISpriteList::CPGISpriteList()
{
   noOfSprites = 0;
   int i;
   for (i=0; i<MAX_SPRITES; i++)
   {
      Links[i]   = 0;
      Sprites[i] = NULL;
   }
}

/*---------------------------------------------------------------------------
 description: destructorul
---------------------------------------------------------------------------*/
CPGISpriteList::~CPGISpriteList()
{
   RemoveAll();
}

/*---------------------------------------------------------------------------
 descrition: adauga un bitmap in lista si returneaza pozitia in lista
 parameters: numele fisierului, numarul de cells
---------------------------------------------------------------------------*/
int CPGISpriteList::Add(CPString filename, int wcells, int hcells)
{
   int i;
   int firstnull = -1; //prima pozitie nula
   for (i=0; i<=noOfSprites; i++)
      if (Sprites[i] != NULL)
      {
         if (Sprites[i]->GetFileName() == filename)
         {
            Links[i]++;
            return i;
         }
      }
      else
      {
         if (firstnull == -1)
            firstnull = i;
      }
   //stiu sigur ca firstnull nu este -1 pentru ca m-am dus pana la noOfSprites
   Links[firstnull] = 1;
   try
   {
      Sprites[firstnull] = new CSprite(filename, wcells, hcells);
   }
   catch(...)
   {
      logWrite("CPGISpriteList::Add - UNABLE TO CREATE SPRITE from file ");
      logWriteLn(filename.c_str());
      Sprites[firstnull] = NULL;
   }
   if (noOfSprites == firstnull)
      noOfSprites++; //adica pe o pozitie pe care inca nu a mai fost nimic
   return firstnull;
}

int CPGISpriteList::AddCustom(int _width, int _height, int wcells, int hcells)
{
   int i;
   int firstnull = -1; //prima pozitie nula
   for (i=0; i<=noOfSprites; i++)
   {
      if (Sprites[i] != NULL)
      {
      }
      else
      {
         if (firstnull == -1)
            firstnull = i;
      }
   }

   //stiu sigur ca firstnull nu este -1 pentru ca m-am dus pana la noOfSprites
   Links[firstnull] = 1;
   try
   {
      Sprites[firstnull] = new CSpriteCustom(_width, _height, wcells, hcells);
   }
   catch(...)
   {
      logWrite("CPGISpriteList::AddCustom - UNABLE TO CREATE SPRITE.");
      Sprites[firstnull] = NULL;
   }
   if (noOfSprites == firstnull)
      noOfSprites++; //adica pe o pozitie pe care inca nu a mai fost nimic
   return firstnull;
}

/*--------------------------------------------------------------------------
 description: decrementeaza Links[index] si daca se ajunge la 0 atunci
              sterge sprite-ul cu numarul index din lista
              nu mai face replace cu ultimul ci i-l face NULL
 parameters : index
--------------------------------------------------------------------------*/
void CPGISpriteList::Remove(int index)
{
   if (!Sprites[index])
   {
     return;
   }

   Links[index] --;

   if (Links[index] == 0)
   {
     delete Sprites[index];
     Sprites[index] = NULL; //trebuie dezalocat
   }   
}

// Description : removes a sprite by pointer
// Params      : pointer to a sprite
// Result      : n/a
// Comments    : n/a
void CPGISpriteList::Remove(CPGISprite* _sprite)
{
  if (!_sprite)
  {
    return;
  }

  // search the index, call Remove(index)
  for(int i = 0; i < noOfSprites; i++)
  {
    if (_sprite == Sprites[i])
    {
      Remove(i);
      return;
    }
  }
}

/*--------------------------------------------------------------------------
 description: sterge intreaga lista indiferent de Links
--------------------------------------------------------------------------*/
void CPGISpriteList::RemoveAll()
{
   logWriteLn("CPGISpriteList::RemoveAll", LOGDET_LOWEST);
   logWriteILn(noOfSprites, LOGDET_LOWEST);
   int i;
   for (i=0; i<noOfSprites; i++)
   {
      if (Sprites[i] != NULL)
      {
         logWriteILn(i, LOGDET_LOWEST);
         delete Sprites[i];
         Sprites[i] = NULL;
         Links[i] = 0;
         logWriteILn(i, LOGDET_LOWEST);
      }
   }
   noOfSprites = 0;
   logWriteLn("CPGISpriteList::RemoveAll - over", LOGDET_LOWEST);
}

/*---------------------------------------------------------------------------
 description: face release pe toate suprafetele fara sa sterga sprite-urile
---------------------------------------------------------------------------*/
void CPGISpriteList::ReleaseAll()
{
   int i;
   for (i=0; i<noOfSprites; i++)
   {
      if (Sprites[i])
      {
        Sprites[i]->ReleaseSurface();
      }  
   }

   noOfSprites = 0;      
}

/*---------------------------------------------------------------------------
 description: reincarca suprafetele
              adica inversul lui releaseAll
---------------------------------------------------------------------------*/
void CPGISpriteList::RestoreAll()
{
   int i;
   for (i=0; i<noOfSprites; i++)
      if (Sprites[i])
         Sprites[i]->Restore();
}

/*--------------------------------------------------------------------------
 Description: o chestie care pune sprite-ul
 Parameters : la fel ca si putsprite din Sprite
 Others     : as fi putut sa o fac de fiecare data da' parca nu-i ase die faina
--------------------------------------------------------------------------*/
void CPGISpriteList::Paint(int spriteindex, int x, int y, int zoomlevel, const CPRect& rcClip)
{
//   if (spriteindex >= 0)
//      Sprites[spriteindex]->Paint(0, 0, x, y, zoomlevel, rcClip);
}

/*---------------------------------------------------------------------------
 descrition: returneaza true daca lista este vida
---------------------------------------------------------------------------*/
bool CPGISpriteList::IsEmpty()
{
   return (noOfSprites <= 0);
}

/*---------------------------------------------------------------------------
 descrition: returneaza numarul de obiecte din lista
---------------------------------------------------------------------------*/
int CPGISpriteList::GetSpriteCount()
{
   int i;
   int count = 0;
   for (i=0; i<noOfSprites; i++)
      if (Sprites[i])
         count++;
   return count;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
