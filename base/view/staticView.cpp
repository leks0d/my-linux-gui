
#include "mango.h"

namespace mango
{
	StaticView::StaticView(void)
	{

	}


	StaticView::StaticView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		:View(title, parent, rect, style, show)
	{

	}


	StaticView::~StaticView(void)
	{

	}


	int StaticView::onPaint(Canvas& canvas)
	{
		Rect rect;
		TCHAR *title;
		getRect(rect);

		title = getTitle();
		if (title)
			canvas.drawText(title, -1, rect, DT_SINGLELINE | DT_NOPREFIX);

		return 0;
	}


};

