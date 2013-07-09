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
			result = view->onTouchMove(msg.getParameter1() & 0x00ffff, (msg.getParameter1() >> 16) & 0x00ffff, msg.getParameter2());
			break;

		case VM_TOUCHDOWN:
			result = view->onTouchDown(msg.getParameter1() & 0x00ffff, (msg.getParameter1() >> 16) & 0x00ffff, msg.getParameter2());
			break;

		case VM_TOUCHUP:
			result = view->onTouchUp(msg.getParameter1() & 0x00ffff, (msg.getParameter1() >> 16) & 0x00ffff, msg.getParameter2());
			break;

		case VM_KEYDOWN:
			result = view->onKeyDown(msg.getParameter1(), msg.getParameter2());
			break;

		case VM_KEYUP:
			result = view->onKeyUp(msg.getParameter1(), msg.getParameter2());
			break;

		case VM_COMMAND:
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
		}

		return result;
	}

	int Party::processMessage(Message& msg)
	{
		return -1;
	}


#define MANGO_WAKELOCK_NAME  "mango"

	int Party::wakeLock(void) 
	{
#ifndef WIN32
		FILE* file;

		file  = fopen("/sys/power/wake_lock", "wt");
		if (file == NULL) {
			log_e("Can't /sys/power/wake_lock");
			return -1;
		}

		fwrite(MANGO_WAKELOCK_NAME, 1, strlen(MANGO_WAKELOCK_NAME) + 1, file);
		fclose(file);
#endif
		return 0;
	}


	int Party::wakeUnlock(void) 
	{
#ifndef WIN32
		FILE* file;

		file  = fopen("/sys/power/wake_unlock", "wt");
		if (file == NULL) {
			log_e("Can't /sys/power/wake_unlock");
			return -1;
		}

		fwrite(MANGO_WAKELOCK_NAME, 1, strlen(MANGO_WAKELOCK_NAME) + 1, file);
		fclose(file);
#endif
		return 0;
	}

}

