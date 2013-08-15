
#include "mango.h"

namespace mango
{

	static void test();
	
	ValueTextView::ValueTextView(void)
	{

	}


	ValueTextView::ValueTextView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mId = id;
		mText = NULL;
		resId = 0;
		ResType = 0;
		mColor = ARGB (255 ,255, 255,255);
		mSize = 16;
		mPress = 0;
	}

	ValueTextView::~ValueTextView(void)
	{

	}

	int ValueTextView::onPaint(Canvas& canvas)
	{
		TCHAR *wt;
		wt = TEXT("MY MUSIC");
		log_i("TextView::onPaint");
		canvas.setTextColor(mColor);
		canvas.setTextSize(mSize);
		
		if (resId > 0){
			computeLeft(&canvas);
			canvas.drawTextResource(resId, 0, 0);
		}
		
		if(mText != NULL)
			canvas.drawText(mText, strlen(mText), mKeyLen+3, 0);

		if(mNormalBgdResId>0&&mPress==0){
			canvas.	drawImageResource(mNormalBgdResId,0,0);
		}else if(mSelectBgdResId>0&&mPress==1){
			canvas.	drawImageResource(mSelectBgdResId,0,0);
		}
		
		return 0;
	}

	void ValueTextView::setTextString(char* text){
		int len;

		if(mText != NULL){
			delete mText;
			mText = NULL;
		}

		if(text == NULL)
			return;
		
		len = strlen(text);
		
		if(len>0){
			mText = new char[len + 1];
			memcpy(mText,text,len+1);
			invalidateRect();
		}

	}

	void ValueTextView::setTextResoure(int id){
		if(id>0){
			resId = id;
			ResType = 1;
			//invalidateRect();
		}
	}

	void ValueTextView::setTextColor(COLORREF color){
		mColor = color;
		invalidateRect();
	}

	void ValueTextView::setTextSize(int size){
		mSize = size;
		invalidateRect();
	}

	void ValueTextView::setTextLayoutType(int layout){
		if(mLayoutType == layout)
			return;	
		mLayoutType = layout;
		invalidateRect();
	}

	void ValueTextView::computeLeft(Canvas *canvas){
		WCHAR	wStrBuf[256 + 1] ;
		Size size;
		Rect rect;
		int count,width,wpadd,hpadd;
		char utf8Path[300];
		
		getRect(rect);
		width = rect.getWidth();
		//log_i("TextView::setTextType width=%d",width);
		
		count = gSessionLocal.mResource.loadString(resId, wStrBuf, 256 + 1,gSessionLocal.mStockGraphic.mCanvas.getTextLanguage());
		
		Charset::wideCharToMultiByte(CP_UTF8, wStrBuf, String::lstrlen(wStrBuf), utf8Path, 260 * 3);
		//log_i("TextView::setTextType utf8Path=%s",utf8Path);

		canvas->getTextExtentPoint(wStrBuf,count,size);
		//log_i("TextView::setTextType size.cx=%d",size.cx);
		
		mKeyLen = size.cx;

		//log_i("TextView::setTextType mLeft=%d",mLeft);
	}

	 int ValueTextView::onTouchDown(int x, int y, int flag){
	 	
	 	return 0;
	 }

	 int ValueTextView::onTouchUp(int x, int y, int flag){
	 	log_i("onTouchUp VM_COMMAND = %d",mId);
	 	postMessage(getParent(), VM_COMMAND, mId, (unsigned int)this);
		return 0;
	 }
	
};







