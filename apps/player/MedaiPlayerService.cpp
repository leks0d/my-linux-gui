/*++

Copyright (c) 2005-2011  Wisky

Module Name:

    MediaPlayerService.cpp

Abstract:
     
	

Environment:
    

Notes:

   Copyright (c) 2005-2011 Wisky.  All Rights Reserved.


Revision History:

  2011-7-28 Created by Fibo Lu

--*/

#include "../../../solutions/mediabrowser/jni/omxal/Omxal.h"
#include "MediaPlayerInterface.h"
#include "MediaPlayerService.h"

#define LOG_TAG "ParticleMedia"
#include <android/log.h>  
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)  
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)  
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)  
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)  

#define MAX_NUMBER_INTERFACES 5

namespace particle
{
	extern MediaServiceData* gMediaServiceData;

	MediaPlayerService::MediaPlayerService()
	{
		mCallbackFunction = NULL;

		mEngine    = NULL; 
		mPlayer    = NULL;
		mOutputMix = NULL;
		mSeek	   = NULL;

		mSpectrumEnable = false;
		mAudioEqEnable = false;
		memset(mEqualizerBandLevel, 0, sizeof(mEqualizerBandLevel));

		mSpdifOut = false;
		mOutMinSampleRate = 0;
		mGaplessDuration = 0;

		mAudioChunnel = 0;
	}

	MediaPlayerService::~MediaPlayerService()
	{

	}


	bool MediaPlayerService::setEventCallback(MediaPlayerEventCallback function, void* callbackData)
	{
		mCallbackFunction = function;
		mCallbackData = callbackData;
		return true;
	}



	void MediaPlayerService::playerCallback(XAPlayItf caller, void * pContext, XAuint32 eventId)
	{
		MediaPlayerService* player = (MediaPlayerService*)pContext;
		player->playerCallbackProc(caller, pContext, eventId);
	}
	

	void MediaPlayerService::callbackFunction(XAPlayItf caller, void * pContext, MediaPlayerEvent eventId, int param0, int param1)
	{
		if (mCallbackFunction)
		{
			LOGI("MediaPlayerService::callbackFunction eventId = %d", eventId);
			mCallbackFunction(mCallbackData, eventId, param0, param1);
		}
	//	LOGI("MediaPlayerService::callbackFunction 2222");
	}


	void MediaPlayerService::playerCallbackProc(XAPlayItf caller, void * pContext, XAuint32 eventId)
	{
		switch (eventId)
		{
		case XA_PLAYEVENT_HEADATEND :
			LOGI("playback2222 End");
			callbackFunction(caller, pContext, MEDIA_PLAYER_EOF, 0, 0);
			break;

		case XA_PLAYEVENT_HEADATMARKER :
			LOGI("playback hit the marker.");
			break;

		case XA_PLAYEVENT_HEADATNEWPOS :
			LOGI("playback at new position.");
			break;

		case XA_PLAYEVENT_HEADMOVING :
			LOGI("playback is active.");
			break;

		case XA_PLAYEVENT_HEADSTALLED :
			LOGI("playback stalled.");
			break;

		default :
			LOGI("bogus callback event : %d", (int)eventId);
			break;
		}
	}


	bool MediaPlayerService::setSource(const char *url)
	{
		if (strlen(url) >= 260 * 3)
			return false;

		strcpy (mUri, url);
		return true;
	}


	bool MediaPlayerService::release()
	{
		bool success;
		mMutex.lock();

		success = mediaDeinit();

		mMutex.unlock();
		return success;
	}


	int MediaPlayerService::getDuration()
	{
		XAmillisecond millisecond = -1;
		mMutex.lock();

		if (mPlayer)
			mPlayer->getDuration(&millisecond);
	
		mMutex.unlock();
		return millisecond;
	}


	int MediaPlayerService::getCurrentPosition()
	{
		XAmillisecond millisecond = -1;
		mMutex.lock();

	//	if (mPlayer)
	//		mPlayer->getPosition(&millisecond);

		millisecond = gMediaServiceData->mCurrentPosition;
		mMutex.unlock();
		return millisecond;
	}


	int MediaPlayerService::seekTo(int millisecond)
	{
		int pos = -1;
		mMutex.lock();

		if (mSeek)
			pos = mSeek->setPosition(millisecond, 0);

		mMutex.unlock();
		return pos==0;
	}


