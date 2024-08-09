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
//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#ifdef __WIN32__

#include "PP_CAvi.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CPP_CAvi::CPP_CAvi(CPIWinControl *Parent) : CPIControl(Parent)
{
  mStatus   = AS_STOP;
  mCenter   = false;
  mAutoSize = false;
  mStretch  = false;
  mPStretch = false;

  mTime       = 0;
  //mValidVideo = 0;
  //mValidAudio = 0;

  mPlayList = new CPStringList();
  mPlayList->Clear();

  mHandleOnEndPlayList = NULL;
  mHandleOnEndPlay     = NULL;

  AVIFileInit();
}
//---------------------------------------------------------------------------

bool CPP_CAvi::PrepareVideo()
{
  mVideoData.mValid = false;

  for(int i = 0; i < MAX_VIDEO_STREAMS; i++)
  {
    mVideoData.mVideoStreams[i] = NULL;
    mVideoData.mVideoFormats[i] = NULL;
    mVideoData.mVideoPGF[i]     = NULL;
    mVideoData.mVideoEndTime[i] = 0;
    //mVideoData.mValid           = false;
  }

  mVideoData.mNumVideoStreams = 0;

  do
  {

    if (AVIFileGetStream(mAviHandle, &mVideoData.mVideoStreams[mVideoData.mNumVideoStreams], streamtypeVIDEO, mVideoData.mNumVideoStreams))
       break;

  }
  while(++mVideoData.mNumVideoStreams < MAX_VIDEO_STREAMS);

  logWrite("Found streams : ");
  logWriteILn(mVideoData.mNumVideoStreams);

  if (mVideoData.mNumVideoStreams <= 0)
  {
    mVideoData.mValid = false;
    return false;
  }

  // determin formatele streamurilor
  for(int i = 0; i < mVideoData.mNumVideoStreams; i++)
  {
    PAVISTREAM pStream = mVideoData.mVideoStreams[i];

    LONG lSize;
    if(AVIStreamReadFormat(pStream, AVIStreamStart(pStream), NULL, &lSize))
    {
      return false;
    }

    LPBYTE pChunk = new BYTE[lSize];
    if(!pChunk)
    {
      return false;
    }

    if(AVIStreamReadFormat(pStream, AVIStreamStart(pStream), pChunk, &lSize))
    {
      return false;
    }

    mVideoData.mVideoFormats[i] = (LPBITMAPINFO)pChunk;
  }

  // deschid streamul video
  mVideoData.mVideoPGF[0] = AVIStreamGetFrameOpen(mVideoData.mVideoStreams[0], NULL);

  if(!mVideoData.mVideoPGF[0])
  {
    return false;
  }

  mVideoData.mVideoEndTime[0] = AVIStreamEndTime(mVideoData.mVideoStreams[0]);

  logWriteILn(mVideoData.mVideoFormats[0]->bmiHeader.biWidth);
  logWriteILn(mVideoData.mVideoFormats[0]->bmiHeader.biHeight);

  mVideoData.mValid  = true;
  mStatus = AS_STOP;

  // creez suprafata in functie de parametri
  CreateSurface();

  return true;
}
//---------------------------------------------------------------------------

