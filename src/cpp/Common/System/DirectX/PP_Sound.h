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
//  Unit              : Sound System interface
//
//  Version           : 1.12
//
//  Description       :
//    * structuri de date si prototipuri pentru manipulat sunete plus initializarea sistemului de sunet si
//      setari pentru PrimaryBuffer.
//    * din cauza balamucului din bucataria interna directsound'ului e imposibil sa se mentina o structura
//      scalara si portabila. Eventual dupa vreo citeva luni de familiarizare si aprofundare a DxSound sa pot
//      face ceva...
//
//  History           :
//    [08.07.1999] - [Karg] - am facut LoadAsWav si am modificat Load
//    [08.11.1999] - [Karg] - am descoperit 2 buguri;
//                          - unul la incarcarea din fisier (daca nu se putea incarca,
//                            (sunetul era tratat ca si cum s'ar fi reusit incarcarea)
//                          - altul la crearea Primary Bufferului : nu se facea
//                            SetFormat pentru a seta parametrii mixerului din DirectSound,
//                            astfel incit sunetul playat era de foarte proasta calitate
//    [06.02.2000] - [Karg] - am adaugat 'soundSystemInstances', pentru ca sistemul de
//                            sunet sa nu poata fi initailizat de mai multe ori
//    [07.02.2000] - [Karg] - am sters LoadAs..., pentru ca au aparut clasele SLoad
//                          - metoda Load s'a modificat radical
//    [08.02.2000] - [Karg] - am testat noua formula de incarcare cu clase SLoad
//    [19.02.2002] - [Tmn]  - added default constructor to the TSoundEntry
//    [09.09.2004] - [Karg] - added VolumeToDeviceValue() method; please note that from now on
//                            volume (for methods that require such a value) must be send as a
//                            value between 0..100 (device independence ;)
//
//-----------------------------------------------------------------------------

#ifndef PP_SoundH
#define PP_SoundH
//---------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include "dsound.h"

#define SS_SAMPLE_RATE    22050
#define SS_BITSPERSAMPLE  16
#define SS_CHANNELS       1

#define SoundPath "..\\SND\\"
#define MaxSoundInstances 12   // se pp ca nu se va cere ca un sunet sa se
                               // playeze de mai mult de MaxSoundInstances
#define MaxSoundsHandled 512

#define SS_PAN_LEFT   -10000
#define SS_PAN_RIGHT  10000
#define SS_PAN_CENTER 0

#define SS_VOL_MAX    0
#define SS_VOL_MIN    -10000

enum TPlayStyle { not_looped, looped };

// o intrare de sunet
typedef struct SSoundEntry_t
{
  SSoundEntry_t() // TMN: Added c'tor to not use uninitialized memory
  : nInstances(0),
    Current(0),
	pszFileName(0),
	used(false),
	enabled(false),
	volume(0),
	playStyle(not_looped)
  {}

  int nInstances;
  int Current;
  char* pszFileName;
  LPDIRECTSOUNDBUFFER lpSoundBuffer[MaxSoundInstances];
  bool used;
  bool enabled;
  LONG volume;
  TPlayStyle playStyle;
} TSoundEntry;

// clasa de baza in sistemul de sunet
class CSoundSystem
{
  public:
    class CErrorGeneric{};

  public:
    static CSoundSystem* Instance(void* hWnd = NULL);
    static void   Release();

    // misc routines
    char* ErrorReport(int);

    // initializari
    bool PowerOn(void* hWnd);
    bool GetCaps(void);
    bool OnLine();

    // sound entries management
    bool Play(int what_sound, int pan_value, TPlayStyle playStyle);
    bool PlayWithVol(int what_sound, int pan_value, int vol_value);
    bool Play(int what_sound, int pan_value);
    bool Stop(int what_sound);
    bool SetVolume(int what_sound, LONG vol_value);
    int  GetVolume(int _whatSound);
    bool SetEnabled(int what_sound, bool enabled);
    bool SetPlayType(int what_sound, TPlayStyle playStyle);
    bool Load(int what_sound, const char* filename, int instances);
    UINT LoadNew(const char* filename, int instances);

    int  GetMasterVolume();
    void SetMasterVolume(int _volume);

    void setEnabled(bool enabled);           //added by grabX

  protected:
    CSoundSystem(void);
    ~CSoundSystem();

  private:
    static int soundSystemInstances;
    static CSoundSystem* mInstance;
    LPDIRECTSOUND       direct_sound;
    LPDIRECTSOUNDBUFFER primary_buffer;
    TSoundEntry*        sounds[MaxSoundsHandled];
    DSCAPS              ds_caps;
    bool                Enabled;
    int                 mMasterVolume;

    LPDIRECTSOUNDBUFFER GetFreeBuffer(int what_sound);

    int VolumeToDeviceValue(int _percent);
    int VerifyVolume(int _percent);

    int PanToDeviceValue(int _percent);
    int VerifyPan(int _percent);
};

typedef CSoundSystem CPSISoundSystem;

CPSISoundSystem* GetSoundInstance(void* hWnd = NULL);

#endif
