/*++

Copyright (c) 2005-2011  Wisky

Module Name:

    HifiDeviceService.cpp 

Abstract:
     
	

Environment:
    

Notes:

   Copyright (c) 2005-2011 Wisky.  All Rights Reserved.


Revision History:

  2011-06-09 Created by Fibo Lu

--*/

#include "MediaClient.h"
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

//#include "../../mediacommon/MediaBinder.h"
//#include "MediaPlayerClient.h"

#define LOGD(msg) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, msg)

//namespace particle
{
	sp<IBinder>			gMediaServiceBinder;
	sp<IMemoryHeap>		gMediaServiceMemHeap;
//	MediaServiceData*	gMediaServiceData = NULL;
//	MediaClient*		gMediaClient = NULL;

	MediaClient::MediaClient()
	{
	//	if (gMediaServiceData == NULL)
		{
	//		getMediaService();
	//		getMediaServiceDataMemory();
		}

	}


	MediaClient::~MediaClient()
	{

	}


	void MediaClient::getMediaServiceDataMemory()
	{
		Parcel data, reply;

		gMediaServiceBinder->transact(MEDIA_GET_SERVICE_DATA_MEMORY, data, &reply);
		gMediaServiceMemHeap = interface_cast<IMemoryHeap> (reply.readStrongBinder());

		unsigned int *base;

		base = (unsigned int *)(gMediaServiceMemHeap->getBase());

		LOGD("EneaBufferClient base=%p \n",base);

		if((base != (unsigned int *)-1) && (base != (unsigned int *)0))
		{
		//	LOGD("EneaBufferClient base=%p Data=0x%x\n",base, *base);
		//	*base = (*base)+1;
		//	LOGD("EneaBufferClient base=%p Data=0x%x CHANGED\n",base, *base);
		}
		else
		{
		//	LOGE("Error shared memory not available\n");
		}

//		gMediaServiceData = (MediaServiceData *)base;
	}


	void MediaClient::getMediaService()
	{
		sp<IServiceManager> sm = defaultServiceManager();

		gMediaServiceBinder = sm->getService(String16("particle.MediaService"));

		if (gMediaServiceBinder.get() == 0) 
		{
	//		LOGE("particle.MediaService not published, waiting...");
		}
	}


	status_t MediaClient::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
	{
		switch(code) 
		{
		case MEDIA_TEST: 
			{
				pid_t pid = data.readInt32();
				int   num = data.readInt32();
				num = num + 3000;

				reply->writeInt32(num);
			} 
			break;

		case MEDIA_PLAYER_EVENT:
			{
				void* context = (void*)data.readInt32();
				int	  eventId = data.readInt32();
				int   param0  = data.readInt32();
				int   param1  = data.readInt32();

				//LOGI(" MediaClient::onTransact MEDIA_PLAYER_EVENT context=0x%x eventId = %d", (unsigned int)context, eventId);

		//		MediaPlayerClient::playerCallback(NULL, context, eventId, param0, param1);
			}
			break;

		default:
			return BBinder::onTransact(code, data, reply, flags);
		}

		return NO_ERROR;
	}


	void createMediaClient()
	{
		if (gMediaClient == NULL)
		{
			gMediaClient = new MediaClient;

			Parcel data, reply;
			data.writeStrongBinder(gMediaClient);
			gMediaServiceBinder->transact(MEDIA_CLIENT, data, &reply);

			ProcessState::self()->startThreadPool();
		}
	}
}
