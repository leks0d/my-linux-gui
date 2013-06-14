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
#ifndef _PARTICLE_MEDIA_PLAYER_SERVICE_H
#define _PARTICLE_MEDIA_PLAYER_SERVICE_H

namespace particle
{

	class MediaPlayerService : public MediaPlayerInterface
	{
	public:
		MediaPlayerService();
		virtual ~MediaPlayerService();


	public: //interface

		bool setEventCallback(MediaPlayerEventCallback, void*);

		bool setSource(const char *url);
		bool release();

		int  getDuration();
		int  getCurrentPosition();
		int  seekTo(int millisecond);

		bool prepare();

		bool start();
		bool stop();
		bool isPlaying();
		bool pause();

		bool spectrumEnable(bool enable);
		int* getSpectrumAddress();

		bool audioEqualizerEnable(bool enable);
		bool equalizerSetBandLevel(int bandIndex, int level);

		bool isSpdifCablePlugIn();
		bool enableSpdifOut(bool enable);
		bool isSpdifOut();

		bool getAudioParameter(int* bitsPerSample, int *sampleRate, int *bitRate, int *bitRateMode, int *outSampleRate, int *outBitsPerSample);
		bool setOutMinSampleRate(int sampleRate);

		bool sigTerm();

		bool setNextSongForGapless(const char *url);
		bool gaplessPlay(const char *url);
		bool setGaplessDuration(int millisecond);

		bool selectAudioChunnel(int chunnel);
		int  getAudioChunnel();

	public:
		static void playerCallback(XAPlayItf caller, void * pContext, XAuint32 eventId);
		void playerCallbackProc(XAPlayItf caller, void * pContext, XAuint32 eventId);
		void callbackFunction(XAPlayItf caller, void * pContext, MediaPlayerEvent eventId, int param0, int param1);

	private:

		bool	mediaInit(const char * filename);
		bool    mediaDeinit();

		volatile bool		mEOF;
		bool				mPlaying;

		omxal::EngineInterface*	mEngine; 
		omxal::PlayerInterface*	mPlayer;
		omxal::ObjectInterface*	mOutputMix;
		omxal::SeekInterface*	mSeek;
		XAEqualizerItf			mEqualizerItf;

		int						mEof;
		int						mIsImage;

		char					mUri[260 * 3];

		MediaPlayerEventCallback mCallbackFunction;
		void*				mCallbackData;

		bool				mSpectrumEnable;
		bool				mAudioEqEnable;
		#define EQUALIZER_BANDS_NUMBER  8
		int					mEqualizerBandLevel[EQUALIZER_BANDS_NUMBER];

		bool				mSpdifOut;
		int					mOutMinSampleRate;
		int					mGaplessDuration;
		int					mAudioChunnel;
		particle::Mutex     mMutex;
	};
};

#endif