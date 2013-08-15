
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
		mSelectColor = -1;
		mLeft = 0;
		mTop = 0;
		mLayoutType = 0;
		mEnable = 1;
		mLog = 0;
		iText = NULL;
	}


	TextView::~TextView(void)
	{

	}


	int TextView::onPaint(Canvas& canvas)
	{
		int charCount;
		Rect rect;

		getRect(rect);
		//log_i("TextView::onPaint:%s",mText);

		if(mNormalBgdResId>0&&mPress==0){
			canvas.	drawImageResource(mNormalBgdResId,0,0);
		}else if(mSelectBgdResId>0&&mPress==1){
			canvas.	drawImageResource(mSelectBgdResId,0,0);
		}
		
		if(mPress==1&&mSelectColor>0)
			canvas.setTextColor(mSelectColor);
		else
			canvas.setTextColor(mColor);

		if(mSelect)
			canvas.	drawImageResource(mSelectBgdResId,0,0);
		if(!mEnable)
			canvas.setTextColor(RGB(120,120,120));
		
		canvas.setTextSize(mSize);
		
		if(mLog)
			log_i("ResType=%d,mText=%s",ResType,mText);
		
		if (resId > 0 && ResType == 1){
			computeLeft(&canvas);
			canvas.drawTextResource(resId, mLeft,mTop);
		}
		if(iText != NULL && ResType == 0){
			computeLeft(&canvas);
			canvas.drawText(iText, iTextLen, mLeft, mTop);
		}

		return 0;
	}

	void TextView::setTextString(char* text){
		int len;
		int charCount;
			
		if(iText!=NULL){
			delete iText;
			iText = NULL;
			iTextLen = 0;
		}
		
		if(text == NULL)
			return;
		
		iText = new WCHAR[103];
		
		if(Charset::isTextUtf8(text)){
			charCount = Charset::multiByteToWideChar(CP_UTF8, text, strlen(text), iText, 102);
		}else{
			charCount = Charset::multiByteToWideChar(CP_ACP, text, strlen(text), iText, 102);
		}
		iText[charCount] = '\0';
		iTextLen = charCount;

		len = strlen(text);
		mText = new char[len];
		memcpy(mText,text,len+1);
		
		invalidateRect();
	}

	void TextView::getTextString(char *string){
		strcpy(string,mText);
		log_i("ResType=%d;mText:%s",ResType,mText);
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
		int count,width,hight,wpadd,hpadd;
		char utf8Path[300];
		
		getRect(rect);
		width = rect.getWidth();
		hight = rect.getHight();
		
		if(ResType == 1){
			count = gSessionLocal.mResource.loadString(resId, wStrBuf, 256 + 1,1);
			canvas->getTextExtentPoint(wStrBuf,count,size);
		}else{
			count = String::lstrlen(iText);
			canvas->getTextExtentPoint(iText,count,size);
		}
		
		wpadd = width - size.cx;
		hpadd = hight - size.cy;
		
		if(wpadd < 0)
			wpadd = 0;
		if(hpadd < 0)
			hpadd = 0;
		
		switch(mLayoutType){
			case TEXT_LAYOUT_LEFT:
				mLeft = 0;
				mTop = hpadd/2;
				break;
			case TEXT_LAYOUT_CENTER:
				mLeft = wpadd/2;
				mTop = hpadd/2;
				break;
			case TEXT_LAYOUT_RIGHT:
				mLeft = wpadd-1;
				break;
		}
	}

	 int TextView::onTouchDown(int x, int y, int flag){
	 	if(mEnable == 0)
			return 0;
	 	mPress = 1;
		invalidateRect();	
	 	return 0;
	 }
	 int TextView::onTouchUp(int x, int y, int flag){
	 	if(mEnable == 0)
			return 0;
	 	mPress = 0;
		invalidateRect();
		//log_i("onTouchUp VM_COMMAND:mId=%d",mId);
		postMessage(getParent(), VM_COMMAND, mId, (unsigned int)this);
		postMessage(getParent(), VM_NOTIFY, VSEEKBAR_TEXTVIEW_UP, (unsigned int)this);
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







