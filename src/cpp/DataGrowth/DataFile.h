//---------------------------------------------------------------------------

#ifndef DataFileH
#define DataFileH
//---------------------------------------------------------------------------

#include <string>
#include <vector>

class CDataEntry
{
  public:
    CDataEntry();
    ~CDataEntry();

  public: // cc and assignment

  public:
    int  GetLocation() { return mLocation; }
    void SetLocation(int _v);

    void SetData(char* _data, int _datasize);
    char*  GetData() { return mData; }

    int GetDataSize() { return mDataSize; }

    void SetId(std::string _id) { mId = _id; }
    std::string GetId() { return mId; }

  private:
    int   mLocation;
    char* mData;
    int   mDataSize;
    std::string mId;
};

class CDataFile
{
  public:
    CDataFile();
    ~CDataFile();

  public:
    int  GetEntryCount();
    void AddEntry(CDataEntry* _v);
    CDataEntry* GetEntry(int _index);
    void DeleteEntry(CDataEntry* _v);
    void DeleteEntry(int _v);
    void DeleteEntriesInRange(int _v1, int _v2);
    void DeleteAllEntries();

	void WriteToFile(std::wstring _fileName);
    void ReadFromFile(std::wstring _fileName);

  private:
    std::vector<CDataEntry*> mEntries;
};

#endif