bool CPP_CAvi::PrepareAudio()
{
  mAudioData.mValid = false;

  for(int i = 0; i < MAX_VIDEO_STREAMS; i++)
  {
    mAudioData.mAudioStreams[i] = NULL;
    mAudioData.mAudioFormats[i] = NULL;
    mAudioData.mAudioData[i]    = NULL;
    //mAudioData.mValid           = false;
  }

  // determin numarul de streamuri audio
  do
  {
    if(AVIFileGetStream(mAviHandle, &mAudioData.mAudioStreams[mAudioData.mNumAudioStreams], streamtypeAUDIO, mAudioData.mNumAudioStreams))
    {
      break;
    }
  }
  while(++mAudioData.mNumAudioStreams < MAX_AUDIO_STREAMS);

  logWrite("Found streams : ");
  logWriteILn(mAudioData.mNumAudioStreams);

  if (mAudioData.mNumAudioStreams <= 0)
  {
    return false;
  }

  // determin formatele streamurilor audio
  for(int i = 0; i < mAudioData.mNumAudioStreams; i++)
  {
    PAVISTREAM pStream = mAudioData.mAudioStreams[i];

    LONG lSize;
    if(AVIStreamReadFormat(pStream, AVIStreamStart(pStream), NULL, &lSize))
            return false;

    LPBYTE pChunk = new BYTE[lSize];
    if(!pChunk)
            return false;

    if(AVIStreamReadFormat(pStream, AVIStreamStart(pStream), pChunk, &lSize))
            return false;

    mAudioData.mAudioFormats[i] = (LPWAVEFORMAT)pChunk;
  }

  // aduc primul stream audio
  LONG lSize;
  if(AVIStreamRead(mAudioData.mAudioStreams[0], 0, AVISTREAMREAD_CONVENIENT, NULL, 0, &lSize, NULL) != 0)
  {
    return false;
  }

  LPBYTE lBuffer;

  try
  {
    lBuffer = new BYTE[lSize];
  }
  catch(...)
  {
    return false;
  }

  if(AVIStreamRead(mAudioData.mAudioStreams[0], 0, AVISTREAMREAD_CONVENIENT, lBuffer, lSize, NULL, NULL) != 0)
  {
    delete lBuffer;
    return false;
  }

  mAudioData.mAudioData[0] = lBuffer;

  mAudioData.mValid = true;

  return true;
}
//---------------------------------------------------------------------------

bool CPP_CAvi::ReleaseVideo()
{
  if (mVideoData.mValid)
  {
    if (AVIStreamGetFrameClose(mVideoData.mVideoPGF[0]))
    {
      //return;
    }

    for(int i = 0; i < mVideoData.mNumVideoStreams; i++)
    {
      if(mVideoData.mVideoStreams[i])
      {
        AVIStreamRelease(mVideoData.mVideoStreams[i]);
      }

      if(mVideoData.mVideoFormats[i])
      {
        delete [] ((LPBYTE)mVideoData.mVideoFormats[i]);
      }
    }

    GetGraphicInstance()->ReleaseSurface(mSurface);
  }

  return true;
}
//---------------------------------------------------------------------------

bool CPP_CAvi::ReleaseAudio()
{
  if (mAudioData.mValid)
  {
    for(int i = 0; i < mAudioData.mNumAudioStreams; i++)
    {
      if(mAudioData.mAudioStreams[i])
      {
        AVIStreamRelease(mAudioData.mAudioStreams[i]);
      }

      if(mAudioData.mAudioFormats[i])
      {
        delete [] ((LPBYTE)mAudioData.mAudioFormats[i]);
      }

      if(mAudioData.mAudioData[i])
      {
        //delete [] mAudioData.mAudioData[i];
      }
    }
  }

  return true;
}
//---------------------------------------------------------------------------

