//---------------------------------------------------------------------------


#pragma hdrstop

#include <fstream>
#include <ios>
#include "DataFile.h"

//---------------------------------------------------------------------------

CDataEntry::CDataEntry()
{
  mLocation = 0;
  mData     = 0;
  mDataSize = 0;
}
//---------------------------------------------------------------------------

CDataEntry::~CDataEntry()
{
  delete mData;
  
  mData = 0;
  mDataSize = 0;
  mLocation = 0;
}
//---------------------------------------------------------------------------

void CDataEntry::SetLocation(int _v)
{
  mLocation = _v;
}
//---------------------------------------------------------------------------

void CDataEntry::SetData(char* _data, int _datasize)
{
  if (mData)
  {
    delete mData;
  }

  mData = _data;

  mDataSize = _datasize;
}
//---------------------------------------------------------------------------

CDataFile::CDataFile()
{
  mEntries.clear();
}
//---------------------------------------------------------------------------

CDataFile::~CDataFile()
{
  DeleteAllEntries();  
}
//---------------------------------------------------------------------------

int CDataFile::GetEntryCount()
{
  return mEntries.size();
}
//---------------------------------------------------------------------------

void CDataFile::AddEntry(CDataEntry* _v)
{
  mEntries.push_back(_v);
}
//---------------------------------------------------------------------------

CDataEntry* CDataFile::GetEntry(int _index)
{
  if (_index >= (int)mEntries.size())
  {
    return 0;
  }

  return mEntries[_index];
}
//---------------------------------------------------------------------------

void CDataFile::DeleteEntry(CDataEntry* _v)
{
}
//---------------------------------------------------------------------------

void CDataFile::DeleteEntry(int _v)
{
  std::vector<CDataEntry*>::iterator lIt = mEntries.begin();

  lIt += _v;

  mEntries.erase(lIt);
}
//---------------------------------------------------------------------------

void CDataFile::DeleteEntriesInRange(int _v1, int _v2)
{
  std::vector<CDataEntry*>::iterator lItStart = mEntries.begin();
  lItStart += _v1;

  std::vector<CDataEntry*>::iterator lItEnd = mEntries.begin();
  lItEnd += _v2;

  mEntries.erase(lItStart, lItEnd);
}
//---------------------------------------------------------------------------

void CDataFile::DeleteAllEntries()
{
  mEntries.clear();
}
//---------------------------------------------------------------------------

void CDataFile::WriteToFile(std::wstring _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);//, 0666);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return;
  }

  // write the number of entries
  short lEntriesCount = mEntries.size();

  lFile.write((char*)&lEntriesCount, sizeof(short));

  // 4 bytes for the number of entries, 12 bytes for each file table entry
  int lLocation = lEntriesCount * 12 + 2;

  // write the header - the header contain pairs of (location, size) information
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    // write the id
    char lId[4];

    std::string lTemp = mEntries[i]->GetId();

    for(int j = 0; j < 4; j++)
    {
      lId[j] = lTemp[j];
    }

    lFile.write(lId, 4);

    // write the location
    lFile.write((char*)&lLocation, sizeof(int));

    // write the size
    int lSize = mEntries[i]->GetDataSize();

    lFile.write((char*)&lSize, sizeof(int));

    lLocation += lSize;
  }

  // write the data
  for(int i = 0; i < (int)mEntries.size(); i++)
  {
    lFile.write(mEntries[i]->GetData(), mEntries[i]->GetDataSize());
  }
  
  lFile.close();
}
//---------------------------------------------------------------------------

void CDataFile::ReadFromFile(std::wstring _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::in | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return;
  }

  // make clean before loading
  DeleteAllEntries();

  // read the number of entries
  short lEntriesCount;

  lFile.read((char*)&lEntriesCount, sizeof(short));

  for(int i = 0; i < lEntriesCount; i++)
  {
    int lLocation, lSize;
    char lId[5];
    CDataEntry* lDataEntry = new CDataEntry();

    lFile.read(lId, 4);

    lId[4] = '\0';
    lDataEntry->SetId(std::string(lId));

    lFile.read((char*)&lLocation, sizeof(int));
    lFile.read((char*)&lSize, sizeof(int));


    lDataEntry->SetLocation(lLocation);

    // save the current file position
    int lFilePos = lFile.tellp();

    // skip from file start to data and read it
    char* lData = new char[lSize];

    lFile.seekp(lLocation, std::ios_base::beg);

    lFile.read(lData, lSize);

    lDataEntry->SetData(lData, lSize);

    mEntries.push_back(lDataEntry);

    // seek back
    lFile.seekp(lFilePos, std::ios_base::beg);
  }

  lFile.close();
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
