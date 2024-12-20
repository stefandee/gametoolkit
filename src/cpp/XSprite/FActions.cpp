//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <Clipbrd.hpp>
#include <fstream>
#include <sstream>
#pragma hdrstop

#include <system.hpp>
#include "FActions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormActions *FormActions;
//---------------------------------------------------------------------------
__fastcall TFormActions::TFormActions(TComponent* Owner)
        : TForm(Owner)
{
  stringGrid->Cells[0][0] = "ACTION NAME";
  stringGrid->Cells[1][0] = "ID";
  stringGrid->Cells[2][0] = "UPDATE";
  stringGrid->Cells[3][0] = "START FRAME";
  stringGrid->Cells[4][0] = "END FRAME";
  stringGrid->Cells[5][0] = "X ALIGN";
  stringGrid->Cells[6][0] = "Y ALIGN";
  stringGrid->Cells[7][0] = "EFFECT";
  stringGrid->Cells[8][0] = "INACTIVE";
  stringGrid->Cells[9][0] = "REPEAT";
  stringGrid->Cells[10][0] = "X SPEEDS";
  stringGrid->Cells[11][0] = "Y SPEEDS";

  mActions.clear();
  mActions.push_back(CAction());
}
//---------------------------------------------------------------------------

void TFormActions::ScreenToList()
{
  for(unsigned int i = 0; i < mActions.size(); i++)
  {
    // eliminate spaces?
    stringGrid->Cells[0][i + 1].Trim();
    mActions[i].mName   = stringGrid->Cells[0][i + 1];

    mActions[i].mId     = stringGrid->Cells[1][i + 1].ToIntDef(0);
    mActions[i].mUpdate = stringGrid->Cells[2][i + 1].ToIntDef(0);
    mActions[i].SetStartFrame(stringGrid->Cells[3][i + 1].ToIntDef(0));
    mActions[i].SetEndFrame(stringGrid->Cells[4][i + 1].ToIntDef(0));

    // x align
    stringGrid->Cells[5][i + 1] = stringGrid->Cells[5][i + 1].UpperCase();

    if (stringGrid->Cells[5][i + 1] != "LEFT" && stringGrid->Cells[5][i + 1] != "RIGHT")
    {
      stringGrid->Cells[5][i + 1] = "LEFT";
    }

    mActions[i].mXAlign   = stringGrid->Cells[5][i + 1];

    // y align
    stringGrid->Cells[6][i + 1] = stringGrid->Cells[6][i + 1].UpperCase();

    if (stringGrid->Cells[6][i + 1] != "TOP" && stringGrid->Cells[6][i + 1] != "BOTTOM")
    {
      stringGrid->Cells[6][i + 1] = "TOP";
    }

    mActions[i].mYAlign   = stringGrid->Cells[6][i + 1];

    // effect
    stringGrid->Cells[7][i + 1] = stringGrid->Cells[7][i + 1].UpperCase();

    UnicodeString x = stringGrid->Cells[7][i + 1];

    if (stringGrid->Cells[7][i + 1] != "NORMAL" && stringGrid->Cells[7][i + 1] != "FLIPPED")
    {
      stringGrid->Cells[7][i + 1] = "NORMAL";
    }

    mActions[i].mEffect   = stringGrid->Cells[7][i + 1];

    // inactive
    stringGrid->Cells[8][i + 1] = stringGrid->Cells[8][i + 1].UpperCase();

    if (stringGrid->Cells[8][i + 1] != "YES" && stringGrid->Cells[8][i + 1] != "NO")
    {
      stringGrid->Cells[8][i + 1] = "NO";
    }

    mActions[i].mInactive   = stringGrid->Cells[8][i + 1];

    mActions[i].mRepeat = stringGrid->Cells[9][i + 1].ToIntDef(0);

    // check the speeds - first, convert the string to a speed vector
    StringToSpeeds(i, stringGrid->Cells[10][i + 1], stringGrid->Cells[11][i + 1]);

    stringGrid->Cells[10][i + 1] = BuildSpeedsList(i, true);
    stringGrid->Cells[11][i + 1] = BuildSpeedsList(i, false);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::Addaction1Click(TObject *Sender)
{
  stringGrid->RowCount++;
  mActions.push_back(CAction());
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::Add10actions1Click(TObject *Sender)
{
  for(int i = 0; i < 10; i++)
  {
    stringGrid->RowCount++;
    mActions.push_back(CAction());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::stringGridContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
  // blah?
  int Column = 0, Row = 0;

  TPoint ScreenPos = ClientToScreen(MousePos);

  stringGrid->MouseToCell(MousePos.x, MousePos.y, Column, Row);

  mLastPopupPoint = TPoint(Column, Row);

  Handled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::Removeaction1Click(TObject *Sender)
{
  // do something useful
  DeleteRow(mLastPopupPoint.y);
}
//---------------------------------------------------------------------------

void TFormActions::DeleteRow(int _pos)
{
  for(int i = _pos; i < stringGrid->RowCount - 1; i++)
  {
    stringGrid->Rows[i] = stringGrid->Rows[i + 1]; 
  }

  stringGrid->RowCount--;
}
//---------------------------------------------------------------------------

void TFormActions::DeleteColumn(int _pos)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::Save1Click(TObject *Sender)
{
  ScreenToList();

  // save the actions list to a file
  SaveActions(mFileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::FormShow(TObject *Sender)
{
  // load actions from file
  LoadActions(mFileName);
}
//---------------------------------------------------------------------------

void TFormActions::SetFileName(UnicodeString _fileName)
{
  mFileName = _fileName;
}
//---------------------------------------------------------------------------

void TFormActions::ListToScreen()
{
  stringGrid->RowCount = mActions.size() + 1;

  for(unsigned int i = 0; i < mActions.size(); i++)
  {
    stringGrid->Cells[0][i + 1] = mActions[i].mName; //"ACTION NAME";
    stringGrid->Cells[1][i + 1] = UnicodeString(mActions[i].mId); //"ID";
    stringGrid->Cells[2][i + 1] = UnicodeString(mActions[i].mUpdate); //"UPDATE";
    stringGrid->Cells[3][i + 1] = UnicodeString(mActions[i].GetStartFrame()); //"START FRAME";
    stringGrid->Cells[4][i + 1] = UnicodeString(mActions[i].GetEndFrame()); //"END FRAME";
    stringGrid->Cells[5][i + 1] = mActions[i].mXAlign; //"X ALIGN";
    stringGrid->Cells[6][i + 1] = mActions[i].mYAlign; //"Y ALIGN";
    stringGrid->Cells[7][i + 1] = mActions[i].mEffect; //"EFFECT";
    stringGrid->Cells[8][i + 1] = mActions[i].mInactive; //"INACTIVE";
    stringGrid->Cells[9][i + 1] = UnicodeString(mActions[i].mRepeat); //"REPEAT";
    stringGrid->Cells[10][i + 1] = BuildSpeedsList(i, true); //"SPEEDS";
    stringGrid->Cells[11][i + 1] = BuildSpeedsList(i, false); //"SPEEDS";
  }
}
//---------------------------------------------------------------------------

UnicodeString TFormActions::BuildSpeedsList(int _action, bool _what)
{
  UnicodeString lResult = "";

  for(unsigned int i = 0; i < mActions[_action].mSpeeds.size(); i++)
  {
    UnicodeString lToAdd;

    if (_what)
    {
      lToAdd = UnicodeString(mActions[_action].mSpeeds[i].GetX()) + ",";
    }
    else
    {
      lToAdd = UnicodeString(mActions[_action].mSpeeds[i].GetY()) + ",";
    }

    lResult = lResult + lToAdd;
  }

  lResult.SetLength(lResult.Length() - 1);

  return lResult;
}
//---------------------------------------------------------------------------

void TFormActions::StringToSpeeds(int _action, UnicodeString _xString, UnicodeString _yString)
{
  // build a vector by selecting values in the string
  int lPosX = _xString.Pos(L",");
  int lPosY = _yString.Pos(L",");

  mActions[_action].mSpeeds.clear();

  while(lPosX != 0 && lPosY != 0)
  {
    // get the possible integer
    UnicodeString lValueX = _xString.SubString(1, lPosX - 1);
    UnicodeString lValueY = _yString.SubString(1, lPosY - 1);

    CSpeed lSpeed = CSpeed(lValueX.ToIntDef(0), lValueY.ToIntDef(0));

    mActions[_action].mSpeeds.push_back(lSpeed);

    // cut the strings
    _xString = _xString.Delete(1, lPosX);
    _yString = _yString.Delete(1, lPosY);

    lPosX = _xString.Pos(L",");
    lPosY = _yString.Pos(L",");
  }

  mActions[_action].mSpeeds.push_back(CSpeed(_xString.ToIntDef(0), _yString.ToIntDef(0)));

  mActions[_action].mSpeeds.resize(abs(mActions[_action].GetStartFrame() - mActions[_action].GetEndFrame()) + 1);
}
//---------------------------------------------------------------------------

void TFormActions::LoadActions(UnicodeString _fileName)
{
  mActions.clear();

  ifstream infile(_fileName.c_str());

  if (!infile.is_open())
  {
      ListToScreen();
      return;
  }

  std::string line;

  while (std::getline(infile, line))
  {
    std::string lName, lXAlign, lYAlign, lInactive, lEffect;
    int  lStartFrame, lEndFrame;
    CAction lA;

    std::istringstream iss(line);

    getline(iss, lName, '\t');

    iss
      >> lA.mId
      >> lA.mUpdate
      >> lStartFrame
      >> lEndFrame
      >> lXAlign
      >> lYAlign
      >> lEffect
      >> lInactive
      >> lA.mRepeat;

    lA.mName = UTF8Decode(lName.c_str());
    lA.mXAlign = UTF8Decode(lXAlign.c_str());
    lA.mYAlign = UTF8Decode(lYAlign.c_str());
    lA.mEffect = UTF8Decode(lEffect.c_str());
    lA.mInactive = UTF8Decode(lInactive.c_str());
    lA.SetStartFrame(lStartFrame);
    lA.SetEndFrame(lEndFrame);

    // correct these values?
    if (lA.mXAlign != "LEFT" && lA.mXAlign != "RIGHT")
    {
      lA.mXAlign = "LEFT";
    }

    if (lA.mYAlign != "TOP" && lA.mYAlign != "BOTTOM")
    {
      lA.mYAlign = "TOP";
    }

    if (lA.mEffect != "NORMAL" && lA.mEffect != "FLIPPED")
    {
      lA.mEffect = "NORMAL";
    }

    if (lA.mInactive != "YES" && lA.mInactive != "NO")
    {
      lA.mInactive = "NO";
    }

    // read the speeds
    lA.mSpeeds.clear();

    if (!std::getline(infile, line))
    {
        break;
    }

    // read the speeds
    int lSpeedNo;

    std::istringstream iss2(line);
    iss2 >> lSpeedNo;

    for(int i = 0; i < lSpeedNo; i++)
    {
      if (!std::getline(infile, line))
      {
        break;
      }

      std::istringstream iss3(line);

      std::string strSpeedX, strSpeedY;

      getline(iss3, strSpeedX, '\t');
      getline(iss3, strSpeedY, '\t');

      lA.mSpeeds.push_back(CSpeed(UnicodeString(UTF8Decode(strSpeedX.c_str())).ToIntDef(0), UnicodeString(UTF8Decode(strSpeedY.c_str())).ToIntDef(0)));
    }

    mActions.push_back(lA);
  }

  /*
  while(!feof(lFile))
  {
    char lName[200], lXAlign[200], lYAlign[200], lInactive[200], lEffect[200];
    int  lStartFrame, lEndFrame;
    CAction lA;

    fscanf(lFile, "%s\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%d\t\n",
      lName,
      &lA.mId,
      &lA.mUpdate,
      &lStartFrame,
      &lEndFrame,
      lXAlign,
      lYAlign,
      lEffect,
      lInactive,
      &lA.mRepeat
    );

    lA.mName = UTF8Decode(lName);
    lA.mXAlign = UTF8Decode(lXAlign);
    lA.mYAlign = UTF8Decode(lYAlign);
    lA.mEffect = UTF8Decode(lEffect);
    lA.mInactive = UTF8Decode(lInactive);
    lA.SetStartFrame(lStartFrame);
    lA.SetEndFrame(lEndFrame);

    // correct these values?
    if (lA.mXAlign != "LEFT" && lA.mXAlign != "RIGHT")
    {
      lA.mXAlign = "LEFT";
    }

    if (lA.mYAlign != "TOP" && lA.mYAlign != "BOTTOM")
    {
      lA.mYAlign = "TOP";
    }

    if (lA.mEffect != "NORMAL" && lA.mEffect != "FLIPPED")
    {
      lA.mEffect = "NORMAL";
    }

    if (lA.mInactive != "YES" && lA.mInactive != "NO")
    {
      lA.mInactive = "NO";
    }

    // read the speeds
    int lSpeedNo;

    fscanf(lFile, "%d\n", &lSpeedNo);

    lA.mSpeeds.clear();

    for(int i = 0; i < lSpeedNo; i++)
    {
      int lX, lY;

      fscanf(lFile, "%d\t%d\n", &lX, &lY);

      lA.mSpeeds.push_back(CSpeed(lX, lY));
    }

    mActions.push_back(lA);
  }
  */

  ListToScreen();

  infile.close();

  //fclose(lFile);
}
//---------------------------------------------------------------------------

void TFormActions::SaveActions(UnicodeString _fileName)
{
    ofstream outfile(_fileName.c_str());

    if (!outfile.is_open()) {
        return;
    }

      for(unsigned int i = 0; i < mActions.size(); i++)
      {
        CAction lA = mActions[i];

        // fprintf(lFile, "%s\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%d\t\n",

        outfile <<
          UTF8Encode(lA.mName).c_str() <<
          "\t" <<
          lA.mId <<
          "\t" <<
          lA.mUpdate <<
          "\t" <<
          lA.GetStartFrame() <<
          "\t" <<
          lA.GetEndFrame() <<
          "\t" <<
          UTF8Encode(lA.mXAlign).c_str() <<
          "\t" <<
          UTF8Encode(lA.mYAlign).c_str() <<
          "\t" <<
          UTF8Encode(lA.mEffect).c_str() <<
          "\t" <<
          UTF8Encode(lA.mInactive).c_str() <<
          "\t" <<
          lA.mRepeat <<
          "\n";

        // write the speeds number
        auto speedsSize = lA.mSpeeds.size();

        outfile << speedsSize << "\n";

        // write the speeds vector
        for(unsigned int j = 0; j < speedsSize; j++)
        {
          auto speedX = lA.mSpeeds[j].GetX();
          auto speedY = lA.mSpeeds[j].GetY();

          outfile << speedX << "\t" << speedY << "\n";
        }
      }

    outfile.close();
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::Correct1Click(TObject *Sender)
{
  ScreenToList();
}
//---------------------------------------------------------------------------

// will transform the mActions into a binary form ready to be exported
char* TFormActions::GetData(UnicodeString _fileName, int* _size)
{
  // load the actions from the file
  LoadActions(_fileName);

  // build the speeds list and fill the action index field
  std::vector<CSpeed> lSpeedsToBeSaved;

  lSpeedsToBeSaved.clear();

  for(int i = 0; i < (int)mActions.size(); i++)
  {
    mActions[i].mSpeedIndex.mStart = lSpeedsToBeSaved.size();

    for(int j = 0; j < (int)mActions[i].mSpeeds.size(); j++)
    {
      lSpeedsToBeSaved.push_back(mActions[i].mSpeeds[j]);
    }

    mActions[i].mSpeedIndex.mEnd = lSpeedsToBeSaved.size() - 1;
  }

  // first, build the size

  // the initial value is the actions number
  int lSize = 1;

  for(unsigned int i = 0; i < mActions.size(); i++)
  {
    // data size
    lSize += 9;

    // two shorts for speed index
    lSize += 4;

    // number of speeds
    //lSize += 1;
    // speeds
    //lSize += mActions[i].mSpeeds.size() * 2;
  }

  // speeds count
  lSize += 2;

  // speeds array (x,y as byte)
  lSize += 2 * lSpeedsToBeSaved.size();

  // allocate the memory
  char* lData = new char[lSize];

  // and fill it
  int lIndex = 0;

  lData[lIndex++] = mActions.size();

  for(unsigned int i = 0; i < mActions.size(); i++)
  {
    //lData[lIndex++] = mActions[i].mId;

    lData[lIndex++] = (short)(mActions[i].mUpdate) % 256;
    lData[lIndex++] = (short)(mActions[i].mUpdate) / 256;

    lData[lIndex++] = mActions[i].GetStartFrame();
    lData[lIndex++] = mActions[i].GetEndFrame();

    if (mActions[i].mXAlign == "LEFT")
    {
      lData[lIndex++] = '\0';
    }

    if (mActions[i].mXAlign == "RIGHT")
    {
      lData[lIndex++] = '\1';
    }

    if (mActions[i].mYAlign == "TOP")
    {
      lData[lIndex++] = '\0';
    }

    if (mActions[i].mYAlign == "BOTTOM")
    {
      lData[lIndex++] = '\1';
    }

    if (mActions[i].mEffect == "NORMAL")
    {
      lData[lIndex++] = '\0';
    }

    if (mActions[i].mEffect == "FLIPPED")
    {
      lData[lIndex++] = '\1';
    }

    if (mActions[i].mInactive == "YES")
    {
      lData[lIndex++] = '\0';
    }

    if (mActions[i].mInactive == "NO")
    {
      lData[lIndex++] = '\1';
    }

    lData[lIndex++] = mActions[i].mRepeat;

    lData[lIndex++] = (short)(mActions[i].mSpeedIndex.mStart) % 256;
    lData[lIndex++] = (short)(mActions[i].mSpeedIndex.mStart) / 256;

    lData[lIndex++] = (short)(mActions[i].mSpeedIndex.mEnd) % 256;
    lData[lIndex++] = (short)(mActions[i].mSpeedIndex.mEnd) / 256;
    //lData[lIndex++] = mActions[i].mSpeeds.size();

    //for(unsigned int j = 0; j < mActions[i].mSpeeds.size(); j++)
    //{
    //  lData[lIndex++] = mActions[i].mSpeeds[j].GetX();
    //  lData[lIndex++] = mActions[i].mSpeeds[j].GetY();
    //}
  }

  // write the speeds array size
  lData[lIndex++] = (short)(lSpeedsToBeSaved.size()) % 256;
  lData[lIndex++] = (short)(lSpeedsToBeSaved.size()) / 256;

  // write the speeds array
  for(int i = 0; i < (int)lSpeedsToBeSaved.size(); i++)
  {
    lData[lIndex++] = (char)lSpeedsToBeSaved[i].GetX();
    lData[lIndex++] = (char)lSpeedsToBeSaved[i].GetY();
  }

  *_size = lSize;
  return lData;
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  ScreenToList();

  // save the actions list to a file
  SaveActions(mFileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::PastefromClipboard1Click(TObject *Sender)
{
  // check the clipboard
  if (!Clipboard()->HasFormat(CF_TEXT))
  {
    return;
  }

  // clipboard has some text in it, let's transform it
  UnicodeString lText = Clipboard()->AsText;

  UnicodeString lType = "final static byte";

  int lPos = lText.Pos(lType);

  mActions.clear();

  while(lPos != 0)
  {
    // cut the string of the type definition
    lText.Delete(lPos, lType.Length());

    int lDelimiterPos = lText.Pos(";");

    if (lDelimiterPos == 0)
    {
      break;
    }

    // extract the string from the start to ";" pos
    UnicodeString lNameValue = lText.SubString(1, lDelimiterPos - 1);

    lNameValue.Trim();

    // search the "="
    int lEqualPos = lNameValue.Pos("=");

    // create and add the action
    CAction lA;

    lA.mName = lNameValue.SubString(1, lEqualPos - 1).Trim();

    UnicodeString lValue = lNameValue.SubString(lEqualPos + 1, lText.Length() - lEqualPos - 1).Trim();
    lA.mId   = lValue.ToIntDef(0);

    mActions.push_back(lA);

    // delete this from text
    lText.Delete(1, lDelimiterPos);

    // restart the process ;)
    lPos = lText.Pos(lType);
  }

  ListToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TFormActions::Export1Click(TObject *Sender)
{
  // blabla
  if (dlgExport->Execute())
  {
    Export(dlgExport->FileName);
  }
}
//---------------------------------------------------------------------------

void TFormActions::Export(UnicodeString _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return;
  }

  // get the actions data
  int   lActionsDataSize;
  char* lActionsData = FormActions->GetData(mFileName, &lActionsDataSize);

  lFile.write(lActionsData, lActionsDataSize);

  // closing the shop
  lFile.close();
  delete [] lActionsData;
}
//---------------------------------------------------------------------------