bool CPP_CAvi::InternalOpen(CPString _fileName)
{
  if(AVIFileOpen(&mAviHandle, _fileName.c_str(), OF_READ, NULL) != 0)
  {
    logWriteLn("Could not open avi file.");
    return false;
  }

  PrepareAudio();

  if (!PrepareVideo())
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

void CPP_CAvi::InternalClose()
{
  ReleaseVideo();
  ReleaseAudio();
  AVIFileRelease(mAviHandle);
}
//---------------------------------------------------------------------------

void CPP_CAvi::NextFile()
{
  if (mVideoData.mValid)
  {
    InternalClose();
  }  

  // caut urmatorul fisier din lista care se poate deschide
  for(int i = mPlayIndex; i < mPlayList->Count; i++)
  {
    if (InternalOpen(*mPlayList->GetString(i)))
    {
      mPlayIndex = i;
      break;
    }

    InternalClose();
  }

  // s'a ajuns la sfirsitul listei - trebuie generat un event
  OnEndPlayList();
}
//---------------------------------------------------------------------------

bool CPP_CAvi::Open(CPString     _fileName)
{
  // sterg playlistul vechi si adaug fisierul
  // va fi un playlist cu un singur fisier
  mPlayList->Clear();
  mPlayList->Add(new CPString(_fileName));
  mPlayIndex = 0;

  NextFile();

  return true;
}
//---------------------------------------------------------------------------

bool CPP_CAvi::Open(CPStringList* _list)
{
  if (_list == NULL)
  {
    return false;
  }

  if (_list->Count <= 0)
  {
    return false;
  }

  mPlayList->Clear();
  mPlayIndex = 0;

  for(int i = 0; i < _list->Count; i++)
  {
    mPlayList->Add(_list->GetString(i));
  }

  NextFile();

  return true;
}
//---------------------------------------------------------------------------

void CPP_CAvi::Play()
{
  if (mVideoData.mValid)
  {
    if (mStatus != AS_PAUSE)
    {
      mTime = 0;
    }

    mStatus = AS_PLAY;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Stop()
{
  if (mVideoData.mValid)
  {
    mStatus = AS_STOP;
    mTime   = 0;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Pause()
{
  if (mVideoData.mValid)
  {
    if (mStatus == AS_PAUSE)
    {
      Play();
      return;
    }

    mStatus = AS_PAUSE;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Update()
{
  CPIControl::Update();

  if (!mVideoData.mValid)
  {
    return;
  }

  switch(mStatus)
  {
    case AS_STOP :
      break;

    case AS_PLAY :
      mTime += 30;

      if (mTime > mVideoData.mVideoEndTime[0])
      {
        Stop();

        logWriteLn("Avi stopped, generating onendplay event"); 

        OnEndPlay();
      }

      break;

    case AS_PAUSE :
      break;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Paint()
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  if (!mVideoData.mValid)
  {
    return;
  }

  SetClipRegion();

  // afisez frame'ul curent
  PAVISTREAM pStream = mVideoData.mVideoStreams[0];
  PGETFRAME &pgf     = mVideoData.mVideoPGF[0];

  LONG lFrame;
  if(mTime <= mVideoData.mVideoEndTime[0])
  {
    lFrame = AVIStreamTimeToSample(pStream, mTime);
  }
  else
  {
    return;
  }

  LPBITMAPINFOHEADER pBmp = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, lFrame);

  if (!pBmp)
  {
    return;
  }

  // pictez suprafata pe background - ORIBIL !!!
  HDC hdc;
  mSurface->GetDC(&hdc);

  LPBYTE pBmpBits = ((LPBYTE)pBmp) + pBmp->biSize;

  LPBYTE pGDIBits;
  HBITMAP hBmp = CreateDIBSection(hdc, (LPBITMAPINFO)pBmp, DIB_RGB_COLORS, (LPVOID*)&pGDIBits, NULL, 0);

  memcpy(pGDIBits, pBmpBits, pBmp->biSizeImage);

  HDC hBmpDC = CreateCompatibleDC(hdc);

  HGDIOBJ pOldObj = SelectObject(hBmpDC, hBmp);

  if (mStretch)
  {
    StretchBlt(hdc, 0, 0, mSurfaceW, mSurfaceH, hBmpDC, 0, 0, pBmp->biWidth, pBmp->biHeight, SRCCOPY);
  }
  else
  {
    if (mPStretch)
    {
      StretchBlt(hdc, 0, 0, mSurfaceW, mSurfaceH, hBmpDC, 0, 0, pBmp->biWidth, pBmp->biHeight, SRCCOPY);
    }
    else
    {
      BitBlt(hdc, 0, 0, mSurfaceW, mSurfaceH, hBmpDC, 0, 0, SRCCOPY);
    }
  }

  if(pOldObj) SelectObject(hBmpDC, pOldObj);
  DeleteObject(hBmp);
  DeleteDC(hBmpDC);

  mSurface->ReleaseDC(hdc);

  TRect lDstRect;

  if (mCenter && !mAutoSize && !mStretch)
  {
    lDstRect = Rect(lL + GetWidth()  / 2 - mSurfaceW / 2,
                    lT + GetHeight() / 2 - mSurfaceH / 2,
                    lL + GetWidth()  / 2 + mSurfaceW / 2,
                    lT + GetHeight() / 2 + mSurfaceH / 2);
  }
  else
  {
    lDstRect = Rect(lL, lT, lL + mSurfaceW, lT + mSurfaceH);
  }

  GetGraphicInstance()->PaintSurfaceToSurface(
    NULL,
    lDstRect,
    mSurface,
    Rect(0, 0, mSurfaceW, mSurfaceH)
    );

  //GetGraphicInstance()->Rectangle(NULL, Rect(lL + 1, lT + 1, lL + GetWidth() - 1, lT + GetHeight() - 1), 0xFFFFFF); 
}
//---------------------------------------------------------------------------

void CPP_CAvi::Close()
{
  InternalClose();
  mPlayList->Clear();
}
//---------------------------------------------------------------------------

void CPP_CAvi::CreateSurface()
{
  if (!mVideoData.mValid)
  {
    return;
  }

  if (mSurface)
  {
    GetGraphicInstance()->ReleaseSurface(mSurface);
  }

  if (mStretch)
  {
    mSurface = GetGraphicInstance()->CreateSurface(GetWidth(), GetHeight());
    mSurfaceW = GetWidth();
    mSurfaceH = GetHeight();
    return;
  }

  if (mPStretch)
  {
    float lFactor = (float)mVideoData.mVideoFormats[0]->bmiHeader.biWidth/mVideoData.mVideoFormats[0]->bmiHeader.biHeight;

    if (lFactor >= 1.0)
    {
      mSurface  = GetGraphicInstance()->CreateSurface(GetWidth(), GetWidth() / lFactor);
      mSurfaceW = GetWidth();
      mSurfaceH = GetWidth() / lFactor;
    }
    else
    {
      mSurface  = GetGraphicInstance()->CreateSurface(GetHeight() * lFactor, GetHeight());
      mSurfaceW = GetHeight() * lFactor;
      mSurfaceH = GetHeight();
    }

    return;
  }

  mSurface  = GetGraphicInstance()->CreateSurface(mVideoData.mVideoFormats[0]->bmiHeader.biWidth, mVideoData.mVideoFormats[0]->bmiHeader.biHeight);
  mSurfaceW = mVideoData.mVideoFormats[0]->bmiHeader.biWidth;
  mSurfaceH = mVideoData.mVideoFormats[0]->bmiHeader.biHeight;

  logWriteLn("Dimensiunile :");
  logWriteFLn(mSurfaceW);
  logWriteFLn(mSurfaceH);
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetStretch(bool _s)
{
  mStretch = _s;
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetPStretch(bool _ps)
{
  mPStretch = _ps;
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetAutoSize(bool _a)
{
  if (!mVideoData.mValid)
  {
    return;
  }

  mAutoSize = _a;

  if (mAutoSize)
  {
    SetWidth(mVideoData.mVideoFormats[0]->bmiHeader.biWidth);
    SetHeight(mVideoData.mVideoFormats[0]->bmiHeader.biHeight);
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetWidth(int _width)
{
  CPIControl::SetWidth(_width);

  // autosize == true : chiar daca se seteaza width'ul, controlul trebuie sa fie dimensionat
  // la dimensiunile filmului
  SetAutoSize(GetAutoSize());
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetHeight(int _height)
{
  CPIControl::SetHeight(_height);

  // autosize == true : chiar daca se seteaza height'ul, controlul trebuie sa fie dimensionat
  // la dimensiunile filmului
  SetAutoSize(GetAutoSize());
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::Resize(int _width, int _height)
{
  CPIControl::SetWidth(_width);
  CPIControl::SetHeight(_height);
  SetAutoSize(GetAutoSize());
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetHandleOnEndPlay (void (__closure* value)(CPIObject *Sender))
{
  mHandleOnEndPlay = value;
}
//---------------------------------------------------------------------------

void CPP_CAvi::OnEndPlay()
{
  if (mHandleOnEndPlay != NULL)
  {
    mHandleOnEndPlay(this);
  }

  // urmatorul fisier din playlist
  mPlayIndex++;
  logWrite("next item in playlist");
  logWriteILn(mPlayIndex);
  NextFile();

  // daca s'a ajuns la sfirsitul listei, atunci apelul da in gol - mVideoData.mValid e false
  // din cauza apelului InternalClose (dar nu e nici o problema)
  logWriteLn("Playing next item");
  Play();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetHandleOnEndPlayList (void (__closure* value)(CPIObject *Sender))
{
  mHandleOnEndPlayList = value;
}
//---------------------------------------------------------------------------

void CPP_CAvi::OnEndPlayList()
{
   // repeat, etc

   if (mHandleOnEndPlayList != NULL)
   {
     mHandleOnEndPlay(this);
   }
}
//---------------------------------------------------------------------------

CPP_CAvi::~CPP_CAvi()
{
  Close();
  delete mPlayList;
  AVIFileExit();
}
//---------------------------------------------------------------------------

/*
CPP_CAvi::CPP_CAvi(CPIWinControl *Parent) : CPIControl(Parent)
{
  mValid    = false;
  mStatus   = AS_STOP;
  mCenter   = false;
  mAutoSize = false;
  mStretch  = false;
  mPStretch = false;
  AVIFileInit();

  //SetWidth(500);
  //SetHeight(400);
}
//---------------------------------------------------------------------------

bool CPP_CAvi::Open(CPString _fileName)
{
  if (mValid)
  {
    return false;
  }

  mValid = false;

  if(AVIFileOpen(&mAviHandle, _fileName.c_str(), OF_READ, NULL) != 0)
  {
    logWriteLn("Could not open avi file.");
    return false;
  }

  for(int i = 0; i < MAX_VIDEO_STREAMS; i++)
  {
    mVideoStreams[i] = NULL;
    mVideoFormats[i] = NULL;
    mVideoPGF[i]     = NULL;
    mVideoEndTime[i] = 0;
  }

  mNumVideoStreams = 0;

  do
  {

    if (AVIFileGetStream(mAviHandle, &mVideoStreams[mNumVideoStreams], streamtypeVIDEO, mNumVideoStreams))
       break;

  }
  while(++mNumVideoStreams < MAX_VIDEO_STREAMS);

  logWrite("Found streams : ");
  logWriteILn(mNumVideoStreams);

  if (mNumVideoStreams <= 0)
  {
    return false;
  }

  // determin formatele streamurilor
  for(int i = 0; i < mNumVideoStreams; i++)
  {
    PAVISTREAM pStream = mVideoStreams[i];

    LONG lSize;
    if(AVIStreamReadFormat(pStream, AVIStreamStart(pStream), NULL, &lSize))
    {
      return false;
    }

    LPBYTE pChunk = new BYTE[lSize];
    if(!pChunk)
    {
      return false;
    }

    if(AVIStreamReadFormat(pStream, AVIStreamStart(pStream), pChunk, &lSize))
    {
      return false;
    }

    mVideoFormats[i] = (LPBITMAPINFO)pChunk;
  }


  // deschid streamul
  mVideoPGF[0] = AVIStreamGetFrameOpen(mVideoStreams[0], NULL);

  if(!mVideoPGF[0])
  {
    return false;
  }

  mVideoEndTime[0] = AVIStreamEndTime(mVideoStreams[0]);

  logWriteILn(mVideoFormats[0]->bmiHeader.biWidth);
  logWriteILn(mVideoFormats[0]->bmiHeader.biHeight);

  mValid  = true;
  mStatus = AS_STOP;

  // creez suprafata in functie de parametri
  CreateSurface();

  return true;
}
//---------------------------------------------------------------------------

void CPP_CAvi::Play()
{
  if (mValid)
  {
    if (mStatus != AS_PAUSE)
    {
      mTime = 0;
    }

    mStatus = AS_PLAY;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Stop()
{
  if (mValid)
  {
    mStatus = AS_STOP;
    mTime   = 0;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Pause()
{
  if (mValid)
  {
    if (mStatus == AS_PAUSE)
    {
      Play();
      return;
    }
    
    mStatus = AS_PAUSE;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Update()
{
  CPIControl::Update();

  if (!mValid)
  {
    return;
  }

  switch(mStatus)
  {
    case AS_STOP :
      break;

    case AS_PLAY :
      mTime += 30;

      if (mTime > mVideoEndTime[0])
      {
        Stop();
        OnEndPlay();
      }

      break;

    case AS_PAUSE :
      break;
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::Paint()
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  if (!mValid)
  {
    return;
  }

  SetClipRegion();

  // afisez frame'ul curent
  PAVISTREAM pStream = mVideoStreams[0];
  PGETFRAME &pgf     = mVideoPGF[0];

  LONG lFrame;
  if(mTime <= mVideoEndTime[0])
  {
    lFrame = AVIStreamTimeToSample(pStream, mTime);
  }
  else
  {
    return;
  }

  LPBITMAPINFOHEADER pBmp = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, lFrame);

  if (!pBmp)
  {
    return;
  }

  // pictez suprafata pe background - ORIBIL !!!
  HDC hdc;
  mSurface->GetDC(&hdc);

  LPBYTE pBmpBits = ((LPBYTE)pBmp) + pBmp->biSize;

  LPBYTE pGDIBits;
  HBITMAP hBmp = CreateDIBSection(hdc, (LPBITMAPINFO)pBmp, DIB_RGB_COLORS, (LPVOID*)&pGDIBits, NULL, 0);

  memcpy(pGDIBits, pBmpBits, pBmp->biSizeImage);

  HDC hBmpDC = CreateCompatibleDC(hdc);

  HGDIOBJ pOldObj = SelectObject(hBmpDC, hBmp);

  if (mStretch)
  {
    StretchBlt(hdc, 0, 0, mSurfaceW, mSurfaceH, hBmpDC, 0, 0, pBmp->biWidth, pBmp->biHeight, SRCCOPY);
  }
  else
  {
    if (mPStretch)
    {
      StretchBlt(hdc, 0, 0, mSurfaceW, mSurfaceH, hBmpDC, 0, 0, pBmp->biWidth, pBmp->biHeight, SRCCOPY);
    }
    else
    {
      BitBlt(hdc, 0, 0, mSurfaceW, mSurfaceH, hBmpDC, 0, 0, SRCCOPY);
    }
  }

  if(pOldObj) SelectObject(hBmpDC, pOldObj);
  DeleteObject(hBmp);
  DeleteDC(hBmpDC);

  mSurface->ReleaseDC(hdc);

  TRect lDstRect;

  if (mCenter && !mAutoSize && !mStretch)
  {
    lDstRect = Rect(lL + GetWidth()  / 2 - mSurfaceW / 2,
                    lT + GetHeight() / 2 - mSurfaceH / 2,
                    lL + GetWidth()  / 2 + mSurfaceW / 2,
                    lT + GetHeight() / 2 + mSurfaceH / 2);
  }
  else
  {
    lDstRect = Rect(lL, lT, lL + mSurfaceW, lT + mSurfaceH);
  }

  GetGraphicInstance()->PaintSurfaceToSurface(
    NULL,
    lDstRect,
    mSurface,
    Rect(0, 0, mSurfaceW, mSurfaceH)
    );

  //GetGraphicInstance()->Rectangle(NULL, Rect(lL + 1, lT + 1, lL + GetWidth() - 1, lT + GetHeight() - 1), 0xFFFFFF); 
}
//---------------------------------------------------------------------------

void CPP_CAvi::Close()
{
  if (mValid)
  {
    if (AVIStreamGetFrameClose(mVideoPGF[0]))
    {
      return;
    }

    for(int i = 0; i < mNumVideoStreams; i++)
    {
      if(mVideoStreams[i])
      {
        AVIStreamRelease(mVideoStreams[i]);
      }

      if(mVideoFormats[i])
      {
        delete [] ((LPBYTE)mVideoFormats[i]);
      }
    }

    GetGraphicInstance()->ReleaseSurface(mSurface);
  }

  AVIFileRelease(mAviHandle);
}
//---------------------------------------------------------------------------

void CPP_CAvi::CreateSurface()
{
  if (!mValid)
  {
    return;
  }

  if (mSurface)
  {
    GetGraphicInstance()->ReleaseSurface(mSurface);
  }

  if (mStretch)
  {
    mSurface = GetGraphicInstance()->CreateSurface(GetWidth(), GetHeight());
    mSurfaceW = GetWidth();
    mSurfaceH = GetHeight();
    return;
  }

  if (mPStretch)
  {
    float lFactor = (float)mVideoFormats[0]->bmiHeader.biWidth/mVideoFormats[0]->bmiHeader.biHeight;

    if (lFactor >= 1.0)
    {
      mSurface  = GetGraphicInstance()->CreateSurface(GetWidth(), GetWidth() / lFactor);
      mSurfaceW = GetWidth();
      mSurfaceH = GetWidth() / lFactor;
    }
    else
    {
      mSurface  = GetGraphicInstance()->CreateSurface(GetHeight() * lFactor, GetHeight());
      mSurfaceW = GetHeight() * lFactor;
      mSurfaceH = GetHeight();
    }

    return;
  }

  mSurface  = GetGraphicInstance()->CreateSurface(mVideoFormats[0]->bmiHeader.biWidth, mVideoFormats[0]->bmiHeader.biHeight);
  mSurfaceW = mVideoFormats[0]->bmiHeader.biWidth;
  mSurfaceH = mVideoFormats[0]->bmiHeader.biHeight;

  logWriteLn("Dimensiunile :");
  logWriteFLn(mSurfaceW);
  logWriteFLn(mSurfaceH);
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetStretch(bool _s)
{
  mStretch = _s;
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetPStretch(bool _ps)
{
  mPStretch = _ps;
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetAutoSize(bool _a)
{
  if (!mValid)
  {
    return;
  }

  mAutoSize = _a;

  if (mAutoSize)
  {
    SetWidth(mVideoFormats[0]->bmiHeader.biWidth);
    SetHeight(mVideoFormats[0]->bmiHeader.biHeight);
  }
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetWidth(int _width)
{
  CPIControl::SetWidth(_width);

  // autosize == true : chiar daca se seteaza width'ul, controlul trebuie sa fie dimensionat
  // la dimensiunile filmului
  SetAutoSize(GetAutoSize());
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetHeight(int _height)
{
  CPIControl::SetHeight(_height);

  // autosize == true : chiar daca se seteaza height'ul, controlul trebuie sa fie dimensionat
  // la dimensiunile filmului
  SetAutoSize(GetAutoSize());
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::Resize(int _width, int _height)
{
  CPIControl::SetWidth(_width);
  CPIControl::SetHeight(_height);
  SetAutoSize(GetAutoSize());
  CreateSurface();
}
//---------------------------------------------------------------------------

void CPP_CAvi::SetHandleOnEndPlay (void (__closure* value)(CPIObject *Sender))
{
  mHandleOnEndPlay = value;
}
//---------------------------------------------------------------------------

void CPP_CAvi::OnEndPlay()
{
   if (mHandleOnEndPlay != NULL)
       mHandleOnEndPlay(this);
}
//---------------------------------------------------------------------------

CPP_CAvi::~CPP_CAvi()
{
  Close();
  AVIFileExit();
}
//---------------------------------------------------------------------------
*/

#endif // __WIN32__
