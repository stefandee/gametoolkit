/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZResrce.cpp
 * Application :  IBK Open Class Library
 * Purpose     :  Resource locking classes:
 *                ZPrivateResource:
 *                  Resource for serialization within current process.
 *                ZSharedResource:
 *                  Resource for serialization of multiple processes.
 *                ZResourceLock:
 *                  Utility lock class to manage lock/unlock in a block of
 *                  code in conjunction with ZPrivateResource or
 *                  ZSharedResource.
 *                ZResource:
 *                  Virtual class ZPrivateResource and ZSharedResource are
 *                  derived of. Not intended for application use.
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.21  First implementation                        P.Koch, IBK
 * 2001.12.03  Supressed all external links defines        Karg
 *
 * OPEN SOURCE LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to IBK at info@ibk-software.ch.
 */

#ifndef _ZRESRCE_
#define _ZRESRCE_

#include <ZExcept.hpp>

class ZResourceLock;
class ZPrivateResource;
class ZSharedResource;

typedef void* ZResourceHandle;

class ZResource : public ZBase {
   public:
      ZResource();
      virtual ~ZResource();

      ZResource& lock(long timeOut = -1);
      ZResource& unlock();

   protected:
      virtual ZResourceHandle handle() const = 0;

   private:
      ZResource(const ZResource& aResource);
      ZResource& operator=(const ZResource& aResource);

      ZResourceLock* iReslockGate;
      long iRefCount;

      friend class ZResourceLock;
      friend class ZPrivateResource;
      friend class ZSharedResource;
}; // ZResource

class ZResourceLock : public ZBase
{
   public:
      ZResourceLock(ZResource* aResource, long aTimeOut = -1);
      ~ZResourceLock();

   private:
      ZResourceLock(const ZResourceLock& aResourceLock);
      ZResourceLock& operator=(const ZResourceLock& aResourceLock);
      ZResourceLock& setLock(long aTimeOut = -1);
      ZResourceLock& clearLock();

      ZResource* iResource;

      friend class ZResource;
}; // ZResourceLock

class ZPrivateResource : public ZResource
{
   public:
      ZPrivateResource();
      virtual ~ZPrivateResource();

   protected:
      virtual ZResourceHandle handle() const { return iResourceHandle; }

   private:
      ZPrivateResource(const ZPrivateResource& aPrivateResource);
      ZPrivateResource& operator=(const ZPrivateResource& aPrivateResource);

      ZResourceHandle iResourceHandle;

      friend class ZResourceLock;
}; // ZPrivateResource

class ZSharedResource : public ZResource
{
   public:
      ZSharedResource(const ZString& aKeyName);
      virtual ~ZSharedResource();

      ZString keyName() const { return iKeyName; }

   protected:
      virtual ZResourceHandle handle() const { return iResourceHandle; }

   private:
      ZSharedResource(const ZSharedResource& aSharedResource);
      ZSharedResource& operator=(const ZSharedResource& aSharedResource);

      ZResourceHandle iResourceHandle;
      ZString iKeyName;

      friend class ZResourceLock;
}; // ZSharedResource

ZEXCLASSDECLARE(ZResourceLockTimeout, ZException)

#endif // _ZRESRCE_
