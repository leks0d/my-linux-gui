
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
	}


	ImageView::~ImageView(void)
	{
		delete mImagePath;
	}


	int ImageView::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
		
		if (resId > 0 && ResType == 1)
			canvas.drawImageResource(resId, 0, 0,true);
		if(mMSkBitmap!=NULL && mMSkBitmap->isVaild()){
			canvas.drawBitmap(mMSkBitmap->mBits,0,0,mMSkBitmap->width,mMSkBitmap->height);
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
};







