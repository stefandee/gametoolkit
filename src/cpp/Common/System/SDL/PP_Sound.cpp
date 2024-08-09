//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2017 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the      //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
#include <fcntl.h>
#include <string.h>
#pragma hdrstop

#include "PP_Sound.h"
#include "extras/physfsrwops.h"
#include "easylogging++.h"

//CSoundSystem* gSoundSystem = 0;

CSoundSystem* CSoundSystem::mInstance = 0;

CPSISoundSystem* GetSoundInstance()
{
    return CPSISoundSystem::Instance();
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza singura instanta posibila
 parameters : hwnd - optional daca vrei numai o instanta
---------------------------------------------------------------------------*/
//CSoundSystem* CSoundSystem::Instance(void* hWnd)
CSoundSystem* CSoundSystem::Instance()
{
    if (mInstance == NULL)
    {
        mInstance = new CSoundSystem();

        if (mInstance->PowerOn() == false)
        {
            throw CErrorGeneric();
        }
    }

    return mInstance;
}

/*---------------------------------------------------------------------------
 description: face delete
---------------------------------------------------------------------------*/
void CSoundSystem::Release()
{
    if (mInstance != NULL)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
// descriere : constructorul obiectului CSoundSystem
//             de fapt, mai mult e o vrajeala ;-)
//---------------------------------------------------------------------------
CSoundSystem::CSoundSystem()
{
    // poate exista o singura instanta din aceasta clasa
    // soundSystemInstances = 0;

    Enabled = true;

    //direct_sound = NULL;
    //primary_buffer = NULL;

    for(int i = 0; i < MaxSoundsHandled; i++)
    {
        sounds[i] = new TSoundEntry;
        sounds[i]->used = false;
    }
}

//---------------------------------------------------------------------------
// descriere : destructorul obiectului CSoundSystem
//---------------------------------------------------------------------------
CSoundSystem::~CSoundSystem()
{
    LOG(INFO) << "AUDIO SYSTEM STOPPING...";

    // stop everything related to the channel playing
    Mix_HaltChannel(-1);

    for(int i = 0; i < MaxSoundsHandled; i++)
    {
        if (sounds[i]->used == true)
        {
            // free the memory used by the data structure - Mix_FreeChunk is not
            // able to free mem alloc'ed with new, so we'll do it ourselves
            // Mix_FreeChunk(sounds[i]->mMixChunk);
            delete [] sounds[i]->mMixChunk->abuf;

            delete sounds[i]->mMixChunk;

            // sterg din memorie intrarea
            delete sounds[i];
        }
    }

    // and close the mixer
    Mix_CloseAudio();

    LOG(INFO) << "AUDIO SYSTEM STOPPED.";
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// descriere : init SDL audio
// probleme  : raportarea erorilor ?
//---------------------------------------------------------------------------
bool CSoundSystem::PowerOn(void* hWnd)
{
    LOG(INFO) << "AUDIO SYSTEM INITIALIZING...";

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 8192) < 0)
    {
        LOG(FATAL) << CPString("Couldn't open audio: ") + SDL_GetError();
        return false;
    }

    // allocate a number of mixing channels
    int lChannels = Mix_AllocateChannels(SS_MAX_CHANNELS);

    //
    // logging of real specs
    // mixing SDL_mixer and SDLaudio for this call is no problem at all ;)
    //
    int lAudioRate, lAudioChannels;
    Uint16 lAudioFormat;

    //char lDrvName[256];
    //SDL_AudioDriverName(lDrvName, 256);

    const char* lDrvName = SDL_GetCurrentAudioDriver();

    if (Mix_QuerySpec(&lAudioRate, &lAudioFormat, &lAudioChannels) != 0)
    {
        VLOG(1) << "...opened audio " << lDrvName << "(" << lChannels << " channels) at " << lAudioRate << " Hz " << (lAudioFormat&0xFF) << " bit " << ((lAudioChannels > 1) ? "stereo" : "mono");
    }
    else
    {
        VLOG(1) << "...cannot query the audio specs " << Mix_GetError();
    }

    LOG(INFO) << "AUDIO SYSTEM INITIALIZED.";

    Enabled = true;

    return true;
}

//---------------------------------------------------------------------------
// descriere : returneaza un string dintr'o eroare
//---------------------------------------------------------------------------
CPString CSoundSystem::ErrorReport(int error)
{
    return CPString("no error");
}

/*--------------------------------------------------------------------------
 Description: incarca un sunet nou si returneaza indexul
              retunreaza -1 daca este eroare
--------------------------------------------------------------------------*/
unsigned int CSoundSystem::LoadNew(const char* filename, int instances)
{
    int i;

    //daca sistemul a fost disabled atunci se iese din metoda
    VLOG(1) << "CSoundSystem::LoadNew - " << filename;

    if (Enabled == false)
        return -1;

    for (i=0; i<MaxSoundsHandled; i++)
    {
        if (sounds[i]->used == false)
        {
            if (Load(i, filename, instances))
            {
                VLOG(1) << "...OK";
                return i;
            }
            else
            {
                VLOG(1) << "...NOT OK";
                return -1;
            }
        }
        else if (CPString(filename) == sounds[i]->mFileName)
        {
            VLOG(1) << "...ALREADY LOADED";
            return i;
        }
    }

    LOG(INFO) << "...MAX SOUNDS LIMIT REACHED";
    return -1;
}

//---------------------------------------------------------------------------
// descriere : evident
//---------------------------------------------------------------------------
bool CSoundSystem::Play(int what_sound, int pan_value, TPlayStyle playtype)
{
    //daca sistemul a fost disabled atunci se iese din metoda
    if (!Enabled)
    {
        return true;
    }

    if (what_sound < 0)
    {
        return false;
    }

    if (!sounds[what_sound]->used)
    {
        return false;
    }

    // initial value should be 0 (no loop, no play)...i guess :)
    int lPlayStyleValue = 1;

    switch(playtype)
    {
    case not_looped:
        lPlayStyleValue = 1;
        break;

    case looped:
        lPlayStyleValue = -1;
        break;
    }

    // should also set the volume to play...
    Mix_VolumeChunk(sounds[what_sound]->mMixChunk, sounds[what_sound]->volume * GetMasterVolume() / 100);

    int lChannel = Mix_PlayChannel(-1, sounds[what_sound]->mMixChunk, lPlayStyleValue);

    if (lChannel == -1)
    {
        LOG(ERROR) << "CSoundSystem::Play - play channel failed.";
        return false;
    }

    // should also set panning...
    int lPanLeft, lPanRight;

    PanToDeviceValue(pan_value, &lPanLeft, &lPanRight);

    Mix_SetPanning(lChannel, lPanLeft, lPanRight);

    return true;
}

//---------------------------------------------------------------------------
// descriere : la fel ca cea de sus, dar se cinta cu volum
//---------------------------------------------------------------------------
bool CSoundSystem::PlayWithVol(int what_sound, int pan_value, int vol_value)
{
    //daca sistemul a fost disabled atunci se iese din metoda
    if (!Enabled)
    {
        return true;
    }

    // verify the volume
    int lVerifiedVolume = VerifyVolume(vol_value);

    SetVolume(what_sound, lVerifiedVolume);

    Play(what_sound, pan_value);

    return true;
}

//---------------------------------------------------------------------------
// descriere : la fel ca cea de sus, doar ca-si ia
//             playtype-ul din variabila ei
//---------------------------------------------------------------------------
bool CSoundSystem::Play(int what_sound, int pan_value)
{
    //daca sistemul a fost disabled atunci se iese din metoda
    if (!Enabled)
    {
        return false;
    }

    if (what_sound < 0)
    {
        return false;
    }

    if (!sounds[what_sound]->used)
    {
        return false;
    }

    Play(what_sound, pan_value, sounds[what_sound]->playStyle);

    return true;
}

//---------------------------------------------------------------------------
// descriere : la fel de evident; inca netestata
// comments  : in caz ca sunetul nu putea fi incarcat, se genera exceptie
//---------------------------------------------------------------------------
bool CSoundSystem::Stop(int what_sound)
{
    //daca sistemul a fost disabled atunci se iese din metoda
    if (!Enabled)
    {
        return true;
    }

    if (what_sound < 0)
    {
        return false;
    }

    //Mix_HaltChannel(sounds[what_sound]->mMixChunk);

    return false;
}

//---------------------------------------------------------------------------
// descriere : testata de Grab
//---------------------------------------------------------------------------
bool CSoundSystem::SetVolume(int what_sound, long vol_value)
{
    //daca sistemul a fost disabled atunci se iese din metoda
    if (!Enabled)
    {
        return true;
    }

    if (what_sound < 0)
    {
        return true;
    }

    sounds[what_sound]->volume = VolumeToDeviceValue(vol_value);

    return true;
}

// ghici ce ?
int  CSoundSystem::GetVolume(int _whatSound)
{
    if (!Enabled)
        //daca sistemul a fost disabled atunci se iese din metoda
        return true;

    if (_whatSound < 0)
    {
        return true;
    }

    if (sounds[_whatSound]->used == false)
        return true;

    return sounds[_whatSound]->volume;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// descriere : idem; inca netestata
//---------------------------------------------------------------------------
bool CSoundSystem::SetEnabled(int what_sound, bool enabled)
{
    //daca sistemul a fost disabled atunci se iese din metoda
    if (!Enabled)
    {
        return true;
    }

    if (what_sound < 0)
    {
        return false;
    }

    sounds[what_sound]->enabled = enabled;

    if (enabled)
    {
        return (SetVolume(what_sound, sounds[what_sound]->volume));
    }
    else
    {
        return Stop(what_sound);
    }
}

//---------------------------------------------------------------------------
// descriere : idem; inca netestata
//---------------------------------------------------------------------------
bool CSoundSystem::SetPlayType(int what_sound, TPlayStyle playStyle)
{
    if (!Enabled)
        //daca sistemul a fost disabled atunci se iese din metoda
        return true;

    if (what_sound < 0)
    {
        return false;
    }

    sounds[what_sound]->playStyle = playStyle;
    return true;
}

/*--------------------------------------------------------------------------
 Description: arata daca sound system este enabled sau nu
--------------------------------------------------------------------------*/
bool CSoundSystem::GetEnabled()
{
    return Enabled;
}
//---------------------------------------------------------------------------

//added by grabX
void CSoundSystem::SetEnabled(bool enabled)
{
    Enabled = enabled;
}
//---------------------------------------------------------------------------


// Descriere   : incarca un sunet intr'o pozitie specificata (util pentru muzica);
//               este un fel de interfata cu functiile de incarcare efectiva;
//               sint suportate fisiere de tip WAV, RAW si VOC
// Param       : pozitia, fisierul, numarul de instante ale sunetului
// Rezultat    : true la reusita, false altfel
// Comentarii  : n/a
bool CSoundSystem::Load(int what_sound, const char* filename, int instances)
{
    if (!Enabled)
        return true;

    if (sounds[what_sound]->used == true)
    {
        // destroy buffer
        Mix_FreeChunk(sounds[what_sound]->mMixChunk);
    }

    SDL_RWops* rwops = PHYSFSRWOPS_openRead(filename);

    if (!rwops)
    {
        return false;
    }

    // setting up the Mix_Chunk
    sounds[what_sound]->mMixChunk = Mix_LoadWAV_RW(rwops, 1);

    if (!sounds[what_sound]->mMixChunk)
    {
        return false;
    }

    //sounds[what_sound]->mMixChunk->volume = VolumeToDeviceValue(50);

    // setez si valorile interne
    sounds[what_sound]->mFileName  = CPString(filename);
    sounds[what_sound]->used       = true;
    sounds[what_sound]->Current    = 0;
    sounds[what_sound]->nInstances = instances;
    sounds[what_sound]->volume     = VolumeToDeviceValue(100);

    sounds[what_sound]->enabled    = true;
    sounds[what_sound]->playStyle  = not_looped;

    return true;
}
//---------------------------------------------------------------------------

int  CSoundSystem::GetMasterVolume()
{
    return mMasterVolume;
}
//---------------------------------------------------------------------------

void CSoundSystem::SetMasterVolume(int _volume)
{
    /*
    if (_volume > SS_VOL_MAX)
    {
      _volume = SS_VOL_MAX;
    }
    else
    {
      if (_volume < SS_VOL_MIN)
      {
        _volume = SS_VOL_MIN;
      }
    }
    */

    mMasterVolume = VerifyVolume(_volume);

    Mix_Volume(-1, VolumeToDeviceValue(mMasterVolume));
}
//---------------------------------------------------------------------------

// parameter is a value between 0..100 (a percent)
// the return value will scale the percent to the device values (hence the method
// name ;)
int CSoundSystem::VolumeToDeviceValue(int _percent)
{
    return SS_VOL_MIN + (SS_VOL_MAX - SS_VOL_MIN) * (_percent / 100.);
}
//---------------------------------------------------------------------------

// the volumes should be in 0..100 range; this method verifies the input value
// and performs changes on it
int CSoundSystem::VerifyVolume(int _percent)
{
    if (_percent < 0)
    {
        _percent = 0;
    }

    if (_percent > 100)
    {
        _percent = 100;
    }

    return _percent;
}
//---------------------------------------------------------------------------

void CSoundSystem::PanToDeviceValue(int _percent, int* _left, int* _right)
{
    int lVerifiedPercent = VerifyPan(_percent);

    if (lVerifiedPercent < 0)
    {
        *_left  = -255 * lVerifiedPercent / 100;
        *_right = 255 - *_left;
    }

    if (lVerifiedPercent > 0)
    {
        *_right  = 255 * lVerifiedPercent / 100;
        *_left = 255 - *_right;
    }

    if (lVerifiedPercent == 0)
    {
        *_right = 255;
        *_left  = 255;
    }
}
//---------------------------------------------------------------------------

int  CSoundSystem::VerifyPan(int _percent)
{
    if (_percent < -100)
    {
        _percent = -100;
    }

    if (_percent > 100)
    {
        _percent = 100;
    }

    return _percent;
}
//---------------------------------------------------------------------------


