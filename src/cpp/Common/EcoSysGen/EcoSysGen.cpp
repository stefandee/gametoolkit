//---------------------------------------------------------------------------


#pragma hdrstop

#include "EcoSysGen.h"
//---------------------------------------------------------------------------

EcoSysGen::~EcoSysGen()
{
  CleanItems();
}
//---------------------------------------------------------------------------

void EcoSysGen::SetItems(std::vector<EcoSysItemIn*> _items)
{
  CleanItems();

  mItems = _items;
}
//---------------------------------------------------------------------------

std::vector<EcoSysItemIn*> EcoSysGen::GetItems()
{
  std::vector<EcoSysItemIn*> lResult;

  for(unsigned int i = 0; i < mItems.size(); i++)
  {
    lResult.push_back(mItems[i]->Clone()); 
  }

  return lResult;
}
//---------------------------------------------------------------------------

void EcoSysGen::CleanItems()
{
  for(unsigned int i = 0; i < mItems.size(); i++)
  {
    delete mItems[i];
    mItems[i] = 0;
  }

  mItems.clear();
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
