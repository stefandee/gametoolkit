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
#include <io.h>
//#include <vcl.h>
#pragma hdrstop

#include "PP_Sound.h"
#include "logfile.h"
#include "SWavLoad.h"
#include "SVocLoad.h"
#include "SAuLoad.h"
#include "SPSndLoad.h"

CSoundSystem* gSoundSystem = NULL;

int CSoundSystem::soundSystemInstances = 0;
CSoundSystem* CSoundSystem::mInstance = NULL;

CPSISoundSystem* GetSoundInstance(void* hWnd)
{
  return CPSISoundSystem::Instance(hWnd);
  /*
   CPSISoundSystem* lSSI;
   return lSSI->Instance(hWnd);
  */
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza singura instanta posibila
 parameters : hwnd - optional daca vrei numai o instanta
---------------------------------------------------------------------------*/
CSoundSystem* CSoundSystem::Instance(void* hWnd)
{
   if (mInstance == NULL)
   {
      mInstance = new CSoundSystem();
      if (mInstance->PowerOn(hWnd) == false)
         throw CErrorGeneric();
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
  soundSystemInstances = 0;

  Enabled = true;
  direct_sound = NULL;
  primary_buffer = NULL;
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
  soundSystemInstances = 0;
  for(int i = 0; i < MaxSoundsHandled; i++)
  {
    if (sounds[i]->used == true)
    {
       // opresc toate sunetele
       for(int j = 0; j < sounds[i]->nInstances; j++)
       {
         sounds[i]->lpSoundBuffer[j]->Stop();
       }

       // releasuiesc bufferul principal (cel dupa care se fac duplicatele)
       sounds[i]->lpSoundBuffer[0]->Release();

       // sterg din memorie intrarea
       delete [] sounds[i]->pszFileName;
       delete sounds[i];
    }
  }

  if (primary_buffer)
  {
    primary_buffer->Release();
  }

  if (direct_sound)
  {
    direct_sound->Release();
  }
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
// descriere : initializeaza obiectele direct_sound si primary_buffer
//             copy & paste din IKlowns
// probleme  : raportarea erorilor ?
//---------------------------------------------------------------------------
bool CSoundSystem::PowerOn(void* hWnd)
{
    HRESULT result;
    DSBUFFERDESC   dsbd;       // buffer description struct

    // nu poate exista mai mult de o instanta din aceasta clasa
    soundSystemInstances++;
    if (soundSystemInstances > 1)
    {
      return false;
    }

    // Check if we already have a primary buffer
    if (direct_sound != NULL)
    {
        return true;
    }

    // Create the Direct Sound Object
    if (DirectSoundCreate(NULL,&direct_sound, NULL) != 0)
    {
      return false;
    }

    if(direct_sound->SetCooperativeLevel( static_cast<HWND>(hWnd), DSSCL_EXCLUSIVE ) != DS_OK)
    {
      return false;
    }

    // Set up the primary direct sound buffer.
    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
    dsbd.dwBufferBytes = 0;

    dsbd.lpwfxFormat = NULL;

    if ((result = direct_sound->CreateSoundBuffer(&dsbd, &primary_buffer, NULL)) != 0)
    {
      OutputDebugString(ErrorReport(result));
      return false;
    }

    WAVEFORMATEX waveFormat;

    memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
    waveFormat.wFormatTag         = WAVE_FORMAT_PCM;
    waveFormat.nChannels          = 2; // nu e stereo
    waveFormat.nSamplesPerSec     = SS_SAMPLE_RATE;
    waveFormat.wBitsPerSample     = (WORD)SS_BITSPERSAMPLE;
    waveFormat.nBlockAlign        = (__int16)(waveFormat.wBitsPerSample / 8 * waveFormat.nChannels);
    waveFormat.nAvgBytesPerSec    = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize             = 0;

    if ((result = primary_buffer->SetFormat(&waveFormat)) != DS_OK)
    {
      OutputDebugString(ErrorReport(result));
      return false;
    }

    Enabled = true;
    return true;
}

//---------------------------------------------------------------------------
// descriere : returneaza un string dintr'o eroare
//---------------------------------------------------------------------------
char* CSoundSystem::ErrorReport(int error)
{
  // un switch
  switch(error)
  {
    case DSERR_BADFORMAT        :
      return "bad format";
    case DSERR_INVALIDCALL      :
      return "invalid call";
    case DSERR_INVALIDPARAM     :
      return "invalid param";
    case DSERR_OUTOFMEMORY      :
      return "out of memory";
    case DSERR_PRIOLEVELNEEDED  :
      return "priority level needed";
    case DSERR_UNSUPPORTED      :
      return "unsupported";
    case DSERR_ALLOCATED        :
      return "allocated";
    case DSERR_NOAGGREGATION    :
      return "no aggregation";
    case DSERR_UNINITIALIZED    :
      return "uninitialized";
    default :
      return "error - not implemented yet";
  }
}

//---------------------------------------------------------------------------
// descriere : returneaza capabilitatzile placii de sunet
//---------------------------------------------------------------------------
bool CSoundSystem::GetCaps(void)
{
  HRESULT result;

  result = direct_sound->GetCaps(&ds_caps);
  if(result != DS_OK)
  {
//    errWrite(ErrorReport(result));
    return false;
  }

  return true;
}

//---------------------------------------------------------------------------
// descriere : gaseste un buffer liber - metoda interna
//---------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER CSoundSystem::GetFreeBuffer(int what_sound)
{
  DWORD Status;
  HRESULT result;
  LPDIRECTSOUNDBUFFER Buffer;

  if((sounds[what_sound] == NULL) || (sounds[what_sound]->used == false)) return NULL;

  Buffer = sounds[what_sound]->lpSoundBuffer[sounds[what_sound]->Current];
  if (Buffer)
  {
    result = Buffer->GetStatus(&Status);
    if(result < 0) Status = 0;

    if((Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
    {
      if(sounds[what_sound]->nInstances > 1)
      {
        if (++sounds[what_sound]->Current >= sounds[what_sound]->nInstances) sounds[what_sound]->Current = 0;

        Buffer = sounds[what_sound]->lpSoundBuffer[sounds[what_sound]->Current];
        result = Buffer->GetStatus(&Status);

        if((result >= 0) && (Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
        {
          Buffer->Stop();
          Buffer->SetCurrentPosition(0);
        }
      }
      else
      {
        Buffer = NULL;
      }
    }

    if(Buffer && (Status & DSBSTATUS_BUFFERLOST))
    {
      if((Buffer->Restore() < 0))
      {
        Buffer = NULL;
      }
    }
  }

  return Buffer;
}

/*--------------------------------------------------------------------------
 Description: incarca un sunet nou si returneaza indexul
              retunreaza -1 daca este eroare
--------------------------------------------------------------------------*/
UINT CSoundSystem::LoadNew(const char* filename, int instances)
{
   int i;

   //daca sistemul a fost disabled atunci se iese din metoda
   logWriteLn("CSoundSystem::LoadNew - begin");
   logWriteLn(filename);
   if (Enabled == false)
      return -1;

   for (i=0; i<MaxSoundsHandled; i++)
      if (sounds[i]->used == false)
      {
         if (Load(i, filename, instances))
         {
            logWriteLn("CSoundSystem::LoadNew - new one");
            return i;
         }
         else
         {
            logWriteLn("CSoundSystem::LoadNew - cannot load");
            return -1;
         }
      }
      else
         if (strcmp(filename, sounds[i]->pszFileName) == 0)
         {
            logWriteLn("CSoundSystem::LoadNew - found");
            return i;
         }
   logWriteLn("CSoundSystem::LoadNew - end");
   return -1;
}

//---------------------------------------------------------------------------
// descriere : evident
//---------------------------------------------------------------------------
bool CSoundSystem::Play(int what_sound, int pan_value, TPlayStyle playtype)
{
  HRESULT result;
  LPDIRECTSOUNDBUFFER Buffer;

  //daca sistemul a fost disabled atunci se iese din metoda
  if (!Enabled)
    return true;

  if (sounds[what_sound]->enabled != true)
    return false;

  if(direct_sound == NULL) return false;

  Buffer = GetFreeBuffer(what_sound);
  if(Buffer == NULL) return false;

  int lDevicePan = PanToDeviceValue(VerifyPan(pan_value));
  result = Buffer->SetPan(lDevicePan);

  if(result != DS_OK) return false;

  DWORD dwFlags = 0;

  if (playtype == looped) dwFlags = DSBPLAY_LOOPING;

  result = Buffer->Play(0, 0, dwFlags);
  if(result != DS_OK) return false;

  sounds[what_sound]->playStyle = playtype;
  return true;
}

//---------------------------------------------------------------------------
// descriere : la fel ca cea de sus, dar se cinta cu volum
//---------------------------------------------------------------------------
bool CSoundSystem::PlayWithVol(int what_sound, int pan_value, int vol_value)
{
  if (!Enabled)
  //daca sistemul a fost disabled atunci se iese din metoda
     return true;

  HRESULT result;
  LPDIRECTSOUNDBUFFER Buffer;

  if (sounds[what_sound]->enabled != true)
    return true;

  if(direct_sound == NULL) return false;

  Buffer = GetFreeBuffer(what_sound);
  if(Buffer == NULL)
  {
    logWriteLn("no free buffer");
    return false;
  }

  int lDevicePan = PanToDeviceValue(VerifyPan(pan_value));

  result = Buffer->SetPan(lDevicePan);
  if(result != DS_OK)
  {
    logWriteLn("cannot set pan");
    return false;
  }

  // range check
  int lVerifiedVolume = VerifyVolume(vol_value);

  /*
  if (vol_value < SS_VOL_MIN)
  {
    vol_value = SS_VOL_MIN;
  }
  else
  {
    if (vol_value > SS_VOL_MAX)
    {
      vol_value = SS_VOL_MAX;
    }
  }
  */

  result = Buffer->SetVolume(VolumeToDeviceValue(lVerifiedVolume));
  if(result != DS_OK)
  {
    logWriteLn("cannot set volume");
    return false;
  }

  DWORD dwFlags = 0;

  if (sounds[what_sound]->playStyle == looped) dwFlags = DSBPLAY_LOOPING;

  result = Buffer->Play(0, 0, dwFlags);
  if(result != DS_OK)
  {
    logWriteLn("buffer play failed");
    return false;
  }

  return true;
}

//---------------------------------------------------------------------------
// descriere : la fel ca cea de sus, doar ca-si ia
//             playtype-ul din variabila ei
//---------------------------------------------------------------------------
bool CSoundSystem::Play(int what_sound, int pan_value)
{
  if (!Enabled)
  //daca sistemul a fost disabled atunci se iese din metoda
     return true;

  HRESULT result;
  LPDIRECTSOUNDBUFFER Buffer;

  if (sounds[what_sound]->enabled != true)
    return true;

  if(direct_sound == NULL) return false;

  Buffer = GetFreeBuffer(what_sound);
  if(Buffer == NULL)
  {
    logWriteLn("no free buffer");
    return false;
  }

  int lDevicePan = PanToDeviceValue(VerifyPan(pan_value));

  result = Buffer->SetPan(lDevicePan);
  //result = Buffer->SetPan(10000);
  if(result != DS_OK)
  {
    logWriteLn("cannot set pan");
    return false;
  }

  DWORD dwFlags = 0;

  if (sounds[what_sound]->playStyle == looped) dwFlags = DSBPLAY_LOOPING;

  result = Buffer->Play(0, 0, dwFlags);
  if(result != DS_OK)
  {
    logWriteLn("buffer play failed");
    return false;
  }

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
     return true;

  HRESULT result;

  if(direct_sound == NULL) return false;

  if (sounds[what_sound]->enabled)
  {
    for(int i = 0; i < sounds[what_sound]->nInstances; i++)
    {
      result = sounds[what_sound]->lpSoundBuffer[i]->Stop();
      if(result != DS_OK) return false;

      result = sounds[what_sound]->lpSoundBuffer[i]->SetCurrentPosition(0);
      if(result != DS_OK) return false;
    }
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// descriere : testata de Grab
//---------------------------------------------------------------------------
bool CSoundSystem::SetVolume(int what_sound, LONG vol_value)
{
   if (!Enabled)
   //daca sistemul a fost disabled atunci se iese din metoda
      return true;
   if (sounds[what_sound]->used == false)
      return true;
  HRESULT result;

  // check the volume
  LONG lVerifiedVolume = VerifyVolume(vol_value);

  for(int i = 0; i < sounds[what_sound]->nInstances; i++)
    result = sounds[what_sound]->lpSoundBuffer[i]->SetVolume(VolumeToDeviceValue(lVerifiedVolume));
  if (result != DS_OK) return false;
  sounds[what_sound]->volume = lVerifiedVolume;
  return true;
}

// ghici ce ?
int  CSoundSystem::GetVolume(int _whatSound)
{
   if (!Enabled)
   //daca sistemul a fost disabled atunci se iese din metoda
      return true;
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
      return true;

   sounds[what_sound]->enabled = enabled;
   if (enabled)
      return (SetVolume(what_sound, sounds[what_sound]->volume));
   else
      return Stop(what_sound);
}

//---------------------------------------------------------------------------
// descriere : idem; inca netestata
//---------------------------------------------------------------------------
bool CSoundSystem::SetPlayType(int what_sound, TPlayStyle playStyle)
{
   if (!Enabled)
   //daca sistemul a fost disabled atunci se iese din metoda
      return true;

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
  SAuLoad       auLoad;
  SVocLoad      vocLoad;
  SPSndLoad     pSndLoad;
  TSndInfo      sndInfo;
  LPVOID pData1;
  DWORD  dwData1Size;
  LPVOID pData2;
  DWORD  dwData2Size;
  PCMWAVEFORMAT pcmwf;
  DSBUFFERDESC  dsbdesc;
  HRESULT       result;
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
          logWriteLn("Sound file ");
          logWriteLn(filename);
          logWriteLn(" has no format");
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

  // Set up wave format structure.
  memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
  pcmwf.wf.wFormatTag         = WAVE_FORMAT_PCM;
  pcmwf.wf.nChannels          = SS_CHANNELS;
  pcmwf.wf.nSamplesPerSec     = SS_SAMPLE_RATE;
  pcmwf.wBitsPerSample        = (WORD)SS_BITSPERSAMPLE;
  pcmwf.wf.nBlockAlign        = (__int16)(pcmwf.wBitsPerSample / 8 * pcmwf.wf.nChannels);
  pcmwf.wf.nAvgBytesPerSec    = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;

  // Set up DSBUFFERDESC structure.
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
  dsbdesc.dwSize              = sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags             = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
  dsbdesc.dwBufferBytes       = sndInfo.length;
  dsbdesc.lpwfxFormat         = (LPWAVEFORMATEX)&pcmwf;

  direct_sound->CreateSoundBuffer(&dsbdesc, &sounds[what_sound]->lpSoundBuffer[0], NULL);

  // Lock data in buffer for writing
  result = sounds[what_sound]->lpSoundBuffer[0]->Lock(0, sndInfo.length, &pData1, &dwData1Size, &pData2, &dwData2Size, DSBLOCK_FROMWRITECURSOR);
  sounds[what_sound]->volume = 0;
  if (result != DS_OK)
  {
    return false;
  }

  // citesc din fisier in functie de format
  switch(fileFormat)
  {
    case IS_WAV :
      if (!wavLoad.Load((unsigned char*)pData1, dwData1Size, (unsigned char*)pData2, dwData2Size))
      {
        // Unlock data in buffer
        result = sounds[what_sound]->lpSoundBuffer[0]->Unlock(pData1, dwData1Size, pData2, dwData2Size);
        if (result != DS_OK) return false;
        return false;
      }
      break;

    case IS_AU :
      if (!auLoad.Load((unsigned char*)pData1, dwData1Size, (unsigned char*)pData2, dwData2Size))
      {
        // Unlock data in buffer
        result = sounds[what_sound]->lpSoundBuffer[0]->Unlock(pData1, dwData1Size, pData2, dwData2Size);
        if (result != DS_OK) return false;
        return false;
      }
      break;

    case IS_VOC :
      if (!vocLoad.Load((unsigned char*)pData1, dwData1Size, (unsigned char*)pData2, dwData2Size))
      {
        // Unlock data in buffer
        result = sounds[what_sound]->lpSoundBuffer[0]->Unlock(pData1, dwData1Size, pData2, dwData2Size);
        if (result != DS_OK) return false;
        return false;
      }
      break;

    case IS_PIRON :
      if (!pSndLoad.Load((unsigned char*)pData1, dwData1Size, (unsigned char*)pData2, dwData2Size))
      {
        // Unlock data in buffer
        result = sounds[what_sound]->lpSoundBuffer[0]->Unlock(pData1, dwData1Size, pData2, dwData2Size);
        if (result != DS_OK) return false;
        return false;
      }
      break;

    default :
      break;  
  }

  // Unlock data in buffer
  result = sounds[what_sound]->lpSoundBuffer[0]->Unlock(pData1, dwData1Size, pData2, dwData2Size);
  if (result != DS_OK) return false;
  
  // duplicheaza sound bufferul principal; aici intervine 'instances'
  for(int i = 1; i < instances; i++)
  {
    result = direct_sound->DuplicateSoundBuffer(sounds[what_sound]->lpSoundBuffer[0], &sounds[what_sound]->lpSoundBuffer[i]);
    if(result != DS_OK)
    {
      return false;
    }
  }

  // setez si valorile interne
  sounds[what_sound]->pszFileName = new char[strlen(filename)+1];
  strcpy(sounds[what_sound]->pszFileName, filename);
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

  int lDeviceVolume = VolumeToDeviceValue(mMasterVolume);

  if (primary_buffer->SetVolume(lDeviceVolume) != DS_OK)
  {
    logWriteLn("CSoundSystem::SetPrimaryVolume : cannot set volume");
  }
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
  return SS_VOL_MIN + (int)((SS_VOL_MAX - SS_VOL_MIN) * (_percent / 100.));
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

int CSoundSystem::PanToDeviceValue(int _percent)
{
  return _percent * (SS_PAN_RIGHT / 100); // just scaling as the directx panning values resembles the internal representation
}
//---------------------------------------------------------------------------

int CSoundSystem::VerifyPan(int _percent)
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

