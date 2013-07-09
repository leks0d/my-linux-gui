
#include "mango.h"

namespace mango
{

	static void test();
	
	TextView::TextView(void)
	{

	}


	TextView::TextView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mId = id;
		mText = NULL;
		resId = 0;
		ResType = 0;
		mColor = ARGB (255 ,0, 0, 0);
		mSize = 16;
		mPress = 0;
	}


	TextView::~TextView(void)
	{

	}


	int TextView::onPaint(Canvas& canvas)
	{
		TCHAR *wt;
		wt = TEXT("MY MUSIC");
		
		canvas.setTextColor(mColor);
		canvas.setTextSize(mSize);
		
		if (resId > 0 && ResType == 1){
			computeLeft(&canvas);
			canvas.drawTextResource(resId, mLeft, 0);
			//canvas.drawText(wt, String::lstrlen(wt), 0, 0);
		}
		if(mText != NULL && ResType == 0)
			canvas.drawText(mText, strlen(mText), 0, 0);
		
		if(mNormalBgdResId>0&&mPress==0){
			canvas.	drawImageResource(mNormalBgdResId,0,0);
		}else if(mSelectBgdResId>0&&mPress==1){
			canvas.	drawImageResource(mSelectBgdResId,0,0);
		}
		return 0;
	}

	void TextView::setTextString(char* text){
		int len;
		
		len = strlen(text);
		
		if(len>0){
			mText = new char[len + 1];
			memcpy(mText,text,len+1);
			ResType = 0;
			invalidateRect();
		}

	}
	
	void TextView::setTextResoure(int id){
		if(id>0){
			resId = id;
			ResType = 1;
			invalidateRect();
		}
	}
	void TextView::setTextColor(COLORREF color){
		mColor = color;
		invalidateRect();
	}
	
	void TextView::setTextSize(int size){
		mSize = size;
		invalidateRect();
	}
	void TextView::setTextLayoutType(int layout){
		if(mLayoutType == layout)
			return;	
		mLayoutType = layout;			
		invalidateRect();
	}
	void TextView::computeLeft(Canvas *canvas){
		WCHAR	wStrBuf[256 + 1] ;
		Size size;
		Rect rect;
		int count,width,wpadd,hpadd;
		char utf8Path[300];

		//test();
		
		//log_i("TextView::setTextType");
		
		getRect(rect);
		width = rect.getWidth();
		//log_i("TextView::setTextType width=%d",width);
		
		count = gSessionLocal.mResource.loadString(resId, wStrBuf, 256 + 1,1);
		
		Charset::wideCharToMultiByte(CP_UTF8, wStrBuf, String::lstrlen(wStrBuf), utf8Path, 260 * 3);
		//log_i("TextView::setTextType utf8Path=%s",utf8Path);

		canvas->getTextExtentPoint(wStrBuf,count,size);
		//log_i("TextView::setTextType size.cx=%d",size.cx);
		
		wpadd = width-size.cx;
		if(wpadd<0)
			wpadd = 0;
		
		switch(mLayoutType){
			case TEXT_LAYOUT_LEFT:
				break;
			case TEXT_LAYOUT_CENTER:
				mLeft = wpadd/2;
				break;
			case TEXT_LAYOUT_RIGHT:
				mLeft = wpadd-1;
				break;
		}
		//log_i("TextView::setTextType mLeft=%d",mLeft);
	}

	 int TextView::onTouchDown(int x, int y, int flag){
	 	
	 	return 0;
	 }
	 int TextView::onTouchUp(int x, int y, int flag){
	 	postMessage(getParent(), VM_COMMAND, mId, (unsigned int)this);
		return 0;
	 }


	static void test(){
		FILE *fb;
		TCHAR *wt, rt[8];
		char wtutf8[50],rtutf8[50];
		wt = TEXT("My music");

		fb  = fopen("/etc/res_test", "wb");
		if(fwrite(wt,32,1,fb)==1)
			log_i("test write /etc/res_test sucess.")
		fclose(fb);
		fb  = fopen("/etc/res_test", "rb");
		if(fread(rt,32,1,fb)==1)
			log_i("test read /etc/res_test sucess.")
		fclose(fb);

		log_i("TCHAR size=%d",sizeof(TCHAR)/sizeof(char));
		
		Charset::wideCharToMultiByte(CP_UTF8, wt, String::lstrlen(wt), wtutf8, 260 * 3);
		Charset::wideCharToMultiByte(CP_UTF8, rt, String::lstrlen(rt), rtutf8, 260 * 3);
		
		log_i("test wtutf8=%s",wtutf8);
		
		log_i("test rtutf8=%s",rtutf8);

		fb  = fopen("/etc/res_test_char", "wb");
		if(fwrite(wtutf8,strlen(wtutf8),1,fb)==1)
			log_i("test write /etc/res_test_char sucess.")
		char* buf;
		buf = (char*)wt;
		log_i("buf=%x %x %x %x",buf[0],buf[1],buf[2],buf[3]);
		fclose(fb);	
	}
	
};