	bool MediaPlayerService::prepare()
	{
		bool success;
		mMutex.lock();

		if (mPlayer)
		{
			XAuint32 state;
			mPlayer->getPlayState(&state);

			if (state == XA_PLAYSTATE_PLAYING)
			{
				mPlayer->setPlayState(XA_PLAYSTATE_PAUSED);
		//		particle::Thread::sleep(200);
			}
		}

		mediaDeinit();
		success = mediaInit(mUri);

		mMutex.unlock();
		return success;
	}

	bool  MediaPlayerService::start()
	{
		bool success = false;
		mMutex.lock();

		if (mPlayer)
		{
			if (mPlayer->setPlayState(XA_PLAYSTATE_PLAYING) == XA_RESULT_SUCCESS)
				success = true;
		}

		mMutex.unlock();
		return success;
	}

	bool  MediaPlayerService::stop()
	{
		bool success;

		mMutex.lock();
		success = mediaDeinit();
		mMutex.unlock();

		return success;
	}


	bool  MediaPlayerService::pause()
	{
		bool success = false;
		mMutex.lock();

		if (mPlayer)
		{
			XAuint32 state;

			mPlayer->getPlayState(&state);

			if (state == XA_PLAYSTATE_PLAYING)
				mPlayer->setPlayState(XA_PLAYSTATE_PAUSED);

			success = true;
		}
		mMutex.unlock();

		return success;
	}

	bool  MediaPlayerService::isPlaying()
	{
		bool success = false;
		mMutex.lock();

		if (mPlayer)
		{
			XAuint32 state;
			mPlayer->getPlayState(&state);
			if (state == XA_PLAYSTATE_PLAYING)
				success = true;
		}

		mMutex.unlock();
		return success;
	}


	bool MediaPlayerService::spectrumEnable(bool enable)
	{
		bool success = false;
		mMutex.lock();

		mSpectrumEnable = enable;

		if (mPlayer)
		{
			if (mPlayer->spectrumEnable(enable)  == XA_RESULT_SUCCESS)
				success = true;
		}

		mMutex.unlock();
		return success;
	}


	int* MediaPlayerService::getSpectrumAddress()
	{
		int* address = NULL;
		mMutex.lock();

		if (mPlayer)
		{
			address = (int*)(mPlayer->getSpectrumAddress());
		}

		mMutex.unlock();
		return address;
	}


	bool MediaPlayerService::audioEqualizerEnable(bool enable)
	{
		bool success = false;
		mMutex.lock();

		mAudioEqEnable = enable;

		if (mPlayer)
		{
			if (mPlayer->audioEqualizerEnable(enable)  == XA_RESULT_SUCCESS)
				success = true;
		}

		mMutex.unlock();
		return success;
	}


	bool MediaPlayerService::equalizerSetBandLevel(int bandIndex, int level)
	{
		bool success = false;

		//LOGE("Fibo*EQ MediaPlayerService::equalizerSetBandLevel");

		mMutex.lock();

		if (bandIndex >= 0 && bandIndex < EQUALIZER_BANDS_NUMBER)
			mEqualizerBandLevel[bandIndex] =  level;

		if (mPlayer)
		{
			if (mPlayer->equalizerSetBandLevel(bandIndex, level) == XA_RESULT_SUCCESS)
				success = true;
		}

		mMutex.unlock();
		return success;
	}

	bool MediaPlayerService::isSpdifCablePlugIn()
	{
		FILE* file;

		file = fopen("/sys/class/switch/spdif_cable/state", "r");
		if (file == NULL)
		{
			LOGE("can't open /sys/class/switch/spdif_cable/state");
			return false;
		}

		int plugin;
		char buffer[32];

		fread(buffer, 1, 32, file);

		LOGI ("SpdifCablePlugIn %d", (int)buffer[0]);
		fclose(file);

		if (buffer[0] == '1')
			return true;
		else
			return false;
	}


	//fibo extern "C" int  hifi_enable_spdif(int enable);

	bool MediaPlayerService::enableSpdifOut(bool enable)
	{
		mSpdifOut = enable;

	//fibo	hifi_enable_spdif(enable ? 1 : 0);
		return true;
	}

	bool MediaPlayerService::isSpdifOut()
	{
		return mSpdifOut;
	}


	bool MediaPlayerService::getAudioParameter(int* bitsPerSample, int *sampleRate, int *bitRate, int *bitRateMode, int *outSampleRate, int *outBitsPerSample)
	{
		bool success = false;
		mMutex.lock();

		if (mPlayer)
		{
			if (mPlayer->getAudioParameter(bitsPerSample, sampleRate, bitRate, bitRateMode, outSampleRate, outBitsPerSample) == XA_RESULT_SUCCESS)
				success = true;
		}

		mMutex.unlock();
		return success;
	}


