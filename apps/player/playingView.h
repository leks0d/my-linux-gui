#include "SkStream.h"
#include "SkImageDecoder.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkDevice.h"
#include "SkPaint.h"
#include "SkRect.h"

namespace mango
{
class KeyCount{
		int keycode;
		int action;
		int press;
		int count;
		int enter;
		Mutex	mMutex;
		public:
			KeyCount(){
				keycode = 0;
				press = 0;
				count = 0;
				enter = 0;
			}
		int initKeyPress(int code){
			mMutex.lock();
			
			keycode = code;
			press = 1;
			count = 0;
			enter = 0;
			
			mMutex.unlock();
		}
		int TriggerKey(){
			mMutex.lock();
			
			if(press){
				count++;
				if(count>1){
					enter = 1;
					switch(keycode){
						case KEYCODE_NEXT:
							mPlayinglist->fastForward();
							break;
						case KEYCODE_PREV:
							mPlayinglist->fastRewind();
							break;			
					}
				}
			}
			
			mMutex.unlock();
			//log_i("press=%d,count=%d,enter=%d",press,count,enter);
		}
		int isKeyPress(int code){
			int ret = 0;
			
			mMutex.lock();
			
			if(code == keycode && enter){
				ret = 1;
			}else
				ret = 0;
			
			keyRelease();
			
			mMutex.unlock();
			
			log_i("isKeyPress keycode=%d,press=%d",keycode,press);
			
			return ret;
		}
		int keyRelease(){
			press = 0;
			count = 0;
			keycode = 0;
			enter = 0;
		}
	};
	class PlayingView: public View
	{
	public:
		PlayingView(void);
		PlayingView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		void ViewInit(void);
		static unsigned int SeekBarRunnig(void *parameter);
		static unsigned int shutdownRunnig(void *parameter);
		void updatePlayMode();
		void updatePlayButtonIcon();
		void updateAudioInfo();
		void updateEqState();
		static void CalculateSize(float srcw,float srch,float dstw,float dsth,SkRect &rect);
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
		Button*  mAddToPlaylistButton;
		ImageView* mAlbumImage;
		
		Button*  mVolumeButton;
		ImageView* mBatteryIcon;
		TextView* mBatteryText;
		TextView* mVolumeText;
		TextView* mTimeText;
		TextView* mDurtionText;
		TextView* mMyMusicText;
		TextView* mSettingText;
		TextView* mEqState;
		
		TextView* mAudioInfo;
		TextView* mMusicName;
		StaticView* mMusicNameStatic;
		TextView* mArtist;
		TextView* mAlbum;
		
		SeekBar* mSeekBar;
		MSkBitmap *mMSkBitmap;
		
		Thread mSeekBarUpdateThread;
		Thread mShutdowmThread;
		int isNeedFresh;
		int isNeedFreshOld; //backup when sleep in and sleep out
		int mVolume;
		int mBattery;
		int isCharge;
		
	public:
		KeyCount mKeyCount;
		int isUsmCon;
		int isMediaScanning;
		
	};
	
}