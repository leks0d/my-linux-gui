
#ifndef _MANGO_GESTUREDETECTOR_H
#define _MANGO_GESTUREDETECTOR_H

namespace mango
{


	//定义在出现拖动效果之前, 触点可以移动的宽度
#define DRAG_CY		32
#define DRAG_CX		32

	//定义在出现拖动效果之前, 触点可以移动的宽度
#define TOUCHSCREEN_DRAG_CX			32
#define TOUCHSCREEN_DRAG_CY			32
#define TOUCHSCREEN_FAST_MIN_CX		20
#define TOUCHSCREEN_FAST_MAX_CX		42
#define TOUCHSCREEN_FAST_MIN_CY		3
#define TOUCHSCREEN_FAST_MAX_CY		36
#define TOUCHSCREEN_CLICK_DELAY		500

	//动画 每秒帧数
#define CARTOON_FRAMES_PER_SECOND	24	
#define CARTOON_MS_PER_FRAME		(1000/CARTOON_FRAMES_PER_SECOND)	

	//选中延时
#define TOUCHSCREEN_SELECTED_DELAY   1200


	//Touch 模式 
#define TSMM_YDIRECTION		0x0000	// y 方向移动
#define TSMM_XDIRECTION		0x0001	// x 方向移动
#define TSMM_TWOPOINTS		0x0002	// 双指处理
#define TSMM_TWOSHIFT		0x0004	// 双指移动

	//Touch 快速滑动速度
#define TS_FASTMOVE_SPEED_MIN 0
#define TS_FASTMOVE_SPEED_MAX 100

#define TSS_NULL		0x0000
#define TSS_SHIFT		0x0001
#define TSS_TWOPOINTS	0x0002 //双触点	
#define TSS_CLICK		0x0004 //单触点单击
#define TSS_TWOCHECKED	0x0100 //双触点被检测到过



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
		DWORD		  mTwoPointsTime; //双触点时间

	};
};


#endif