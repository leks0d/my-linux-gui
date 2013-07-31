
#ifndef _MANGO_VIEWAXIS_H
#define _MANGO_VIEWAXIS_H

namespace mango
{


#ifdef WIN32
	class EXPORT_CLASS ViewZAxis; 
#endif


	class ViewZAxis : public Object
	{
	public:
		ViewZAxis(void);
		virtual ~ViewZAxis(void);

		int add(View *view);
		int del(View *view);
		bool isExist(View *view);

		bool bringViewToBottom(View *tager);
		//将指定窗口移动窗口栈的顶部
		bool bringViewToTop(View* target);

		//如果指定点有窗口则返回窗口句柄，否则返回NULL。如点位于一个static 类的窗口中，
		//则返在它下的窗口句柄， 如没有则返回NULL。	
		View* getViewFromPoint(Point pt);

		View* getDisplayingView();

		bool isViewShowing(View *view);

		//从项层向下查找包含指定矩形区域的窗口
		View* findViewContainRect (Rect& rect);

		//更新屏上可见区域与无效区相交的窗口 
		bool invalidViewFromScreen(View* view, View* base, Rect& invalidScreenRect);
		bool invalidScreenRectToView(void);

		//给无效窗口发WM_PAINT, 并返回哪一些层存无效窗口, 用SendMessage
		bool sendPainMessageToAllInvalidateView();

		//使指定形区域无效
		void invalidateScreenRect(const Rect* invalidateRect);

		void invalidateViewRect(View* view, const Rect* invalidateRect);

		void validateViewRect(View* view, const Rect* validateRect);

		
		View*  setCapture(View* view);
		View*  getCapture(void);

		View*  setFocus(View* view);
		View*   getFocus(void);


	public:
		bool mExistInvalidateView;
		class Session* mSession;
		class MessageQueue  *messageQueue;

	private:
		View* getToppestItem();
		View* getBottommestItem();
		View* getTopperItem(View* item);
		View* getBottommerItem(View* item);

		void moveToToppest(View *item);
		void moveToTopper(View* baseItem, View* moveItem);
		void moveToBottommest(View *item);
		void moveToBottommer(View* baseItem, View* moveItem);

		//获得与指定窗口的关系窗口
		View* getInheritor(View* parent, int serial, int flag = 0);
		
		//在窗口堆栈中, 只将指定的窗口的子窗口带到该窗口前面, 使子窗口在父窗口上面.
		//返回移动子窗数目
		int bringSubToUp(View* parent);
		int bringSubToDown(View* parent);

		//只将VS_TOPMOST类型窗口带到窗口堆栈顶点.
		//返回移动VS_TOPMOST窗数目
		int bringTopmostToTop();

		View*  findInvalidateView();

		View* _findViewContainRect (Rect& rect);

		void sendDmsMsgToShowingView();

		LIST_HEAD mViewHead;
		Mutex mMutex;


		View*  mCaptureView;
		View*  mFocusView;


	};

	extern ViewZAxis gViewZAxis;
}

#endif