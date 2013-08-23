#include "mango.h"

namespace mango
{

//mViewList ��prev Ϊ�����, mViewList��next Ϊ��Ͳ�
//����ÿһ��next Ϊ�� prev Ϊ��
//		|--prev->         (��top)
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
//		|--next->          (��bottom)

//����
//�������� -- list_add ���ӵ�ͷ��(��), list_add_tail ���ӵ�β��(��)
//head ��next ���Ϊ0, prev ���Ϊ��� 
//node prev���Сnode���� , next����Ŵ���node ���� 
//�� --  β  �� -- ͷ��


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
			sendDmsMsgToShowingView();//��һ���µ�viewҪ��ʾ��ʱ����������ʾ��view��dismiss��Ϣ

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


	//�����ָ�����ڵĹ�ϵ����
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


	//�ڴ��ڶ�ջ��, ֻ��ָ���Ĵ��ڵ��Ӵ��ڴ����ô���ǰ��, ʹ�Ӵ����ڸ���������.
	//�����ƶ��Ӵ���Ŀ
	int ViewZAxis::bringSubToUp(View* parent)
	{
		View*  view;
		int	bringCount = 0;

		view = getBottommerItem(parent);
		while (view) {
			if (view == parent) //û�д����ڸ����ڵĵ�����
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
			if (view == parent) //û�д����ڸ����ڵĵ�����
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


	//ֻ��VS_TOPMOST���ʹ��ڴ������ڶ�ջ����.
	//�����ƶ�VS_TOPMOST����Ŀ
	int ViewZAxis::bringTopmostToTop()
	{
		View*  view;
		View*  nonTopmostView = NULL;
		int  bringCount = 0 ;
	
		//���������, ���ҵ�һ����VS_TOPMOST ����
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
	//����ջ�ӿ�
	//
	//

	//��ָ�������ƶ�����ջ�Ķ���
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
		gMessageQueue.post(getDisplayingView(),VM_NOTIFY,NM_DISMISS,0);//�����л�ǰ����Ϣ��������ʾ��View
	}


	//���ָ�����д����򷵻ش��ھ�������򷵻�NULL�����λ��һ��static ��Ĵ����У�
	//�������µĴ��ھ���� ��û���򷵻�NULL��	
	View* ViewZAxis::getViewFromPoint(Point pt)
	{
		View*  view = NULL;

		mMutex.lock() ;

		view = getToppestItem () ;
		while (view) {
			if (view->mRect.ptIn(pt.x, pt.y)) {
				if (!((view->mShowState == SW_HIDE) || (view->mShowState == SW_MINIMIZE))) {
					// �鿴�Ƿ�Ϊstatic ���ʹ���,	�����static ���ʹ��ڣ���������ҡ�
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


	//��������²��Ұ���ָ����������Ĵ���
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



	//��������²��Ұ���ָ����������Ĵ���
	View* ViewZAxis::findViewContainRect(Rect& targetRect)
	{
		View*  find;

		mMutex.lock() ;

		find = _findViewContainRect(targetRect);

		mMutex.unlock();

		return find;
	}

	



	//
	//���ڲ㴦��
	//

	//�������Ͽɼ���������Ч���ཻ�Ĵ��� 
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



	//������������Ч��, ����¸���
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



	//������Ч����
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

	
	//����Ч���ڷ�WM_PAINT, ��������һЩ�����Ч����, ��SendMessage
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


	//ʹָ����������Ч
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

