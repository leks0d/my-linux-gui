
#include "mango.h"

namespace mango
{
	SeekBar::SeekBar(void)
	{

	}

	SeekBar::SeekBar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mMax = 100;
		mProgress = 38;
		mWidth = 295;
		
		mBkgLeft = 6;
		mSeekLeft = mBkgLeft+6;
		
		mBkgX = 6;
		mBkgY = 8;
		mSeekX = 12;
		mSeekY = 14;
		mSeekWidth = 0;
	}


	SeekBar::~SeekBar(void)
	{
	}
	
	void SeekBar::setProgress(int n){
		if(mMax <= 0 || n<0 || n>mMax){
			log_i("error:invail mMax or progress.");
			return;
		}
		mProgress = n;
		mSeekWidth = n*mWidth/mMax;
		mThumbX = mSeekX+mSeekWidth-15;
#if 1		
		invalidateRect();	
#else
		cartoonDisplay();
#endif
	}
	void SeekBar::cartoonDisplay(){
		Canvas* canvas;

		canvas = getCanvas();
		
		onPaint(*canvas);

		canvas->swapScreenFrontBuffer();
		releaseCanvas();

	}
	int SeekBar::getProgress(){
		return mProgress;
	}
	
	void SeekBar::setTouchX(int x){
		int px = x - mSeekX;
		
		if(px >= mWidth)
			px = mWidth;
		if(px <= 0)
			px = 0;
		
		mSeekWidth = px;
		mProgress = mSeekWidth*mMax/mWidth;
		mThumbX = mSeekX+mSeekWidth-15;
		
		invalidateRect();
	}

	void SeekBar::setBackgroundRes(int resId,int x,int y){
		mBkgImage = resId;
		mBkgX = x;
		mBkgY = y;
	}
	void SeekBar::setSeekgroundRes(int resId,int x,int y){
		mSeekImage = resId;
		mSeekX = x;
		mSeekY = y;
	}

	int SeekBar::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
#if 0
		if(mBkgImage>0&&mSeekImage>0&&mThumbImage>0){
			canvas.drawImageResource(mBkgImage, mBkgLeft, 8);
			canvas.drawImageResource(mSeekImage, mSeekLeft, 14, mSeekWidth, 4);
			//canvas.drawImageResource(mThumbImage, mThumbX, 0);
		}
#else
		if(mBkgImage>0)
			canvas.drawImageResource(mBkgImage, mBkgX, mBkgY);
		if(mSeekImage>0)
			canvas.drawImageResource(mSeekImage, mSeekX, mSeekY,mSeekWidth,4);
#endif
		return 0;
	}
	
	int SeekBar::onTouchDown(int x, int y, int flag){
			log_i("SeekBar::onTouchDown x=%d,y=%d",x,y);
			postMessage(getParent(),VM_NOTIFY,NM_SEEKBAR_DOWM,(unsigned int)this);
			return 0;
	}

	int SeekBar::onTouchMove(int x, int y, int flag){
			log_i("SeekBar::onTouchMove x=%d,y=%d",x,y);
			setTouchX(x);
			postMessage(getParent(),VM_NOTIFY,NM_SEEKBAR_MOVE,(unsigned int)this);
			return 0;
	}
	
	int SeekBar::onTouchUp(int x, int y, int flag){
			log_i("SeekBar::onTouchUp x=%d,y=%d",x,y);
			setTouchX(x);
			postMessage(getParent(),VM_NOTIFY,NM_SEEKBAR_UP,(unsigned int)this);
			return 0;
	}
};







