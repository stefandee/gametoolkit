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
//  Unit              : SDL Sound System interface
//
//  Version           : 2.0
//
//  Description       :
//    * a SDL implementation of the sound system used by the engine
//
//  History           :
//    [02.04.2004] - [Karg]  - SDL integration
//    [09.09.2004] - [Karg]  - volume is now expressed in percents (0..100)
//                             added VolumeToDeviceValue and VerifyVolume to
//                             deal with this
//    [24.09.2004] - [Karg]  - panning is expressed in range (-100%...100%); that is
//                             -100% means full volume on left and silence on right while
//                             100% means full volume on right and silence on left
//-----------------------------------------------------------------------------

#ifndef PP_SoundH
#define PP_SoundH
//---------------------------------------------------------------------------
//#include <windows.h>
//#include <mmsystem.h>
//#include "dsound.h"
#include "PP_String.h"

#define SS_SAMPLE_RATE    22050
#define SS_BITSPERSAMPLE  16
#define SS_CHANNELS       1

const int SS_MAX_CHANNELS = 16;

#define SoundPath "..\\SND\\"

#define MaxSoundsHandled 512

// replace with platform specific
const int SS_VOL_MAX = 0;
const int SS_VOL_MIN = 0;

enum TPlayStyle { not_looped, looped };

// o intrare de sunet
typedef struct SSoundEntry_t
{
  SSoundEntry_t() // TMN: Added c'tor to not use uninitialized memory
  : nInstances(0),
    Current(0),
	mFileName("empty"),
	used(false),
	enabled(false),
	volume(0),
	playStyle(not_looped),
    mMixChunk(0)
  {}

  int nInstances;
  int Current;
  CPString mFileName;

  // replace with platform specific
  void* mMixChunk;

  bool used;
  bool enabled;
  long volume;
  TPlayStyle playStyle;
} TSoundEntry;

// clasa de baza in sistemul de sunet
class CSoundSystem
{
  public:
    class CErrorGeneric{};

  public:
    //static CSoundSystem* Instance(void* hWnd = NULL);
    static CSoundSystem* Instance();
    static void   Release();

    // misc routines
    CPString ErrorReport(int _v);

    // initializari
    bool PowerOn(void* hWnd = 0);
    bool OnLine();

    // sound entries management
    bool Play(int what_sound, int pan_value, TPlayStyle playStyle);
    bool PlayWithVol(int what_sound, int pan_value, int vol_value);
    bool Play(int what_sound, int pan_value);

    bool Stop(int what_sound);
    bool SetVolume(int what_sound, long vol_value);
    int  GetVolume(int _whatSound);
    bool SetEnabled(int what_sound, bool enabled);
    bool SetPlayType(int what_sound, TPlayStyle playStyle);
    bool Load(int what_sound, const char* filename, int instances);
    unsigned int LoadNew(const char* filename, int instances);

    int  GetMasterVolume();
    void SetMasterVolume(int _volume);

    void setEnabled(bool enabled);           //added by grabX

  protected:
    CSoundSystem(void);
    ~CSoundSystem();

  private:
    static CSoundSystem* mInstance;
    TSoundEntry*         sounds[MaxSoundsHandled];
    bool                 Enabled;
    int                  mMasterVolume;

    //SDL_AudioSpec        mAudioSpec;

    int VolumeToDeviceValue(int _percent);
    int VerifyVolume(int _percent);

    void PanToDeviceValue(int _percent, int* _left, int* _right);
    int  VerifyPan(int _percent);
};

typedef CSoundSystem CPSISoundSystem;

//CPSISoundSystem* GetSoundInstance(void* hWnd = NULL);
CPSISoundSystem* GetSoundInstance();

#endif