	bool MediaPlayerService::setOutMinSampleRate(int sampleRate)
	{
		bool success = false;
		mMutex.lock();

		mOutMinSampleRate = sampleRate;

		if (mPlayer)
		{
			if (mPlayer->setOutMinSampleRate(sampleRate)  == XA_RESULT_SUCCESS)
				success = true;
		}

		mMutex.unlock();
		return true;
	}


	bool MediaPlayerService::sigTerm()
	{
		stop();
		callbackFunction(NULL, NULL, MEDIA_PLAYER_ERROR, MEDIA_PLAYER_ERROR_SIGTERM, 0);
		LOGI ("sigTerm");

		return true;
	}

	bool MediaPlayerService::setNextSongForGapless(const char *url)
	{
		bool ret = false;
		mMutex.lock();

		if (mPlayer)
		{
			if (mPlayer->setNextSongForGapless(url) == XA_RESULT_SUCCESS)
				ret = true;
		}

		mMutex.unlock();

		LOGI ("MediaPlayerService::setNextSongForGapless ret = %d", ret);
		return ret;
	}


	bool MediaPlayerService::gaplessPlay(const char *url)
	{
		bool ret = false;
		mMutex.lock();

		if (mPlayer)
		{
			if (mPlayer->gaplessPlay(url) == XA_RESULT_SUCCESS)
				ret = true;
		}
	
		mMutex.unlock();
		return ret;
	}


	bool MediaPlayerService::setGaplessDuration(int millisecond)
	{
		bool ret = false;
		mMutex.lock();

		if (millisecond < 0) 
			mGaplessDuration = 0;
		else if (millisecond > 10 * 1000)
			mGaplessDuration = millisecond;
		else
			mGaplessDuration = millisecond;

		if (mPlayer)
		{
			if (mPlayer->setGaplessDuration(mGaplessDuration) == XA_RESULT_SUCCESS)
				ret = true;
		}

		mMutex.unlock();
		return ret;
	}


	bool MediaPlayerService::selectAudioChunnel(int chunnel)
	{
		bool ret = false;
		mMutex.lock();

		if (chunnel == 0 || chunnel == 1)
		{
			ret = true;
			if ((mAudioChunnel != chunnel) && mPlayer)
			{
				if (mPlayer->selectAudioChunnel(chunnel) != XA_RESULT_SUCCESS)
					ret = false;;
			}

			mAudioChunnel = chunnel;
		}

		mMutex.unlock();
		return ret;
	}

	int  MediaPlayerService::getAudioChunnel()
	{
		return mAudioChunnel;
	}


