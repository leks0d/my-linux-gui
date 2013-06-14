#include "player.h"

namespace mango
{
	enum
	{
		PLAYING_IDB_NEXT = 10,
		PLAYING_IDB_PREV,
	};

	PlayingView::PlayingView(void)
	{

	}


	PlayingView::PlayingView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		
	}


	PlayingView::~PlayingView(void)
	{

	}


	int PlayingView::onCreate()
	{

		if (gPlaylist == NULL)
			gPlaylist = new Playlist;

		Rect rect;
		rect.setEx(20, 80, 80, 80);
		mPrevButton = new Button(PLAYING_IDB_PREV, NULL, this, &rect, 0);
		mPrevButton->setNormalImageId(IDP_PLAYING_PREV_INACTIVE);
		mPrevButton->setPressedImageId(IDP_PLAYING_PREV_ACTIVE);
		mPrevButton->onCreate();


		rect.setEx(180, 80, 80, 80);
		mNextButton = new Button(PLAYING_IDB_NEXT, NULL, this, &rect, 0);
		mNextButton->setNormalImageId(IDP_PLAYING_NEXT_INACTIVE);
		mNextButton->setPressedImageId(IDP_PLAYING_NEXT_ACTIVE);
		mNextButton->onCreate();


		return -1;
	}


	int PlayingView::onDestroy()
	{
		if (mPrevButton)
			mPrevButton->onDestroy();
		
		if (mNextButton)
			mNextButton->onDestroy();

		safeFree(mPrevButton);
		safeFree(mNextButton);

		return -1;
	}


	int PlayingView::onPaint(Canvas& canvas)
	{
		canvas.drawImageResource(IDP_PLAYING_BACKGROUND, 0, 0, false);
		return 0;
	}


	int PlayingView::onNotify(View* fromView, int code, void* parameter)
	{
		return -1;
	}


	int PlayingView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode)	{
		case KEYCODE_BACK:
			break;
		}

		return 0;
	}


	int PlayingView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode)	{
		case KEYCODE_BACK:
			gPlayer.showMediaView();
			break;
		}

		return -1;
	}


	int PlayingView::onCommand(int id, int code, View* fromView)
	{
		int volume;

		switch(id)
		{
		case PLAYING_IDB_NEXT:
			volume = gPlayer.getVolume();
			volume += 4;
			if (volume > 255)
				volume = 255;
				gPlayer.setVolume(volume);
			break;

		case PLAYING_IDB_PREV:
			volume = gPlayer.getVolume();
			volume -= 4;
			if (volume < 0)
				volume = 0;
				gPlayer.setVolume(volume);
			break;

		}
		return -1;
	}


};