/*++

Copyright (c) 2005-2011  Wisky

Module Name:

    HifiDeviceService.h 

Abstract:
     
	

Environment:
    

Notes:

   Copyright (c) 2005-2011 Wisky.  All Rights Reserved.


Revision History:

  2011-06-09 Created by Fibo Lu

--*/

#ifndef _PARTICLE_MEDIA_SERVICE_H
#define _PARTICLE_MEDIA_SERVICE_H

#include <utils/RefBase.h>
#include <utils/threads.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <utils/Timers.h>
#include <binder/MemoryHeapBase.h>

using namespace android;

//#include "../../mediacommon/MediaPlayerInterface.h"

//namespace particle
{
	class MediaClient : public BBinder
	{
		mutable Mutex mLock;
		int32_t mNextConnId;

	public:
		MediaClient();
		virtual ~MediaClient();

		virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t);

	private:
		void getMediaServiceDataMemory();
		void getMediaService();

	};
};

#endif