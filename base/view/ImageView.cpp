
#include "mango.h"

namespace mango
{
	ImageView::ImageView(void)
	{

	}


	ImageView::ImageView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mId = id;
		mImagePath = NULL;
		resId = 0;
		ResType = 0;
		mMSkBitmap = NULL;
		mAlps = true;
		mStartX = 0;
		mStartY = 0;
		mVisiable = true;
	}


	ImageView::~ImageView(void)
	{
		delete mImagePath;
	}


	int ImageView::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
		if(!mVisiable)
			return 0;
		
		if(mMSkBitmap!=NULL && mMSkBitmap->isVaild()){
			canvas.drawBitmap(mMSkBitmap->mBits,mStartX,mStartY,mMSkBitmap->width,mMSkBitmap->height,false);
  		}else if (resId > 0 && ResType == 1){
			canvas.drawImageResource(resId, mStartX, mStartY,mAlps);
		} 
		return 0;
	}

	void ImageView::setImagePath(char* text){
		int len;
		
		len = strlen(text);
		
		if(len>0){
			mImagePath = new char[len + 1];
			memcpy(mImagePath,text,len+1);
			ResType = 0;
			invalidateRect();
		}
	}

	void ImageView::setImageResoure(int id){
		if(id>0){
			resId = id;
			ResType = 1;
			invalidateRect();
		}else{
			ResType = -1;
		}
	}

	void ImageView::setSkBitmap(int *bit,int w,int h){
		if(mMSkBitmap == NULL){
			mMSkBitmap = new MSkBitmap();
		}else
			mMSkBitmap->release();
		mMSkBitmap->create(bit,w,h);
	}

	void ImageView::setMSkBitmap(MSkBitmap *bitmap){
		mMSkBitmap = bitmap;
	}

	void ImageView::setVisiable(bool visiable){
		if(visiable != mVisiable){
			mVisiable = visiable;
			invalidateRect();
		}
	}
	int ImageView::onTouchUp(int x, int y, int flag){
		postMessage(getParent(), VM_NOTIFY, IMAGEVIEW_TOUCH_UP, (unsigned int)this);
	}
};







