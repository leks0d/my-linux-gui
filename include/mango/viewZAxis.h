
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
		//��ָ�������ƶ�����ջ�Ķ���
		bool bringViewToTop(View* target);

		//���ָ�����д����򷵻ش��ھ�������򷵻�NULL�����λ��һ��static ��Ĵ����У�
		//�������µĴ��ھ���� ��û���򷵻�NULL��	
		View* getViewFromPoint(Point pt);

		View* getDisplayingView();

		bool isViewShowing(View *view);

		//��������²��Ұ���ָ����������Ĵ���
		View* findViewContainRect (Rect& rect);

		//�������Ͽɼ���������Ч���ཻ�Ĵ��� 
		bool invalidViewFromScreen(View* view, View* base, Rect& invalidScreenRect);
		bool invalidScreenRectToView(void);

		//����Ч���ڷ�WM_PAINT, ��������һЩ�����Ч����, ��SendMessage
		bool sendPainMessageToAllInvalidateView();

		//ʹָ����������Ч
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

		//�����ָ�����ڵĹ�ϵ����
		View* getInheritor(View* parent, int serial, int flag = 0);
		
		//�ڴ��ڶ�ջ��, ֻ��ָ���Ĵ��ڵ��Ӵ��ڴ����ô���ǰ��, ʹ�Ӵ����ڸ���������.
		//�����ƶ��Ӵ���Ŀ
		int bringSubToUp(View* parent);
		int bringSubToDown(View* parent);

		//ֻ��VS_TOPMOST���ʹ��ڴ������ڶ�ջ����.
		//�����ƶ�VS_TOPMOST����Ŀ
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