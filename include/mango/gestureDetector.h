
#ifndef _MANGO_GESTUREDETECTOR_H
#define _MANGO_GESTUREDETECTOR_H

namespace mango
{


	//�����ڳ����϶�Ч��֮ǰ, ��������ƶ��Ŀ��
#define DRAG_CY		32
#define DRAG_CX		32

	//�����ڳ����϶�Ч��֮ǰ, ��������ƶ��Ŀ��
#define TOUCHSCREEN_DRAG_CX			32
#define TOUCHSCREEN_DRAG_CY			32
#define TOUCHSCREEN_FAST_MIN_CX		20
#define TOUCHSCREEN_FAST_MAX_CX		42
#define TOUCHSCREEN_FAST_MIN_CY		18
#define TOUCHSCREEN_FAST_MAX_CY		38
#define TOUCHSCREEN_CLICK_DELAY		500

	//���� ÿ��֡��
#define CARTOON_FRAMES_PER_SECOND	24	
#define CARTOON_MS_PER_FRAME		(1000/CARTOON_FRAMES_PER_SECOND)	

	//ѡ����ʱ
#define TOUCHSCREEN_SELECTED_DELAY   1200


	//Touch ģʽ 
#define TSMM_YDIRECTION		0x0000	// y �����ƶ�
#define TSMM_XDIRECTION		0x0001	// x �����ƶ�
#define TSMM_TWOPOINTS		0x0002	// ˫ָ����
#define TSMM_TWOSHIFT		0x0004	// ˫ָ�ƶ�

	//Touch ���ٻ����ٶ�
#define TS_FASTMOVE_SPEED_MIN 0
#define TS_FASTMOVE_SPEED_MAX 100

#define TSS_NULL		0x0000
#define TSS_SHIFT		0x0001
#define TSS_TWOPOINTS	0x0002 //˫����	
#define TSS_CLICK		0x0004 //�����㵥��
#define TSS_TWOCHECKED	0x0100 //˫���㱻��⵽��



#define TOUCHEVENTF_MOVE        0x0001 /* touch move */
#define TOUCHEVENTF_DOWN		0x0002 /* touch button down */
#define TOUCHEVENTF_UP			0x0004 /* touch button up */
#define TOUCHEVENTF_ABSOLUTE    0x8000 /* absolute move */


	class TouchEvent : public Object
	{
	public:
		TouchEvent(void);
		virtual ~TouchEvent(void);

		bool setMsg(Message& msg);

	public:
		int   mPoints;
		Point mPoint[3];

		unsigned int mFlags;
		unsigned int mTime;
		unsigned int mExtraInfo;
	};



	class GestureDetector : public Object
	{
	public:
		GestureDetector(void);
		virtual ~GestureDetector(void);

		bool onMsg(int *pSpeed, int *pShift, UINT uMode);


		unsigned int getStatus(void);
		bool isMove(void);

	private:
		unsigned int mStatus;

		TouchEvent	  mTouchEvent[2];
		TouchEvent	  mTouchDownEvent;
		DWORD		  mTwoPointsTime; //˫����ʱ��

	};
};


#endif