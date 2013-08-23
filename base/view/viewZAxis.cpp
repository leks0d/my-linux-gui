#include "mango.h"

namespace mango
{

//mViewList 的prev 为最项层, mViewList的next 为最低层
//其它每一个next 为顶 prev 为底
//		|--prev->         (顶top)
//		|
//		|		<-next
//					|
//		|			one
//					|
//		|		<-prev
//		|
// mViewHead		
//		|
//		|
//		|--next->          (底bottom)

//正序化
//正序如下 -- list_add 增加到头部(底), list_add_tail 增加到尾部(项)
//head 的next 序号为0, prev 序号为最大 
//node prev序号小node本身 , next的序号大于node 本身 
//顶 --  尾  底 -- 头部


	ViewZAxis::ViewZAxis(void)
	{
		INIT_LIST_HEAD(&mViewHead);
		messageQueue = &gMessageQueue;
	}


	ViewZAxis::~ViewZAxis(void)
	{

	}


	int ViewZAxis::del(View *view)
	{
		if (!view)
			return -1;

		list_del(&(view->mZList));
		return 0;
	}


	bool ViewZAxis::isExist(View *target)
	{
		bool success = false;
		View* view;
		LIST_HEAD *pos;

		list_for_each(pos, &mViewHead)
		{
			view = list_entry(pos, View, mZList); 
			if (view == target)
			{
				success = true;
				break ;
			}
		}

		return success ;
	}


	View* ViewZAxis::getToppestItem()
	{
		if (list_empty(&mViewHead))
			return NULL;

		return list_entry(mViewHead.prev, View, mZList); 
	}


	View* ViewZAxis::getBottommestItem()
	{
		if (list_empty(&mViewHead))
			return NULL;

		return list_entry(mViewHead.next, View, mZList); 
	}


	View* ViewZAxis::getTopperItem(View* item)
	{
		View* topperItem = NULL ;

		if (item) {
			if (item->mZList.next && item->mZList.next != &mViewHead) {
				topperItem = list_entry(item->mZList.next, View, mZList);
			}

		} else { 
			topperItem = getBottommestItem();
		}

		return topperItem ;
	}


	View* ViewZAxis::getBottommerItem(View* item)
	{
		View* bottommerItem = NULL ;

		if (item) {
			if (item->mZList.prev && item->mZList.prev != &mViewHead) {
				bottommerItem = list_entry(item->mZList.prev, View, mZList);
			}

		} else { 
			bottommerItem = getToppestItem();
		}

		return bottommerItem ;
	}


	void ViewZAxis::moveToToppest(View *item)
	{
		list_move_tail(&(item->mZList), &mViewHead) ;
	}


	void ViewZAxis::moveToTopper(View* baseView, View* moveView)
	{
		list_move(&(moveView->mZList), &(baseView->mZList)) ;
	}
	
	void ViewZAxis::moveToBottommest(View *item)
	{
		list_move(&(item->mZList), &mViewHead) ;
	}

	void ViewZAxis::moveToBottommer(View* baseView, View* moveView)
	{
		list_move_tail(&(moveView->mZList), &(baseView->mZList)) ;
	}


	int ViewZAxis::add(View *view)
	{
		View* bottommerView = NULL;
		if(view == NULL)
			return 0;
		
		log_i("ViewZAxis::add View:%s",view->name);
		if(!view->getParent())
			sendDmsMsgToShowingView();//当一个新的view要显示的时候，向正在显示的view发dismiss消息

		if (!(view->mStyle & VS_TOPMOST)) {
			bottommerView = getToppestItem();
			while (bottommerView && (bottommerView->mStyle & VS_TOPMOST))
				bottommerView = getBottommerItem(bottommerView);
		}
		
		if (bottommerView) 
			list_add(&(view->mZList), &(bottommerView->mZList));
		else 
			list_add_tail(&(view->mZList), &mViewHead);

		return 0;
	}


	//获得与指定窗口的关系窗口
	View*  ViewZAxis::getInheritor(View* parent, int serial, int flag)
	{
		View* view;
		int	count = 0;

		view = getTopperItem(parent);
		while (view) {
			if (view->mParent == parent) {
				if (count == serial)
					break ;

				count++ ;
			}
			view = getTopperItem (view);
		}

		return view;
	}


	//在窗口堆栈中, 只将指定的窗口的子窗口带到该窗口前面, 使子窗口在父窗口上面.
	//返回移动子窗数目
	int ViewZAxis::bringSubToUp(View* parent)
	{
		View*  view;
		int	bringCount = 0;

		view = getBottommerItem(parent);
		while (view) {
			if (view == parent) //没有窗口在父窗口的底下了
				break ;

			if (view->mParent == parent) {
				moveToTopper (parent, view);
				view = getBottommerItem(parent);
				bringCount++;
			} else {
				view = getBottommerItem(view);
			}
		}

		return bringCount;
	}
	
