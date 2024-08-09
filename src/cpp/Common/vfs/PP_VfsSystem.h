//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Virtual File System
//
//  Version           : 1.0
//
//  Description       :
//    * access to the virtual file system through a singleton
//    * not the best way to handle this, still to be tested
//
//  History           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef PP_VfsSystemH
#define PP_VfsSystemH
//---------------------------------------------------------------------------

const int VFS_VERSION_MAJOR = 1;
const int VFS_VERSION_MINOR = 1;
const int VFS_VERSION_PATCH = 0;

#include "PP_String.h"

class CVfsSystem
{
  public:
    static CVfsSystem* Instance();
    virtual ~CVfsSystem();

  public:
    void LogInfo();
    const CPString& GetWriteDir();
    const CPString& GetBaseDir();

    bool EnsureWriteDir(CPString dir);
    bool Delete(CPString fileName);
    bool Exists(CPString fileName);
    bool FileExists(CPString fileName);

  protected:
    CVfsSystem(void);
    CVfsSystem(const CVfsSystem&);
    CVfsSystem& operator=(const CVfsSystem&);

  private:
    void Init();

    static CVfsSystem* mInstance;

    CPString mWriteDir;
    CPString mBaseDir;

    int mInitResult;
};

//typedef CVfsSystem CPVfsSystem;

CVfsSystem* GetVfsInstance();

inline CVfsSystem* Vfs()
{
    return CVfsSystem::Instance();
}

#endif
