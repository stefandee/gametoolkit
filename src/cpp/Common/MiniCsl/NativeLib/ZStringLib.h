//-----------------------------------------------------------------------------
//  Unit              :
//
//  Version           :
//
//  Description       :
//    *
//  Hystory           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef ZStringLibH
#define ZStringLibH
//---------------------------------------------------------------------------

#include "ZLibrary.h"
#include "GenericCallback.h"

class ZStringLib : public ZLibrary
{
  public:
   ZStringLib(                    // constructor
      ZCsl* _Parent,            // parent
      const ZString& _Name      // library name
      //ZLibrary* _Prev         // previous library
      );

   virtual void InitLibrary();

  private:
    ZString buildAscii(ZString val, char type, int size, int frac);
    ZString buildCsv(ZString val, char type, int size, int frac, char hyph);

    // callback handlers
    ZCsl_callback_t mHandleStrAscii;
    ZCsl_callback_t mHandleStrBuildRecord;
    ZCsl_callback_t mHandleStrCenter;
    ZCsl_callback_t mHandleStrChange;
    ZCsl_callback_t mHandleStrChar;
    ZCsl_callback_t mHandleStrExport;
    ZCsl_callback_t mHandleStrFormatNumber;
    ZCsl_callback_t mHandleStrImport;
    ZCsl_callback_t mHandleStrIndexOf;
    ZCsl_callback_t mHandleStrIsInteger;
    ZCsl_callback_t mHandleStrIsNumber;
    ZCsl_callback_t mHandleStrIsPrintable;
    ZCsl_callback_t mHandleStrLastIndexOf;
    ZCsl_callback_t mHandleStrLeftJustify;
    ZCsl_callback_t mHandleStrLength;
    ZCsl_callback_t mHandleStrLower;
    ZCsl_callback_t mHandleStrParseRecord;
    ZCsl_callback_t mHandleStrRemoveWords;
    ZCsl_callback_t mHandleStrRightJustify;
    ZCsl_callback_t mHandleStrSplitConnectString;
    ZCsl_callback_t mHandleStrSplitPath;
    ZCsl_callback_t mHandleStrSpread;
    ZCsl_callback_t mHandleStrStrip;
    ZCsl_callback_t mHandleStrStripExtension;
    ZCsl_callback_t mHandleStrStripLeading;
    ZCsl_callback_t mHandleStrStripTrailing;
    ZCsl_callback_t mHandleStrSubString;
    ZCsl_callback_t mHandleStrUpper;
    ZCsl_callback_t mHandleStrWordCount;
    ZCsl_callback_t mHandleStrWords;

    // the actual implementation
    ZString StrFormatNumber(ZCsl* csl);
    ZString StrCenter(ZCsl* csl);
    ZString StrChange(ZCsl* csl);
    ZString StrLeftJustify(ZCsl* csl);
    ZString StrRightJustify(ZCsl* csl);
    ZString StrUpper(ZCsl* csl);
    ZString StrLength(ZCsl* csl);
    ZString StrLower(ZCsl* csl);
    ZString StrStrip(ZCsl* csl);
    ZString StrStripTrailing(ZCsl* csl);
    ZString StrStripLeading(ZCsl* csl);
    ZString StrSpread(ZCsl* csl);
    ZString StrSubString(ZCsl* csl);
    ZString StrSplitPath(ZCsl* csl);
    ZString StrSplitConnectString(ZCsl* csl);
    ZString StrStripExtension(ZCsl* csl);
    ZString StrWords(ZCsl* csl);
    ZString StrWordCount(ZCsl* csl);
    ZString StrRemoveWords(ZCsl* csl);
    ZString StrIsNumber(ZCsl* csl);
    ZString StrIsInteger(ZCsl* csl);
    ZString StrIsPrintable(ZCsl* csl);
    ZString StrExport(ZCsl* csl);
    ZString StrImport(ZCsl* csl);
    ZString StrAscii(ZCsl* csl);
    ZString StrChar(ZCsl* csl);
    ZString StrIndexOf(ZCsl* csl);
    ZString StrLastIndexOf(ZCsl* csl);
    ZString StrBuildRecord(ZCsl* csl);
    ZString StrParseRecord(ZCsl* csl);
};

#endif
