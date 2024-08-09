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
//#include <windows.h>
#include <fcntl.h>
//#include <io.h>
#include <string.h>
#pragma hdrstop

#include "PP_Sound.h"
#include "logfile.h"
#include "SWavLoad.h"
#include "SVocLoad.h"
#include "SAuLoad.h"
#include "SPSndLoad.h"
#include "logOut.h"

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
  logOut << "AUDIO SYSTEM STOPPING...\n";

  // stop everything related to the channel playing
  //Mix_HaltChannel(-1);

  for(int i = 0; i < MaxSoundsHandled; i++)
  {
    if (sounds[i]->used == true)
    {
       // free the memory used by the data structure - Mix_FreeChunk is not
       // able to free mem alloc'ed with new, so we'll do it ourselves
       // Mix_FreeChunk(sounds[i]->mMixChunk);

       //delete [] sounds[i]->mMixChunk->abuf;
       //delete sounds[i]->mMixChunk;

       // sterg din memorie intrarea
       delete sounds[i];
    }
  }

  // and close the mixer

  logOut << "AUDIO SYSTEM STOPPED...\n";
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
  logOut << "AUDIO SYSTEM INITIALIZING...\n";

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
   logOut << "CSoundSystem::LoadNew - " << filename << "\n";

   if (Enabled == false)
      return -1;

   for (i=0; i<MaxSoundsHandled; i++)
   {
      if (sounds[i]->used == false)
      {
         if (Load(i, filename, instances))
         {
            logWriteLn("...OK");
            return i;
         }
         else
         {
            logWriteLn("...NOT OK");
            return -1;
         }
      }
      else
         if (CPString(filename) == sounds[i]->mFileName)
         {
            logWriteLn("...ALREADY LOADED");
            return i;
         }
   }

   logWriteLn("...MAX SOUNDS LIMIT REACHED");
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

  // set volume to play...
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
bool CSoundSystem::OnLine()
{
   return Enabled;
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
  SWavLoad      wavLoad;
  //SAuLoad       auLoad;
  //SVocLoad      vocLoad;
  //SPSndLoad     pSndLoad;
  TSndInfo      sndInfo;
  enum TFileFormat {IS_WAV, IS_RAW, IS_VOC, IS_AU, IS_PIRON, IS_UNKNOWN} fileFormat = IS_UNKNOWN;

  //daca sistemul a fost disabled atunci se iese din metoda
  if (!Enabled)
    return true;

  if (sounds[what_sound]->used == true)
  {
    // destroy buffer - cum ?
  }

  // incarcarea efectiva din fisier

  // e WAV ?
  if (!wavLoad.Open(filename))
  {
    /*
    // e AU ?
    if (!auLoad.Open(filename))
    {
      // e VOC ?
      if (!vocLoad.Open(filename))
      {
        // e pSnd ?
        if (!pSndLoad.Open(filename))
        {
          // atunci e RAW (adica nu are format)
          logOut << "Sound file " << filename << " has no format \n";
          return false;
        }
        else
        {
          pSndLoad.GetSndInfo(&sndInfo);
          fileFormat = IS_PIRON;
        }
      }
      else
      {
        vocLoad.GetSndInfo(&sndInfo);
        fileFormat = IS_VOC;
      }
    }
    else
    {
      auLoad.GetSndInfo(&sndInfo);
      fileFormat = IS_AU;
    }
    */
  }
  else
  {
    wavLoad.GetSndInfo(&sndInfo);
    fileFormat = IS_WAV;
  }

  // verific daca formatul fisierului de sunet corespunde cu SS_SAMPLE_RATE, SS_BITS..., SS_CHANNELS
  if ((sndInfo.channels      != SS_CHANNELS)      ||
      (sndInfo.sampleRate    != SS_SAMPLE_RATE)   ||
      (sndInfo.bitsPerSample != SS_BITSPERSAMPLE))
  {
    logWriteLn("Params do not match for sound");
    return false;
  }

  // TODO: setting up the Mix_Chunk

  // citesc din fisier in functie de format
  switch(fileFormat)
  {
    case IS_WAV :
      break;

    case IS_AU :
      break;

    case IS_VOC :
      break;

    case IS_PIRON :
      break;

    default :
      break;
  }

  // setez si valorile interne
  sounds[what_sound]->mFileName  = CPString(filename);
  sounds[what_sound]->used       = true;
  sounds[what_sound]->Current    = 0;
  sounds[what_sound]->nInstances = instances;

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

  // TODO: call the system volume setting
}
//---------------------------------------------------------------------------

//added by grabX
void CSoundSystem::setEnabled(bool enabled)
{
   Enabled = enabled;
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


