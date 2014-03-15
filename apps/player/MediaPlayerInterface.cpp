/*++

Copyright (c) 2005-2011  Wisky

Module Name:

    MediaPlayerInterface.cpp

Abstract:
     
	

Environment:
    

Notes:

   Copyright (c) 2005-2011 Wisky.  All Rights Reserved.


Revision History:

  2011-7-28 Created by Fibo Lu

--*/

#include "MediaPlayerInterface.h"

namespace particle
{
	MediaPlayerInterface::MediaPlayerInterface()
	{

	}

	MediaPlayerInterface::~MediaPlayerInterface()
	{

	}


	bool MediaPlayerInterface::setEventCallback(MediaPlayerEventCallback, void*)
	{
		return false;
	}

	bool MediaPlayerInterface::setListener(void* listener)
	{
		return false;
	}


	bool MediaPlayerInterface::setSource(const char *url)
	{
		return false;
	}


	bool MediaPlayerInterface::release()
	{
		return false;
	}


	int MediaPlayerInterface::getDuration()
	{
		return 0;

	}


	int MediaPlayerInterface::getCurrentPosition()
	{
		return 0;
	}


	int MediaPlayerInterface::seekTo(int millisecond)
	{
		return 0;
	}

	bool MediaPlayerInterface::prepare()
	{
		return false;
	}

	bool  MediaPlayerInterface::start()
	{
		return false;
	}

	bool  MediaPlayerInterface::stop()
	{
		return false;
	}


	bool  MediaPlayerInterface::pause()
	{
		return false;

	}

	bool  MediaPlayerInterface::isPlaying()
	{
		return false;
	}


	bool MediaPlayerInterface::spectrumEnable(bool enable)
	{
		return false;
	}


	int* MediaPlayerInterface::getSpectrumAddress()
	{
		return 0;
	}

	bool MediaPlayerInterface::audioEqualizerEnable(bool enable)
	{
		return false;
	}
	
	bool MediaPlayerInterface::equalizerSetBandLevel(int bandIndex, int level)
	{
		return false;
	}

	bool MediaPlayerInterface::isSpdifCablePlugIn()
	{
		return false;
	}

	bool MediaPlayerInterface::enableSpdifOut(bool enable)
	{
		return false;
	}

	bool MediaPlayerInterface::isSpdifOut()
	{
		return false;
	}

	bool MediaPlayerInterface::getAudioParameter(int* bitsPerSample, int *sampleRate, int *bitRate, int *bitRateMode, int *outSampleRate, int *outBitsPerSample)
	{
		return false;
	}

	bool MediaPlayerInterface::setOutMinSampleRate(int sampleRate)
	{
		return false;
	}

	bool MediaPlayerInterface::sigTerm()
	{
		return false;
	}

	bool MediaPlayerInterface::setNextSongForGapless(const char *url)
	{
		return false;
	}

	bool MediaPlayerInterface::gaplessPlay(const char *url)
	{
		return false;
	}

	bool MediaPlayerInterface::setGaplessDuration(int millisecond)
	{
		return false;
	}

	bool MediaPlayerInterface::selectAudioChunnel(int chunnel)
	{
		return false;
	}

	int  MediaPlayerInterface::getAudioChunnel()
	{
		return -1;
	}

	bool MediaPlayerInterface::setAudioVolume(int volume)
	{
		return 0;
	}

	int  MediaPlayerInterface::getAudioVolume()
	{
		return -1;
	}



};