	bool MediaPlayerService::mediaInit(const char * filename)
	{
		XADataSource		audioSource;
		XADataLocator_URI	uri;
		XADataFormat_MIME	mime;
		XADataSink			audioSink;
		XADataLocator_OutputMix locatorOutputMix;
		XAresult			res = XA_RESULT_SUCCESS;
		int i;

		XAboolean		required[MAX_NUMBER_INTERFACES];
		XAInterfaceID	iidArray[MAX_NUMBER_INTERFACES];

		XAEngineOption EngineOption[] = 
		{
			(XAuint32) XA_ENGINEOPTION_THREADSAFE,
			(XAuint32) XA_BOOLEAN_TRUE
		};

		XAchar uri_path[1024];
		XAInterfaceID iid;

		particle::Thread::setCurrentName("MediaPlayer Main");
		sprintf(uri_path, "%s", filename);

		//particle::addLogRule(LOG_ALL, NULL, NULL);
		gMediaServiceData->mIsPlaying = false;
		gMediaServiceData->mCurrentPosition = 0;

		do
		{
			LOGV("creating engine.");
			res = omxal::createEngine((XAObjectItf *)&mEngine, 1, EngineOption, 0, NULL, NULL);
			if (res != XA_RESULT_SUCCESS)
			{
				break;
			}

			LOGV("Realizing engine.");
			res = mEngine->realize(XA_BOOLEAN_FALSE);
			if (res != XA_RESULT_SUCCESS)
			{
				break;
			}


			// Initialize arrays required[] and iidArray[] */
			for (i = 0; i < MAX_NUMBER_INTERFACES; i++)
			{
				required[i] = XA_BOOLEAN_FALSE;
				iidArray[i] = XA_IID_NULL;
			}
			required[0] =  XA_BOOLEAN_TRUE;
			iidArray[0] =  XA_IID_VOLUME;

			/* Setup the data source structure */
			uri.locatorType = XA_DATALOCATOR_URI;
			uri.pURI		= uri_path;

			mime.formatType = XA_DATAFORMAT_MIME;
			mime.pMimeType  = (XAchar *)"audio/ogg";
			mime.containerType   = XA_CONTAINERTYPE_WAV;
			audioSource.pLocator = (void*)&uri;
			audioSource.pFormat  = (void*)&mime;

			/* Setup the data sink structure */
			locatorOutputMix.locatorType = XA_DATALOCATOR_OUTPUTMIX;
			locatorOutputMix.outputMix   = (XAObjectItf)mOutputMix;
			audioSink.pLocator = (void*)&locatorOutputMix;
			audioSink.pFormat  = NULL;

			/* Set arrays required[] and iidArray[] for no interfaces (PlayItf
			* is implicit) */
			required[0] = XA_BOOLEAN_FALSE;
			iidArray[0] = XA_IID_NULL;
			required[1] = XA_BOOLEAN_FALSE;
			iidArray[1] = XA_IID_NULL;

			/* Create the music player */
			LOGV("creating media player.");
			res = mEngine->createMediaPlayer((XAObjectItf *)&mPlayer,
				&audioSource, NULL,
				&audioSink, NULL, NULL, NULL,
				0, iidArray, required);

			if (res != XA_RESULT_SUCCESS)
				break;

			mPlayer->spectrumEnable(mSpectrumEnable);
			if (gMediaServiceData)
				mPlayer->setSpectrumAddress(gMediaServiceData->mSpectrumData);

			mPlayer->audioEqualizerEnable(mAudioEqEnable);

			for (i = 0; i < EQUALIZER_BANDS_NUMBER; i++)
				mPlayer->equalizerSetBandLevel(i, mEqualizerBandLevel[i]);

			if (mOutMinSampleRate > 0)
				mPlayer->setOutMinSampleRate(mOutMinSampleRate);

			if (mGaplessDuration > 0){
				mPlayer->setGaplessDuration(mGaplessDuration);
				log_i("mPlayer->setGaplessDuration(%d)",mGaplessDuration);
			}

			if (mAudioChunnel != 0)
				mPlayer->selectAudioChunnel(mAudioChunnel);
			

			mPlayer->setIsPlayingAddress(&gMediaServiceData->mIsPlaying);
			mPlayer->setCurrentPositionAddress(&gMediaServiceData->mCurrentPosition);

			LOGV("realizing player.");
			res = mPlayer->realize(XA_BOOLEAN_FALSE);
			if (res != XA_RESULT_SUCCESS)
				break;

			LOGV("Register callback.");
			res = mPlayer->registerCallback(MediaPlayerService::playerCallback, this);
			if (res != XA_RESULT_SUCCESS)
				break;

			LOGV("Set callback event mask.");
			res = mPlayer->setCallbackEventsMask(XA_PLAYEVENT_HEADATEND);
			if (res != XA_RESULT_SUCCESS)
				break;

			LOGV("Get seek interface.");
			iid = (XAInterfaceID)&(XA_IID_SEEK);

		//	i = XA_IID_SEEK->time_low;
			res = mPlayer->getInterface(XA_IID_SEEK, (void**)&mSeek);
			if (res != XA_RESULT_SUCCESS)
				break;


		}while(0);


		if (res != XA_RESULT_SUCCESS)
		{
			mediaDeinit();
			LOGE("Can't decode file");
			return false;
		}
		else
			return true;
	}


	bool MediaPlayerService::mediaDeinit()
	{
		LOGI("MediaPlayerService::mediaDeinit Enter");

		if (mPlayer)
		{
			mPlayer->setPlayState(XA_PLAYSTATE_STOPPED);
			delete mPlayer;
			mPlayer = NULL;
		}

		if (mEngine)
		{
			delete mEngine;
			mEngine = NULL;
		}

		gMediaServiceData->mCurrentPosition = 0;
		gMediaServiceData->mIsPlaying		= false;

		LOGI("MediaPlayerService::mediaDeinit Leave");
		return true;
	}

	MediaServiceData*   gMediaServiceData = NULL;

	MediaPlayerInterface*  createMediaPlayer()
	{
		if (gMediaServiceData == NULL)
			gMediaServiceData = (MediaServiceData *)malloc(sizeof (MediaServiceData));

		MediaPlayerService* player = new MediaPlayerService();

		return (MediaPlayerInterface*)player;
	}

};
