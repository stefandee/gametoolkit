//---------------------------------------------------------------------------
//#include <strstream>
#include <fstream>
#pragma hdrstop

#include "CurveScript.h"
#include "ZMathLib.h"
#include "Serializable.h"
#include "easylogging++.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CurveScript::CurveScript(const CurveScript& _v)
{
  mModuleName = _v.mModuleName;
  mValidCsl   = false;
  SetScript(_v.mScript);

  //SetDefInterval(_v.GetDefInterval());
  SetDefInterval(const_cast<CurveScript&>(_v).GetDefInterval());
  SetParamList(const_cast<CurveScript&>(_v).GetParamList());
  SetName(const_cast<CurveScript&>(_v).GetName());
}
//---------------------------------------------------------------------------

CurveScript& CurveScript::operator= (const CurveScript& _v)
{
  if (&_v == this)
  {
    return (*this);
  }

  mModuleName = _v.mModuleName;
  mValidCsl   = false;
  SetScript(_v.mScript);

  SetDefInterval(const_cast<CurveScript&>(_v).GetDefInterval());
  SetParamList(const_cast<CurveScript&>(_v).GetParamList());
  SetName(const_cast<CurveScript&>(_v).GetName());

  return (*this);
}
//---------------------------------------------------------------------------

CurveScript::~CurveScript()
{
  delete mCompiler;
}
//---------------------------------------------------------------------------

void CurveScript::InitCsl()
{
  if (mValidCsl)
  {
    delete mCompiler;
  }

  mValidCsl = false;

  try
  {
    mCompiler = new ZCsl();
  }
  catch(...)
  {
    LOG(FATAL) << "CurveScript::InitCsl - errors while memory alloc for script compiler.";
    return;
  }

  try
  {
    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // add functions
    addModuleFunc("GetParam(const name)", mHandleGetParam);
  }
  catch(...)
  {
    LOG(FATAL) << "CurveScript::InitCsl - errors initializing libraries";
    delete mCompiler;
    return;
  }

  try
  {
    // The const_cast is a workaround for a Microsoft interface bug.
    //std::istrstream str(const_cast<char*>(mScript.c_str()));
    std::stringstream str;
    str << (const_cast<char*>(mScript.c_str()));

    // compile
    mCompiler->loadScript(mModuleName, &str);

    mValidCsl = true;
  }
  catch(const ZException& err)
  {
    LOG(FATAL) << "CurveScript::InitCsl - errors compiling script";
    for (int i = 0; i < err.count(); i++)
    {
      LOG(FATAL) << ((char*)err[i]);
    }

    delete mCompiler;
    mValidCsl = false;
    mCompiler = 0;
  }
}
//---------------------------------------------------------------------------

void CurveScript::InitHandlers()
{
  mHandleGetParam.set(this, &CurveScript:: mcGetParam);
}
//---------------------------------------------------------------------------

void CurveScript::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void CurveScript::SetScript(CPString _script)
{
  if (mValidCsl)
  {
    delete mCompiler;
  }

  mValidCsl       = false;
  mScript         = _script;

  InitHandlers();
  InitCsl();
}
//---------------------------------------------------------------------------

bool CurveScript::SetScriptFromFile(const char* _fileName)
{
  std::ifstream lFile(_fileName, std::ios_base::in);

  if (!lFile.is_open())
  {
    return false;
  }

  lFile.seekg(0);

  char lBuf[512];

  ZString lScript = "";

  while(lFile.getline(lBuf, 512))
  {
    lScript = lScript + ZString(lBuf) + "\n";
  }

  SetScript((char*)lScript);

  return true;
}
//---------------------------------------------------------------------------

ZString CurveScript::mcGetParam(ZCsl* csl)
{
  ZString lParamName = csl->get("name");

  double lParam = GetParamList().GetParam((char*)lParamName);

  return ZString(lParam);
}
//---------------------------------------------------------------------------

std::vector<double> CurveScript::Compute(TemplateParamList<double>& _paramList)
{
  std::vector<double> lComputed;

  if (!mValidCsl)
  {
    return lComputed;
  }

  TemplateParamList<double>& lParamList = GetParamList();

  if (lParamList.GetName() == CPString("Parametric"))
  {
    double lU = _paramList.GetParam("U");

    ZString lX, lY;

    // execute the script
    try
    {
      lX = mCompiler->call(mModuleName, "XParam", 1, (char*)(ZString(lU)));
    }
    catch(const ZException& /*err*/)
    {
    }

    // execute the script
    try
    {
      lY = mCompiler->call(mModuleName, "YParam", 1, (char*)(ZString(lU)));
    }
    catch(const ZException& /*err*/)
    {
    }

    lComputed.push_back(lX.asDouble());
    lComputed.push_back(lY.asDouble());

    return lComputed;
  }

  if (lParamList.GetName() == CPString("Explicit"))
  {
    double lX = _paramList.GetParam("X");

    if (GetDefInterval().IsIn(lX))
    {
      ZString lY;

      // execute the script
      try
      {
        lY = mCompiler->call(mModuleName, "YFromX", 1, (char*)(ZString(lX)));
      }
      catch(const ZException& /*err*/)
      {
      }

      lComputed.push_back(lX);

      double lYD = lY.asDouble();

      lComputed.push_back(lY.asDouble());
      return lComputed;
    }
  }

  return lComputed;
}
//---------------------------------------------------------------------------

void CurveScript::Serialize(PP::Stream &a)
{
  Curve2D::Serialize(a);

  CSerString((char*)mModuleName).Serialize(a);
  CSerString(mScript).Serialize(a);
}
//---------------------------------------------------------------------------

void CurveScript::DeSerialize(PP::Stream &a)
{
  Curve2D::DeSerialize(a);

  CSerString lModuleName, lScript;

  lModuleName.DeSerialize(a);
  mModuleName = ZString(lModuleName.Get().c_str());

  lScript.DeSerialize(a);
  SetScript(lScript.Get());
}
//---------------------------------------------------------------------------

