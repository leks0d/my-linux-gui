
#ifndef _MANGO_VIEW_H
#define _MANGO_VIEW_H

namespace mango
{

#define VS_TOPMOST      0x00000008
#define VS_CHILD        0x40000000

#ifndef WIN323
#define SW_HIDE             0
#define SW_NORMAL           1
#define SW_MINIMIZE         6
#endif

#define KEYCODE_ENTER		0x0002
#define KEYCODE_VOLUMEUP		158
#define KEYCODE_VOLUMEDOWN		159
#define KEYCODE_PREV		160
#define KEYCODE_PLAY		161
#define KEYCODE_NEXT		162
#define KEYCODE_CAPACITY	200
#define KEYCODE_HEADEST	113
#define KEYCODE_SPIDF		114
#define KEYCODE_HOLD		115
#define KEYCODE_SHORT_POWER		116
#define KEYCODE_LONG_POWER		117
#define KEYCODE_MEDIA_CHANGE		117
#define KEYCODE_BACK		188

#define TOUCH_FLAG_DOWN  0x0001
#define TOUCH_FLAG_UP    0x0002
#define TOUCH_FLAG_MOVE  0x0003

#define NM_CLICK     		1   
#define NM_DISPLAY     	2  
#define NM_DISMISS     	3
#define NM_SEEKBAR_DOWM   4
#define NM_SEEKBAR_MOVE   	5
#define NM_SEEKBAR_UP   	6
#define NM_PLAY_COM   		7
#define NM_SEEK_UPDATE   	8
#define NM_BATTERY_UPDATE 9
#define FLASH_UNMOUNT 		10
#define FLASH_MOUNT 		11
#define SDCARD_UNMOUNT 	12
#define SDCARD_MOUNT 		13
#define MEDIA_SCANNER_START 		14
#define MEDIA_SCANNER_END 		15
#define NM_SPIDF 			16
#define NM_HEADEST 			17

	class View : public Object
	{
	public:
		View(void);
		View(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);

		virtual ~View(void);

	public:
		virtual int onCreate();
		virtual int onDestroy();

		virtual int onActivate();
		virtual int onInactivate();

		virtual int onPaint(Canvas& canvas);

		virtual int setTitle(const TCHAR* title);
		virtual TCHAR* getTitle();

		virtual void  getRect(Rect &rect);
		virtual void  setRect(Rect &rect);

		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 

		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchMove(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);

		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onCommand(int id, int code, View* fromView);
	

	public:
		void  invalidateRect(const Rect* rect = NULL); 
		void  getClientRect(Rect& rect);

		View* setCapture(View* view = NULL);
		View* releaseCapture();
		View* getCapture();
		View* getParent();

		Canvas* getCanvas();
		void    releaseCanvas();


		View* setFocus(View* view = NULL);
		View* getFocus(void);

		int sendMessage(class View* view, unsigned int id, unsigned int parameter1 = 0, unsigned int paramter2 = 0);
		int postMessage(class View* view, unsigned int id, unsigned int parameter1 = 0, unsigned int paramter2 = 0);


	public:
		int mStyle;
		View* mParent;
		int  mShowState;

		Rect  mRect;
		Rect  mInvalidateRect;
		int   mId;
		char* 	name;

	protected:
		LIST_HEAD mZList;

	private:
		MessageQueue* mMessageQueue;
		TCHAR*    mTitle;
		

	friend class ViewZAxis;

	};

	class UseEventInterface{
		public:
		UseEventInterface(){}
		~UseEventInterface(){}
		virtual int onKeyDispatch(int keyCode,int action, int flag);
		virtual int onTouchDispatch(int x,int y, int action);
	};
}

#endif