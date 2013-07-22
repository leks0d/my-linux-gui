
#include "mango.h"

namespace mango
{
	VerticalSeekBar::VerticalSeekBar(void)
	{

	}

	VerticalSeekBar::VerticalSeekBar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mMax = 100;
		mProgress = 38;
		mHeight = 116;
		mBkgLeft = 6;
		mSeekLeft = mBkgLeft+6;
		mBkgImage = 0;
		mThumbImage = 0;
	}


	VerticalSeekBar::~VerticalSeekBar(void)
	{
	}
	
	void VerticalSeekBar::setProgress(int n){
		
		mProgress = n;
		mThumbY = n*mHeight/mMax;
		invalidateRect();	
	}
	
	int VerticalSeekBar::getProgress(){
		return mProgress;
	}
	
	void VerticalSeekBar::setTouchY(int y){
		
		mThumbY = y - 8;
		if(mThumbY < 0)
			mThumbY = 0;
		if(mThumbY > mHeight)
			mThumbY = mHeight;
		
		invalidateRect();
	}

	int VerticalSeekBar::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
		if(mBkgImage>0&&mSeekImage>0&&mThumbImage>0){
			canvas.drawImageResource(mBkgImage, 2, 0);
			canvas.drawImageResource(mThumbImage, 0, mThumbY);
		}
		return 0;
	}
	
	int VerticalSeekBar::onTouchDown(int x, int y, int flag){
			log_i("SeekBar::onTouchDown x=%d,y=%d",x,y);
			setTouchY(y);
			getParent()->onNotify(this,NM_SEEKBAR_DOWM,NULL);
			return 0;
	}

	int VerticalSeekBar::onTouchMove(int x, int y, int flag){
			log_i("SeekBar::onTouchMove x=%d,y=%d",x,y);
			setTouchY(y);
			getParent()->onNotify(this,NM_SEEKBAR_MOVE,NULL);
			return 0;
	}
	
	int VerticalSeekBar::onTouchUp(int x, int y, int flag){
			log_i("SeekBar::onTouchUp x=%d,y=%d",x,y);
			setTouchY(y);
			getParent()->onNotify(this,NM_SEEKBAR_UP,NULL);
			return 0;
	}
};







