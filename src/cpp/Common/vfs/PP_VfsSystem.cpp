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

#pragma hdrstop

#include "PP_VfsSystem.h"
#include "physfs.h"
#include "PP_String.h"
#include "basepath.h"
#include "tinyxml.h"
#include "Paths.h"
#include "Stream_PhysFS.h"
#include "easylogging++.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CVfsSystem* CVfsSystem::mInstance = 0;

CVfsSystem* GetVfsInstance()
{
    return CVfsSystem::Instance();
}
//---------------------------------------------------------------------------

CVfsSystem::CVfsSystem(void)
{
    Init();
}
//---------------------------------------------------------------------------

CVfsSystem::~CVfsSystem()
{
    // TODO: test if deinit fails, but should we care? :)
    PHYSFS_deinit();
}
//---------------------------------------------------------------------------

CVfsSystem* CVfsSystem::Instance()
{
    if (!mInstance)
    {
        mInstance = new CVfsSystem();
    }

    return mInstance;
}
//---------------------------------------------------------------------------

// Description : initializes the physics fs
// Param       :
// Result      :
// Comments    : break this method into several smaller methods
void CVfsSystem::Init()
{
    // initialize the physicsfs library
    mInitResult = PHYSFS_init(0);

    mBaseDir = CPString(PHYSFS_getBaseDir());

    //
    // read the xml config file to mount as appropriate the application paths
    //
    TiXmlDocument xmlDoc = TiXmlDocument(GetBaseDir() + "PPEngine_Vfs.xml");

    xmlDoc.LoadFile();

    TiXmlElement* root = xmlDoc.RootElement();

    if (!root)
    {
        return;
    }

    TiXmlElement* configElem = root->FirstChildElement("config");

    if (configElem != NULL && configElem->Attribute("writeDir") != NULL && CPString(configElem->Attribute("writeDir")) == CPString("platform"))
    {
        PHYSFS_setSaneConfig("pirongames", "pptactical", NULL, 0, 0);
    }
    else
    {
        CPString outPath = mBaseDir + PATH_OUT;

        // despite the documentation, PHYSFS_setWriteDir doesn't seem to be able to create the dir
        PHYSFS_setWriteDir(mBaseDir.c_str());
        PHYSFS_mkdir(PATH_OUT.c_str());
        PHYSFS_setWriteDir(outPath.c_str());

        // automatically mount "out" dir
        PHYSFS_mount(outPath.c_str(), PATH_OUT.c_str(), 0);
    }

    TiXmlElement* elem = root->FirstChildElement("resources")->FirstChildElement("resource");

    while(elem)
    {
        PHYSFS_mount(mBaseDir + elem->Attribute("path"), elem->Attribute("mount"), 0);
        elem = root->NextSiblingElement("resource");
    }

    mWriteDir = CPString(PHYSFS_getWriteDir());
}
//---------------------------------------------------------------------------

void CVfsSystem::LogInfo()
{
    LOG(INFO) << "PhysicsFS Info Start";

    if (!mInitResult)
    {
        LOG(ERROR) << "vfs init failed: " << PHYSFS_getLastError();
    }

    LOG(INFO) << "PhysicsFS base dir " << mBaseDir.c_str();
    LOG(INFO) << "PhysicsFS write dir " << mWriteDir.c_str();

    // log some info
    PHYSFS_Version compiled;
    PHYSFS_Version linked;

    PHYSFS_VERSION(&compiled);
    PHYSFS_getLinkedVersion(&linked);

    VLOG(9) <<
            CPString("physfs version is ") +
            CPString(VFS_VERSION_MAJOR) +
            CPString(VFS_VERSION_MINOR) +
            CPString(VFS_VERSION_PATCH);

    VLOG(9) <<
            CPString("physfs compiled version is ") +
            CPString(compiled.major) + "." +
            CPString(compiled.minor) + "." +
            CPString(compiled.patch);

    VLOG(9) <<
            CPString("physfs linked version is ") +
            CPString(linked.major) + "." +
            CPString(linked.minor) + "." +
            CPString(linked.patch);

    // log the available plugins
    const PHYSFS_ArchiveInfo **rc = PHYSFS_supportedArchiveTypes();
    const PHYSFS_ArchiveInfo **i;

    LOG(INFO) << "Supported archive types:";
    if (*rc == NULL)
        LOG(INFO) << " * Apparently, NONE!";
    else
    {
        for (i = rc; *i != NULL; i++)
        {
            LOG(INFO) << (CPString("* ") + CPString((*i)->extension) + ": " + CPString((*i)->description));
            LOG(INFO) << (CPString("Written by ") + CPString((*i)->author) + ": " + CPString((*i)->url));
        } /* for */
    } /* else */

    // log the search paths
    char **iii;

    LOG(INFO) << "Search path:";

    for (iii = PHYSFS_getSearchPath(); *iii != NULL; iii++)
    {
        LOG(INFO) << *iii;
    }

    //PHYSFS_freeList(iii);

    LOG(INFO) << "PhysicsFS Info End";
}
//---------------------------------------------------------------------------

const CPString& CVfsSystem::GetWriteDir()
{
    return mWriteDir;
}
//---------------------------------------------------------------------------

const CPString& CVfsSystem::GetBaseDir()
{
    return mBaseDir;
}
//---------------------------------------------------------------------------

bool CVfsSystem::EnsureWriteDir(CPString dir)
{
    bool result = (PHYSFS_mkdir(dir.c_str()) != 0);

    if (!result)
    {
        LOG(ERROR) << "Failed to create write dir folder " << dir.c_str() << ", error " << PHYSFS_getLastError();
    }

    return result;
}
//---------------------------------------------------------------------------

bool CVfsSystem::Delete(CPString fileName)
{
    bool result = (PHYSFS_delete(fileName.c_str()) != 0);

    if (!result)
    {
        LOG(ERROR) << "Failed to delete write dir folder " << fileName.c_str() << ", error " << PHYSFS_getLastError();
    }

    return result;
}
//---------------------------------------------------------------------------

bool CVfsSystem::FileExists(CPString fileName)
{
    CPString fixedName = PP::Stream_PhysFS::FixPath2(fileName);

    return PHYSFS_exists(fixedName.c_str()) != 0 && PHYSFS_isDirectory(fixedName.c_str()) == 0;
}
//---------------------------------------------------------------------------

bool CVfsSystem::Exists(CPString fileName)
{
    CPString fixedName = PP::Stream_PhysFS::FixPath2(fileName);

    return PHYSFS_exists(fixedName.c_str()) != 0;
}
