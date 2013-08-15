#include "mango.h"

namespace mango
{
	Party::Party(void)
	{

	}
	
	Party::~Party(void)
	{

	}

	
	int Party::initialize()
	{
		gSession.initialize(this);
		gSessionLocal.initialize(this);
		
		mSession = &gSession;
		mSessionLocal = &gSessionLocal; 
		mViewZAxis = &gViewZAxis;

		mMessageQueue = &gMessageQueue;
		mMessageQueue->mCustomerThreadId = Thread::getCurrentId();
		
		gSession.start();

		return 0;
	}
	
	
	int Party::messageLoop()
	{
		Message msg;
		log_i("Party::messageLoop");
		while (mMessageQueue->get(msg) > 0)	{
			dispatchMessage(msg);
		}

		return 0;
	}


	int Party::getLastError(void)
	{
		return -1;
	}
	

	int Party::dispatchMessage(Message& msg)
	{
		int result = -1;
		View *view = msg.getView();

		if (view == NULL)
			return processMessage(msg);

		switch(msg.getId())
		{
		case VM_PAINT:
			mSessionLocal->mStockGraphic.mCanvas.renew(view);
			result = view->onPaint(mSessionLocal->mStockGraphic.mCanvas);
			break;

		case VM_TOUCHMOVE:
			result = view->onTouchMove(msg.getParameter1(), msg.getParameter2(),0);
			break;

		case VM_TOUCHDOWN:
			result = view->onTouchDown(msg.getParameter1(), msg.getParameter2(),0);
			break;

		case VM_TOUCHUP:
			result = view->onTouchUp(msg.getParameter1(), msg.getParameter2(),0);
			break;

		case VM_KEYDOWN:
			result = view->onKeyDown(msg.getParameter1(), msg.getParameter2());
			break;

		case VM_KEYUP:
			result = view->onKeyUp(msg.getParameter1(), msg.getParameter2());
			break;

		case VM_COMMAND:
			log_i("view->onCommand msg.getParameter1()=%d",msg.getParameter1());
			result = view->onCommand(msg.getParameter1() & 0x00ffff, (msg.getParameter1() >> 16) & 0x00ffff, (View* )(msg.getParameter2()));
			break;

		case VM_CREATE:
			result = view->onCreate();
			break;
		case VM_DESTROY:
			result = view->onDestroy();
			break;

		case VM_ACTIVATE:
			result = view->onActivate();
			break;

		case VM_INACTIVATE:
			result = view->onInactivate();
			break;
		case VM_NOTIFY:
			result = view->onNotify(NULL,msg.getParameter1(),(void*)msg.getParameter2());
			break;
		}

		return result;
	}

	int Party::processMessage(Message& msg)
	{
		return -1;
	}


#define MANGO_WAKELOCK_NAME  "mango"

	int Party::wakeLock(const char *name) 
	{
#ifndef WIN32
		FILE* file;

		file  = fopen("/sys/power/wake_lock", "wt");
		if (file == NULL) {
			log_e("Can't /sys/power/wake_lock");
			return -1;
		}
		log_i("Party::wakeLock name=%s",name);
		fwrite(name, 1, strlen(name) + 1, file);
		fclose(file);
#endif
		return 0;
	}


	int Party::wakeUnlock(const char *name) 
	{
#ifndef WIN32
		FILE* file;

		file  = fopen("/sys/power/wake_unlock", "wt");
		if (file == NULL) {
			log_e("Can't /sys/power/wake_unlock");
			return -1;
		}
		log_i("Party::wakeUnlock name=%s",name);
		fwrite(name, 1, strlen(name) + 1, file);
		fclose(file);
#endif
		return 0;
	}

}