	int ViewZAxis::bringSubToDown(View* parent)
	{
		View*  view;
		int	bringCount = 0;

		view = getTopperItem(parent);
		while (view) {
			if (view == parent) //没有窗口在父窗口的底下了
				break ;

			if (view->mParent == parent) {
				moveToBottommer(parent, view);
				view = getTopperItem(parent);
				bringCount++;
			} else {
				view = getTopperItem(view);
			}
		}

		return bringCount;
	}


	//只将VS_TOPMOST类型窗口带到窗口堆栈顶点.
	//返回移动VS_TOPMOST窗数目
	int ViewZAxis::bringTopmostToTop()
	{
		View*  view;
		View*  nonTopmostView = NULL;
		int  bringCount = 0 ;
	
		//从项层向下, 查找第一个非VS_TOPMOST 窗口
		view = getToppestItem () ;
		while (view) {
			if (!(view->mStyle & VS_TOPMOST)) {
				nonTopmostView = view ;
				break ;
			}
			view = getBottommerItem(view);
		}

		if (!nonTopmostView)
			return bringCount;

		view = getBottommerItem(view);
		while (view) {
			if (view->mStyle & VS_TOPMOST) {
				moveToTopper (nonTopmostView, view) ;
				bringCount++ ;
			}
			view = getBottommerItem (view) ;
		}

		return bringCount ;
	}



	//
	//
	//窗口栈接口
	//
	//

	//将指定窗口移动窗口栈的顶部
	bool ViewZAxis::bringViewToTop(View* target)
	{
		if (!target)
			return false;
		
		sendDmsMsgToShowingView();
		
		mMutex.lock();

		moveToToppest(target);
		bringSubToUp(target);
		bringTopmostToTop();
		
		mMutex.unlock();
		
		gMessageQueue.post(target,VM_NOTIFY,NM_DISPLAY,0);
		return true;
	}

	bool ViewZAxis::bringViewToBottom(View* target)
	{
		View *view;
		if (!target)
			return false;
		
		mMutex.lock();
		
		moveToBottommest(target);
		bringSubToDown(target);
		bringTopmostToTop();

		mMutex.unlock();
		
		return true;
	}

	View* ViewZAxis::getDisplayingView(){
		View *view,*prev;
		
		mMutex.lock();
		
		prev = view = getToppestItem();
		while(view){		
			prev = view;
			view = view->getParent();
		}
		if(prev != NULL){
			log_i("ViewZAxis::getDisplayingView() view->name=%s",prev->name);
		}else{
			log_i("ViewZAxis::getDisplayingView() prev = NULL.");
		}
		mMutex.unlock();
		
		return prev;
	}

	bool ViewZAxis::isViewShowing(View *view){
		View *display;
		display = getDisplayingView();
		return display == view;
	}

	void ViewZAxis::sendDmsMsgToShowingView(){
		gMessageQueue.post(getDisplayingView(),VM_NOTIFY,NM_DISMISS,0);//窗口切换前发消息给正在显示的View
	}


	//如果指定点有窗口则返回窗口句柄，否则返回NULL。如点位于一个static 类的窗口中，
	//则返在它下的窗口句柄， 如没有则返回NULL。	
	View* ViewZAxis::getViewFromPoint(Point pt)
	{
		View*  view = NULL;

		mMutex.lock() ;

		view = getToppestItem () ;
		while (view) {
			if (view->mRect.ptIn(pt.x, pt.y)) {
				if (!((view->mShowState == SW_HIDE) || (view->mShowState == SW_MINIMIZE))) {
					// 查看是否为static 类型窗口,	如果是static 类型窗口，则继续查找。
					//if (stricmp (pWnd->pWndClass->lpszClassName, CONTROL_STATIC_NAME))
					//break ;
					break ;
				}
			}
			view = getBottommerItem (view) ;
		}

		mMutex.unlock();

		return view;
	}


	//从项层向下查找包含指定矩形区域的窗口
	View* ViewZAxis::_findViewContainRect(Rect& targetRect)
	{
		View*  find = NULL ;
		View*  view;
		Rect   rect;

		view = getToppestItem();
		while (view)
		{
			if ((view->mShowState != SW_HIDE) && (view->mShowState != SW_MINIMIZE)) {
				rect = view->mRect;
				rect.intersect(targetRect);
				if (rect == targetRect) {
					find = view ;
					break ;
				}
			}
			view = getBottommerItem (view);
		}

		return find;
	}



