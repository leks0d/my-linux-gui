
#include <mango.h>

namespace mango
{

	TouchEvent::TouchEvent(void)
	{
		mPoints = 0;
	}

	
	TouchEvent::~TouchEvent(void)
	{

	}

	
	bool TouchEvent::setMsg(Message& msg)
	{
		bool success = true;

		switch(msg.getId())
		{
		case VM_TOUCHMOVE:
			mFlags = TOUCHEVENTF_MOVE;
			break;

		case VM_TOUCHDOWN:
			mFlags = TOUCHEVENTF_DOWN;
			break;

		case VM_TOUCHUP:
			mFlags = TOUCHEVENTF_UP;
			break;

		default:
			success = false;
		}

		if (!success)
			return success;
#if 0

		mPoint[0].x = (int)(short)(msg.getParameter1() & 0x00ffff);
		mPoint[0].y = (int)(short)((msg.getParameter1() >> 16) & 0x00ffff);
#else
		mPoint[0].x = (int)msg.getParameter1() ;
		mPoint[0].y = (int)msg.getParameter2();
#endif
		mTime = msg.getTime();

		return success;
	}


	GestureDetector::GestureDetector(void)
	{

	}
	

	GestureDetector::~GestureDetector(void)
	{

	}



	//Touch 消息滑动速度 
	bool GestureDetector::onMsg(int *pSpeed, int *pShift, UINT uMode)
	{
		Message* msg = gSessionLocal.mParty->mProcessingMsg;

		TouchEvent		event;
		int				iShift, irSpeed = 0, irShift = 0 ;
		bool			bValid = false ;
		int dx, dy;

		if (msg == NULL)
			return false;

		event.setMsg(*msg);


		do
		{
			//Down
			if (event.mFlags & TOUCHEVENTF_DOWN)
			{
				mTouchDownEvent = event;
				mStatus = TSS_NULL ;
				break ;
			}
	
			//up
			if (event.mFlags & TOUCHEVENTF_UP)
			{

				if (event.mTime < mTouchEvent[0].mTime + 60 && mTouchEvent[0].mTime < mTouchEvent[1].mTime + 60)
				{
					dx = mTouchEvent[0].mPoint[0].x - mTouchEvent[1].mPoint[0].x;
					dy = mTouchEvent[0].mPoint[0].y - mTouchEvent[1].mPoint[0].y;

					if (abs(dx) > abs(dy))
					{
						if ((uMode & TSMM_XDIRECTION) && (abs(dx) > TOUCHSCREEN_FAST_MIN_CX))
						{
							iShift  = abs(dx) - TOUCHSCREEN_FAST_MIN_CX ;
							iShift  = min((TOUCHSCREEN_FAST_MAX_CX - TOUCHSCREEN_FAST_MIN_CX), iShift) ;
							irSpeed = iShift * 100 / (TOUCHSCREEN_FAST_MAX_CX - TOUCHSCREEN_FAST_MIN_CX) ;
							if (dx < 0)
								irSpeed = 0 - irSpeed ;

							mStatus |= TSS_SHIFT ;
							bValid   = TRUE ;
						}
					}
					else
					{
						if ((!(uMode & TSMM_XDIRECTION)) && (abs(dy) > TOUCHSCREEN_FAST_MIN_CY))
						{
							iShift  = abs(dy) - TOUCHSCREEN_FAST_MIN_CY - 15;
							if(iShift<-10) iShift = 1;else if(iShift<=-5) iShift = 2;else if(iShift<=1) iShift = 3;
							iShift  = min((TOUCHSCREEN_FAST_MAX_CY - TOUCHSCREEN_FAST_MIN_CY), iShift) ;
							irSpeed = iShift * 100 / (TOUCHSCREEN_FAST_MAX_CY - TOUCHSCREEN_FAST_MIN_CY) ;
							if (dy < 0)
								irSpeed = 0 - irSpeed ;

							mStatus |= TSS_SHIFT ;
							bValid   = TRUE ;
						}
					}
				}

				if (bValid)
					break ;

				irSpeed = 0 ;
				if (!(mStatus & TSS_SHIFT))
				{
					if (mTouchEvent[0].mTime + TOUCHSCREEN_CLICK_DELAY > event.mTime)
						mStatus |= TSS_CLICK ;
				}
			}

			//move
			if (event.mFlags & (TOUCHEVENTF_UP | TOUCHEVENTF_MOVE))
			{
				if (!(mStatus & TSS_SHIFT))
				{
					if (uMode & TSMM_XDIRECTION)
						iShift = event.mPoint[0].x - mTouchDownEvent.mPoint[0].x;
					else
						iShift = event.mPoint[0].y - mTouchDownEvent.mPoint[0].y;
				}
				else
				{
					if (uMode & TSMM_XDIRECTION)
						iShift = event.mPoint[0].x - mTouchEvent[0].mPoint[0].x;
					else
						iShift = event.mPoint[0].y - mTouchEvent[0].mPoint[0].y;
				}

				if (iShift == 0)
					break ;

				if ((!(mStatus & TSS_SHIFT)) && (abs (iShift) < 24))
					break ;

				mStatus |= TSS_SHIFT ;
				mStatus &= (~TSS_CLICK) ;
				bValid	 = TRUE	 ;
				irShift  = iShift ;
				break ;
			}

		} while (0) ;


		mTouchEvent[1] = mTouchEvent[0];
		mTouchEvent[0] = event;

		if (bValid)
		{
			if (pSpeed)
				*pSpeed = irSpeed;

			if (pShift)
				*pShift = irShift;
		}

		return bValid ;
	}


	bool GestureDetector::isMove(void)
	{
		return (mStatus & TSS_SHIFT) ? true : false;
	}


	unsigned int GestureDetector::getStatus(void)
	{
		return mStatus;
	}

};

