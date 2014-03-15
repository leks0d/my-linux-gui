/*++

Copyright (c) 2005-2011  Wisky

Module Name:

    AudioSink.h (Media View Audio Sink Component)

Abstract:
     
	

Environment:
    

Notes:

   Copyright (c) 2005-2011 Wisky.  All Rights Reserved.


Revision History:

  2011-7-28 Created by Fibo Lu

--*/
#ifndef _PARTICLE_MEDIA_PLAYERINTERFACE_H
#define _PARTICLE_MEDIA_PLAYERINTERFACE_H


namespace particle
{
	typedef void(*MediaPlayerEventCallback)(void* calldata, int evnet, int param0, int param1);

	typedef enum 
	{
		MEDIA_PLAYER_ERROR = 1,
		MEDIA_PLAYER_EOF,

	} MediaPlayerEvent ;
	
	typedef enum 
	{
		MEDIA_PLAYER_ERROR_SIGTERM = 100,

	} MediaPlayerEventErrorType ;
	
	typedef struct tagMediaServiceData
	{
		int  mIsPlaying;
		int  mCurrentPosition;
		int  mSpectrumData[32];

	} MediaServiceData;


	class MediaPlayerInterface
	{
	public:
		MediaPlayerInterface();
		virtual ~MediaPlayerInterface();

	public:

		virtual bool setEventCallback(MediaPlayerEventCallback, void*);
		virtual bool setListener(void* listener);

		virtual bool setSource(const char *url);
		virtual bool release();

		virtual int  getDuration();
		virtual int  getCurrentPosition();
		virtual int  seekTo(int millisecond);

		virtual bool prepare();

		virtual bool start();
		virtual bool stop();
		virtual bool isPlaying();
		virtual bool pause();

		virtual bool spectrumEnable(bool enable);
		virtual int* getSpectrumAddress();

		virtual bool audioEqualizerEnable(bool enable);
		virtual bool equalizerSetBandLevel(int bandIndex, int level);

		virtual bool isSpdifCablePlugIn();
		virtual bool enableSpdifOut(bool enable);
		virtual bool isSpdifOut();

		virtual bool getAudioParameter(int* bitsPerSample, int *sampleRate, int *bitRate, int *bitRateMode, int *outSampleRate, int *outBitsPerSample);
		virtual bool setOutMinSampleRate(int sampleRate);

		virtual bool sigTerm();

		virtual bool setNextSongForGapless(const char *url);
		virtual bool gaplessPlay(const char *url);
		virtual bool setGaplessDuration(int millisecond);

		virtual bool selectAudioChunnel(int chunnel);
		virtual int  getAudioChunnel();

		virtual bool setAudioVolume(int volume);
		virtual int  getAudioVolume();

	};

	extern MediaServiceData*   gMediaServiceData;
	extern MediaPlayerInterface*  createMediaPlayer();
};

#endif