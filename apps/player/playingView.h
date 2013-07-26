namespace mango
{
	class PlayingView: public View
	{
	public:
		PlayingView(void);
		PlayingView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		void ViewInit(void);
		static unsigned int SeekBarRunnig(void *parameter);
		void updatePlayMode();
		void updatePlayButtonIcon();
		void updateAudioInfo();
		virtual ~PlayingView(void);
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onPaint(Canvas& canvas);
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 
		virtual int onCommand(int id, int code, View* fromView);
	private:
		mediainfo mCurrentInfo;
		Button*  mNextButton;
		Button*  mPrevButton;
		Button*  mPlayButton;
		Button*  mPlayModeButton;
		ImageView* mAlbumImage;
		
		Button*  mVolumeButton;
		ImageView* mBatteryIcon;
		TextView* mBatteryText;
		TextView* mVolumeText;
		TextView* mTimeText;
		TextView* mDurtionText;
		TextView* mMyMusicText;
		TextView* mSettingText;
		
		TextView* mAudioInfo;
		TextView* mMusicName;
		TextView* mArtist;
		TextView* mAlbum;
		
		SeekBar* mSeekBar;
		
		Thread mSeekBarUpdateThread;
		int isNeedFresh;
		int mVolume;
		int mBattery;
	};
}