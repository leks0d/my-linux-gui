
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
	}


	SeekBar::~SeekBar(void)
	{
	}
	
	void SeekBar::setProgress(int n){
		
		mProgress = n;
		mSeekWidth = n*mWidth/mMax;
		mThumbX = mSeekLeft+mSeekWidth-15;
		invalidateRect();	
	}
	
	int SeekBar::getProgress(){
		return mProgress;
	}
	
	void SeekBar::setTouchX(int x){
		int px = x - mSeekLeft;
		
		if(px >= mWidth)
			px = mWidth;
		
		mSeekWidth = px;
		mProgress = mSeekWidth*mMax/mWidth;
		mThumbX = mSeekLeft+mSeekWidth-15;
		
		invalidateRect();
	}

	int SeekBar::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
		if(mBkgImage>0&&mSeekImage>0&&mThumbImage>0){
			canvas.drawImageResource(mBkgImage, mBkgLeft, 8);
			canvas.drawImageResource(mSeekImage, mSeekLeft, 14, mSeekWidth, 4);
			//canvas.drawImageResource(mThumbImage, mThumbX, 0);
		}
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







