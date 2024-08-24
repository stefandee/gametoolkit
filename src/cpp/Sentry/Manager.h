//---------------------------------------------------------------------------
#ifndef ManagerH
#define ManagerH
//---------------------------------------------------------------------------

#include <vector>

template <class T> class CManager
{
  public:
    int New();
    int Insert(int index, T item);
    int Clone(int index);
    int Delete(int index);
    void Clear();

    int  Cut(int index);
    void Copy(int index);
    int  Paste();

    void Undo();
    void Redo();

    int MoveUp(int index);
    int MoveDown(int index);

    T Get(int index);
    bool Set(int index, T value);

    int GetItemIndexById(int id);
    T   GetItemById(int id);

    int Size();

    int GenerateUniqueId();

  protected:

  private:
    std::vector<T> mList;
};
//---------------------------------------------------------------------------

template <class T>T CManager<T>::Get(int index)
{
  return mList[index];
}
//---------------------------------------------------------------------------

template <class T>bool CManager<T>::Set(int index, T value)
{
  if (index < 0 || index >= (int)mList.size())
  {
    return false;
  }

  mList[index] = value;
  
  return true;
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::Size()
{
  return mList.size();
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::New()
{
   T item;

   item.SetId(GenerateUniqueId());

   mList.push_back(item);

   return mList.size() - 1;
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::Insert(int index, T item)
{
  if (index < 0 || index >= Size())
  {
    return -1;
  }

  mList.insert(mList.begin() + index, item);

  return index;
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::Clone(int index)
{
  if (index < 0 || index >= Size())
  {
    return -1;
  }

  mList.insert(mList.begin() + index, mList[index]);

  mList[index + 1].SetId(GenerateUniqueId());

  return index + 1;
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::Delete(int index)
{
  auto it = mList.begin() + index;

  mList.erase(it);

  if (Size() == 0)
  {
    return -1;
  }

  if (index >= Size())
  {
    return Size() - 1;
  }

  return index;
}
//---------------------------------------------------------------------------

template <class T>void CManager<T>::Clear()
{
  mList.clear();
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::MoveUp(int index)
{
  if (index < 0 || index >= Size())
  {
    return -1;
  }

  if (index == 0)
  {
    return 0;
  }

  // swap
  T temp;
  
  temp             = mList[index];
  mList[index]     = mList[index - 1];
  mList[index - 1] = temp;

  return index - 1;
}
//---------------------------------------------------------------------------

template <class T>int CManager<T>::MoveDown(int index)
{
  if (index < 0 || index >= Size())
  {
    return -1;
  }

  if (index == Size() - 1)
  {
    return Size() - 1;
  }

  // swap
  T temp;

  temp             = mList[index];
  mList[index]     = mList[index + 1];
  mList[index + 1] = temp;

  return index + 1;
}
//---------------------------------------------------------------------------

// assumes that T has a GetId() method
template <class T>int CManager<T>::GenerateUniqueId()
{
  if (Size() == 0)
  {
    return 0x1000;
  }

  // find the minimum id
  int minId = mList[0].GetId();

  for(int i = 1; i < (int)mList.size(); i++)
  {
    if (minId > mList[i].GetId())
    {
      minId = mList[i].GetId();
    }
  }

  // start from the minimum id, increment and find an id that doesnt match
  int uniqueId = minId;
  bool found = false;

  while(!found)
  {
    found = true;

    for(int i = 0; i < (int)mList.size(); i++)
    {
      if (uniqueId == mList[i].GetId())
      {
        found = false;
        uniqueId++;
        break;
      }
    }
  }

  return uniqueId;
}
//---------------------------------------------------------------------------

template <class T>T CManager<T>::GetItemById(int id)
{
  for(int i = 0; i < Size(); i++)
  {
    if (id == mList[i].GetId())
    {
      return mList[i];
    }
  }

  T tmp;

  return tmp;
}

template <class T>int CManager<T>::GetItemIndexById(int id)
{
  for(int i = 0; i < Size(); i++)
  {
    if (id == mList[i].GetId())
    {
      return i;
    }
  }

  return -1;
}

#endif
