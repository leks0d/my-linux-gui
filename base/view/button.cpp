
#include "mango.h"

namespace mango
{
	Button::Button(void)
	{

	}


	Button::Button(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mId = id;

		mNormalImageId  = 0;
		mPressedImageId = 0;
		mPressed = false;
	}


	Button::~Button(void)
	{

	}


	int Button::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
		Brush brush(RGB(0, 0, 0));

		rect.setEx(0, 0, mRect.right - mRect.left, mRect.bottom - mRect.top); 
		//canvas.fillRect(rect, brush);

		if (mPressed)
			imageId = mPressedImageId;
		else 
			imageId = mNormalImageId;

		if (imageId > 0)
			canvas.drawImageResource(imageId, 0, 0);


		return 0;
	}


	int Button::onTouchDown(int x, int y, int flag)
	{
		mPressed = true;
		invalidateRect();
		return 0;
	}


	int Button::onTouchUp(int x, int y, int flag)
	{
		mPressed = false;
		invalidateRect();
		postMessage(getParent(), VM_COMMAND, mId, (unsigned int)this);
		return 0;
	}


	int Button::setNormalImageId(int imageId)
	{
		mNormalImageId = imageId;
		return 0;
	}


	int Button::setPressedImageId(int imageId)
	{
		mPressedImageId = imageId;
		return 0;
	}

};