	//从项层向下查找包含指定矩形区域的窗口
	View* ViewZAxis::findViewContainRect(Rect& targetRect)
	{
		View*  find;

		mMutex.lock() ;

		find = _findViewContainRect(targetRect);

		mMutex.unlock();

		return find;
	}

	



	//
	//窗口层处理
	//

	//更新屏上可见区域与无效区相交的窗口 
	bool ViewZAxis::invalidViewFromScreen(View* view, View* base, Rect& invalidScreenRect)
	{
		Rect rcRemInvalid[4];
		int  i;
		bool success = true;

		while (1) {
			view = getBottommerItem (view) ;
			if (!view)
				break ;

			if (view == base) {
				view->mInvalidateRect = view->mRect;
				mExistInvalidateView = true;
				break ;
			}

			if ((view->mShowState == SW_HIDE) || (view->mShowState == SW_MINIMIZE))
				continue ;

			if (i = invalidScreenRect.subIntersect(rcRemInvalid, view->mRect)) {
				for (i-- ;  i >= 0 ; i--) {
					if ((rcRemInvalid[i].left == rcRemInvalid[i].right) || \
						(rcRemInvalid[i].top == rcRemInvalid[i].bottom))
						continue ;

					if (!invalidViewFromScreen(view, base, rcRemInvalid[i]))
						success  = false;
				}
				view->mInvalidateRect = view->mRect ;
				mExistInvalidateView = true;
				break ;
			}
		}

		return success ;
	}



	//如果各层存在无效区, 则更新各层
	bool ViewZAxis::invalidScreenRectToView(void)
	{
		View*	base;
		DWORD	dwFlushLayerFlags = 0 ;
		bool	invalidFlag = false;

		Rect  invalidScreenRect;
		Rect  screenRect;

		mMutex.lock() ;

		do
		{
			mSession->getScreenInvalidRect(invalidScreenRect);
			if (invalidScreenRect.isEmpty())
				break;

			mSession->getScreenRect(screenRect);
			base = _findViewContainRect(screenRect);
			invalidViewFromScreen(NULL, base, invalidScreenRect);
			mSession->validateScreen();
			invalidFlag = true;

		}while (0) ;

		mMutex.unlock() ;

		return invalidFlag ;
	}



	//查找无效窗口
	View*  ViewZAxis::findInvalidateView()
	{
		View* view ;

		mMutex.lock() ;

		view = getBottommestItem();
		while (view) {
			if ((view->mShowState != SW_HIDE) && \
				(view->mShowState != SW_MINIMIZE) && \
				(!view->mInvalidateRect.isEmpty())) {
				view->mInvalidateRect.set(0, 0, 0, 0);
				break ;
			}
			view = getTopperItem (view) ;
		}
		mMutex.unlock() ;
		return view ;
	}

	
	//给无效窗口发WM_PAINT, 并返回哪一些层存无效窗口, 用SendMessage
	bool ViewZAxis::sendPainMessageToAllInvalidateView()
	{
		View*  view;
		int	count;

		mMutex.lock() ;
		mExistInvalidateView = false;
		mMutex.unlock() ;

		for (count = 0; count < 500; count++)
		{
			view = findInvalidateView();
			if (view == NULL)
				break ;

			messageQueue->send(view, VM_PAINT);
		}
		return true;
	}


	//使指定形区域无效
	void ViewZAxis::invalidateScreenRect(const Rect* invalidateRect)
	{
		mSession->invalidateScreen(invalidateRect);
	}


	void ViewZAxis::invalidateViewRect(View* view, const Rect* invalidateRect)
	{
		if (!view)
			return;

		if (view->mShowState == SW_HIDE)
			return;

		mMutex.lock() ;
		
		if (invalidateRect == NULL)
			view->mInvalidateRect = view->mRect;
		else
			view->mInvalidateRect = *invalidateRect;

		mExistInvalidateView = true;

		invalidateScreenRect(NULL);

		mMutex.unlock() ;
	}

	void ViewZAxis::validateViewRect(View* view, const Rect* validateRect)
	{
		if (!view)
			return;

		mMutex.lock() ;
		view->mInvalidateRect.set(0, 0, 0, 0);
		mMutex.unlock() ;
	}


	View* ViewZAxis::setCapture(View* view)
	{
		View* old = mCaptureView;
		mCaptureView = view;
		return old;
	}


	View* ViewZAxis::getCapture(void)
	{
		return mCaptureView;
	}


	View* ViewZAxis::setFocus(View* view)
	{
		View* old = mFocusView;
		mFocusView = view;
		return old;
	}


	View* ViewZAxis::getFocus(void)
	{
		return mFocusView;
	}

	ViewZAxis  gViewZAxis;

}

