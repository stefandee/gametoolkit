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
//-----------------------------------------------------------------------------
//  Unit              : Avi Player Control
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * control ce contine un avi; in mod normal ar trebui sa existe un loader de
//      genul GLoad/SLoad pentru avi'uri, insa se pare ca avi'urile depind mult
//      prea mult de Windoze (codecuri audio-video)
//    * este echivalent cu MediaPlayer/VCL
//    * sint suportate si liste de avi'uri
//    * E UN MARE SI IMENS CACAT CE SA INTIMPLA IN ACEST FISIER!!!
//
//  Istorie           :
//    [] - [] -
//    [18.02.2001] - [Karg] - am adaugat suport pentru playlisturi
//-----------------------------------------------------------------------------
#ifndef PP_CAviH
#define PP_CAviH
//---------------------------------------------------------------------------

// this file will get compiled only on windows
#ifdef __WIN32__

#include "Windows.h"
#include "vfw.h"
#include "PP_CControl.h"
#include "PP_StringList.h"

#define MAX_VIDEO_STREAMS 4
#define MAX_AUDIO_STREAMS 4

enum TPP_AviStatus { AS_PLAY, AS_STOP, AS_PAUSE };

struct TAviVideoData
{
  int          mNumVideoStreams;
  bool         mValid;
  LPBITMAPINFO mVideoFormats[MAX_VIDEO_STREAMS];
  PGETFRAME    mVideoPGF    [MAX_VIDEO_STREAMS];
  LONG         mVideoEndTime[MAX_VIDEO_STREAMS];
  PAVISTREAM   mVideoStreams[MAX_VIDEO_STREAMS];
};
//---------------------------------------------------------------------------

struct TAviAudioData
{
  int          mNumAudioStreams;
  bool         mValid;
  PAVISTREAM   mAudioStreams[MAX_AUDIO_STREAMS];
  LPWAVEFORMAT mAudioFormats[MAX_AUDIO_STREAMS];
  LPBYTE       mAudioData   [MAX_AUDIO_STREAMS];
};
//---------------------------------------------------------------------------

class CPP_CAvi : public CPIControl
{
  private:
    PAVIFILE      mAviHandle;
    bool          mPStretch, mStretch, mCenter, mAutoSize;
    int           mTime; // timpul, in milisecunde
    TPP_AviStatus mStatus;

    // playlist
    CPStringList* mPlayList;
    int           mPlayIndex;

    TAviVideoData mVideoData;
    TAviAudioData mAudioData;

    CPGISurface*  mSurface;
    int           mSurfaceW, mSurfaceH;

    // metode interne
    bool InternalOpen(CPString _fileName);

    bool PrepareVideo();
    bool PrepareAudio();
    bool ReleaseVideo();
    bool ReleaseAudio();
    void NextFile();

    void InternalClose();

    // events
    void (__closure* mHandleOnEndPlayList)(CPIObject *Sender);
    void OnEndPlayList();

    void (__closure* mHandleOnEndPlay)(CPIObject *Sender);
    void OnEndPlay();

    // misc
    void CreateSurface();

  protected:

  public:
    // basic interface
    CPP_CAvi(CPIWinControl *Parent = NULL);

    bool Open(CPString     _fileName);
    bool Open(CPStringList* _list);
    void Play();
    void Stop();
    void Pause();
    void Close();

    virtual void Update();
    virtual void Paint();

    ~CPP_CAvi();

    // set-get'uri
    void SetStretch(bool _s);
    bool GetStretch() { return mStretch; }

    void SetPStretch(bool _ps);
    bool GetPStretch() { return mPStretch; }

    void SetCenter(bool _c) { mCenter = _c; }
    bool GetCenter() { return mCenter; }

    void SetAutoSize(bool _a);
    bool GetAutoSize() { return mAutoSize; }

    int  GetPlayTime() { return mTime; };
    void SetPlayTime();

    TPP_AviStatus GetStatus() { return mStatus; };

    // size stuff
    virtual void SetWidth(int _width);
    virtual void SetHeight(int _height);
    virtual void Resize(int _width, int _height);

    // events sets
    virtual void SetHandleOnEndPlay (void (__closure* value)(CPIObject *Sender));
    virtual void SetHandleOnEndPlayList (void (__closure* value)(CPIObject *Sender));
};

/*
class CPP_CAvi : public CPIControl
{
private:
  PAVIFILE      mAviHandle;
  bool          mValid;
  bool          mPStretch, mStretch, mCenter, mAutoSize;
  int           mTime; // timpul, in milisecunde
  TPP_AviStatus mStatus;
  CPGISurface*  mSurface;
  int           mSurfaceW, mSurfaceH;


  void (__closure* mHandleOnEndPlay)(CPIObject *Sender);
  void OnEndPlay();

  void CreateSurface();

protected:

public:
  CPP_CAvi(CPIWinControl *Parent = NULL);

  bool Open(CPString _fileName);
  void Play();
  void Stop();
  void Pause();
  void Mute();
  void ChangePosition();
  void Close();

  virtual void Update();
  virtual void Paint();

  ~CPP_CAvi();

  // citeva get-seturi
  void SetStretch(bool _s);
  bool GetStretch() { return mStretch; }

  void SetPStretch(bool _ps);
  bool GetPStretch() { return mPStretch; }

  void SetCenter(bool _c) { mCenter = _c; }
  bool GetCenter() { return mCenter; }

  void SetAutoSize(bool _a);
  bool GetAutoSize() { return mAutoSize; }

  virtual void SetWidth(int _width);
  virtual void SetHeight(int _height);
  virtual void Resize(int _width, int _height);

  // events set-gets
  virtual void SetHandleOnEndPlay (void (__closure* value)(CPIObject *Sender));
};
*/

#endif // __WIN32__

#endif
