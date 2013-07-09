
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
			canvas.drawImageResource(resId, 0, 0);
		
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
		}
	}

};







