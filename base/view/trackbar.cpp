

#include "mango.h"

namespace mango
{

	Trackbar::Trackbar(void)
	{

	}


	Trackbar::Trackbar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
		mId = id;
	}

	Trackbar::~Trackbar(void)
	{

	}

	
	//���ݵ�ǰλ��, ������ѻ��г���
	int	 Trackbar::getDoneLength (Rect& trackRect)
	{
		int		doneLength;
		int		trackTotalLength;	

		if (mStyle & TBS_VERT)
			trackTotalLength = trackRect.bottom - trackRect.top - mLayout.m_sizeThumb.cy; 
		else
			trackTotalLength = trackRect.right - trackRect.left - mLayout.m_sizeThumb.cx;

		if (mCurrentValue <= mMinimum)
			doneLength = 0;
		else if (mMinimum >= mMaximum)
			doneLength = trackTotalLength;
		else
			doneLength = trackTotalLength *  (mCurrentValue - mMinimum) / (mMaximum - mMinimum);		

		return doneLength ;
	}



	//����Trackbar ��ǰλ�û�û�����ʾλ�� 
	void Trackbar::getThumbDisplayPosition (Point& displayPosition)
	{
		int	doneLength;
		Rect activityArea;	

		activityArea = mRect;
		activityArea.subMargin(mLayout.m_rcMarginTrack);

		doneLength = getDoneLength (activityArea);

		if (mStyle & TBS_VERT)
		{
			displayPosition.x = activityArea.left ;
			if (mStyle & TBS_DOWNISLEFT)
			{
				displayPosition.y  = activityArea.bottom - doneLength ;
				displayPosition.y -= mLayout.m_sizeThumb.cy ;
			}
			else
				displayPosition.y = activityArea.top + doneLength ;
		}
		else
		{
			displayPosition.y = (activityArea.top + activityArea.bottom - mLayout.m_sizeThumb.cy) / 2 ;
			displayPosition.x = activityArea.left + doneLength ;
		}	
	}


	//���ݻ�����ʾλ�� ���Trackbar ��ǰλ��
	//thumbCenter thumb ����λ�� 
	int Trackbar::getPosition(Point& thumbCenter)
	{
		int		doneLength;
		int		trackTotalLength;
		int		position;

		Rect activityArea;	

		activityArea = mRect;
		activityArea.subMargin(mLayout.m_rcMarginTrack);

		if (mStyle & TBS_VERT) 
		{
			if (mStyle & TBS_DOWNISLEFT) 
			{
				doneLength  = activityArea.bottom - thumbCenter.y;
			} 
			else 
			{
				doneLength  = thumbCenter.y - activityArea.top;
			}

			doneLength -= (mLayout.m_sizeThumb.cy / 2);
			trackTotalLength = activityArea.bottom - activityArea.top - mLayout.m_sizeThumb.cy;
		}
		else
		{
			doneLength  = thumbCenter.x  - activityArea.left;
			doneLength -= (mLayout.m_sizeThumb.cx / 2);
			trackTotalLength = activityArea.right - activityArea.left - mLayout.m_sizeThumb.cx;
		}

		if (mMinimum >= mMaximum)
			position = mMinimum;
		else if (trackTotalLength <= 0)
			position = mMinimum;
		else
		{
			position  = (doneLength * (mMaximum - mMinimum) + trackTotalLength - 1) / trackTotalLength;
			position += mMinimum;
			position  = max(position, mMinimum);
			position  = min(position, mMaximum);
		}

		return position;
	}

#if 0

	//����WM_PAINT ��Ϣ
	LRESULT	 ctrlTrackbarPaint (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		HDC		hdc	  ;
		DWORD	mStyle ;
		HRGN	hRgn  ;
		RECT	rcClient, rcTrack ;
		POINT   ptPosThumb ;

		PCTRL_TRACKBAR	pTrackbar ;
		PAINTSTRUCT		ps  ;

		log_i("Enter\n");
		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0) ;
		mStyle	  = (DWORD)GetWindowLong (hWnd, GWL_STYLE) ;

		GetClientRect (hWnd, &rcClient) ;
		SubMarginRect (&rcTrack, &rcClient, &(mLayout.m_rcMarginTrack)) ;
		Trackbar:getThumbDisplayPosition (pTrackbar, &ptPosThumb, mStyle, &rcClient) ;

		if (mStyle & TBS_VERT)
		{
			if (mStyle & TBS_DOWNISLEFT)
			{
				hRgn = CreateRectRgn (rcTrack.left, ptPosThumb.y + mLayout.m_sizeThumb.cy / 2, \
					rcTrack.right, rcTrack.bottom) ;
			}
			else
			{
				hRgn = CreateRectRgn (rcTrack.left, rcTrack.top, \
					rcTrack.right, ptPosThumb.y + mLayout.m_sizeThumb.cy / 2) ;
			}
		}
		else
		{
			hRgn = CreateRectRgn (rcTrack.left, rcTrack.top, ptPosThumb.x + mLayout.m_sizeThumb.cx / 2, rcTrack.bottom) ;
		}

		hdc = BeginPaint (hWnd, &ps) ;

		if (mStyle & TBS_TRANSPARENTBKGND)
			SendMessage (GetParent (hWnd), WM_PRINTCLIENT, (WPARAM)hdc, PRF_CLIENT) ;

		DrawBmpResource (hdc, 0, 0, mLayout.m_hInstance, mLayout.m_iImageBk) ;
		SelectClipRgn   (hdc, hRgn) ;
		DrawPngResource (hdc, rcTrack.left, rcTrack.top, mLayout.m_hInstance, mLayout.m_iImageTrackDone) ;
		DeleteObject ((HGDIOBJ)hRgn) ;

		if (!(mStyle & TBS_NOTHUMB))
		{
			SelectClipRgn (hdc, NULL) ;
			DrawPngResource (hdc, ptPosThumb.x, ptPosThumb.y, mLayout.m_hInstance, mLayout.m_iImageThumb) ;
		}

		EndPaint (hWnd, &ps) ;

		return 0 ;
	}


	//����WM_KEYDOWN ��Ϣ
	LRESULT	 ctrlTrackbarKeydown (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		DWORD		 mStyle       ;
		PCTRL_TRACKBAR pTrackbar ;
		int			 nVirtKey    ;
		DWORD		 dwKeyData   ;
		RECT		 rcClient ;
		int			 iPos ;
		POINT		 ptPosThumb, ptNewPosThumb ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;
		mStyle	  = (DWORD)GetWindowLong (hWnd, GWL_STYLE) ;


		nVirtKey  = (int) wParam ;    
		dwKeyData = (DWORD)lParam;    

		iPos  = mCurrentValue ;

		switch (nVirtKey)
		{
		case VK_UP:
		case VK_UP_LONG:
			iPos++ ;
			iPos = min (iPos, mMaximum) ;
			break ;

		case VK_DOWN:
		case VK_DOWN_LONG:
			iPos-- ;
			iPos = max (iPos, mMinimum) ;
			break ;
		}

		if (iPos != mCurrentValue)
		{
			GetClientRect (hWnd, &rcClient) ;
			Trackbar:getThumbDisplayPosition (pTrackbar, &ptPosThumb, mStyle, &rcClient) ;

			mCurrentValue = iPos ;

			SendNotify (NULL, hWnd, TRBN_THUMBPOSCHANGING) ;

			Trackbar:getThumbDisplayPosition (pTrackbar, &ptNewPosThumb, mStyle, &rcClient) ;

			if ((ptNewPosThumb.x != ptPosThumb.x) || (ptNewPosThumb.y != ptPosThumb.y))
				InvalidateRect (hWnd, NULL, TRUE) ;
		}
		return 0 ;
	}


	//����WM_KEYUP ��Ϣ
	LRESULT	 ctrlTrackbarKeyup (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return 0 ;
	}


	//����WM_LBUTTONDOWN ��Ϣ
	LRESULT	 ctrlTrackbarLbuttondown (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		DWORD			mStyle ;
		POINT			ptPosThumb ;	
		PCTRL_TRACKBAR	pTrackbar ;
		RECT			rect, rcClient ;
		POINT			ptCursor ;
		int				iPos ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0) ;
		mStyle	= (DWORD)GetWindowLong (hWnd, GWL_STYLE) ;

		if (pTrackbar->mOperating)
			return 0 ;

		GetClientRect (hWnd, &rcClient) ;
		Trackbar:getThumbDisplayPosition (pTrackbar, &ptPosThumb, mStyle, &rcClient) ;

		rect = rcClient ;

		if (mStyle & TBS_VERT)
		{
			rect.top	= ptPosThumb.y - 32 ;
			rect.bottom = ptPosThumb.y + mLayout.m_sizeThumb.cy + 32 ;
		}
		else
		{
			rect.left  = ptPosThumb.x - 32 ;
			rect.right = ptPosThumb.x + mLayout.m_sizeThumb.cx + 32 ;
		}


		ptCursor.x = (int)(short)LOWORD(lParam) ;  // horizontal position of cursor 
		ptCursor.y = (int)(short)HIWORD(lParam) ;  // vertical position of cursor 

#if 0
		if (!PtInRect (&rect, ptCursor))
			return 0 ;

		SetCapture (hWnd) ;

		pTrackbar->mOperating = TRUE ;
		pTrackbar->mPositionChange = FALSE ;
		pTrackbar->mCursorPosition   = ptCursor ;

		InvalidateRect (hWnd, NULL, TRUE) ;
#else
		SetCapture (hWnd) ;

		pTrackbar->mOperating = TRUE ;
		pTrackbar->mPositionChange = FALSE ;
		pTrackbar->mCursorPosition   = ptCursor ;

		iPos = ctrlTrackbarPosition (pTrackbar, ptCursor, mStyle, &rcClient) ;
		if (iPos == mCurrentValue)
			return 0 ;

		pTrackbar->mPositionChange = TRUE ;
		mCurrentValue    = iPos ;
		pTrackbar->mCursorPosition   = ptCursor ;

		SendNotify (NULL, hWnd, TRBN_THUMBPOSCHANGING) ;
		InvalidateRect (hWnd, NULL, TRUE) ;

#endif
		return 0 ;
	}


	//����WM_MOUSEMOVE ��Ϣ
	LRESULT	 ctrlTrackbarMousemove (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		DWORD			mStyle ;
		PCTRL_TRACKBAR	pTrackbar ;
		POINT			ptCursor ;
		RECT			rcClient ;
		int				iPos ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0) ;
		if (!pTrackbar->mOperating)
			return 0 ;

		pTrackbar  = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0) ;
		mStyle	   = (DWORD)GetWindowLong (hWnd, GWL_STYLE) ;

		ptCursor.x = (int)(short)LOWORD(lParam) ;  // horizontal position of cursor 
		ptCursor.y = (int)(short)HIWORD(lParam) ;  // vertical position of cursor 

		GetClientRect (hWnd, &rcClient) ;

		iPos = ctrlTrackbarPosition (pTrackbar, ptCursor, mStyle, &rcClient) ;
		if (iPos == mCurrentValue)
			return 0 ;

		pTrackbar->mPositionChange = TRUE ;
		mCurrentValue    = iPos ;
		pTrackbar->mCursorPosition   = ptCursor ;

		SendNotify (NULL, hWnd, TRBN_THUMBPOSCHANGING) ;
		InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}


	//����WM_LBUTTONUP ��Ϣ
	LRESULT	 ctrlTrackbarLbuttonup (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		DWORD			mStyle ;
		PCTRL_TRACKBAR	pTrackbar ;
		POINT			ptCursor ;
		RECT			rcClient ;
		int				iPos ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0) ;
		if (!pTrackbar->mOperating)
			return 0 ;

		pTrackbar->mOperating = FALSE ;
		ReleaseCapture () ;


		pTrackbar  = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0) ;
		mStyle	   = (DWORD)GetWindowLong (hWnd, GWL_STYLE) ;

		ptCursor.x = (int)(short)LOWORD(lParam) ;  // horizontal position of cursor 
		ptCursor.y = (int)(short)HIWORD(lParam) ;  // vertical position of cursor 

		GetClientRect (hWnd, &rcClient) ;

		iPos = ctrlTrackbarPosition (pTrackbar, ptCursor, mStyle, &rcClient) ;
		if ((iPos == mCurrentValue) && (!(pTrackbar->mPositionChange)))
			return 0 ;

		mCurrentValue  = iPos ;

		SendNotify (NULL, hWnd, NM_RELEASEDCAPTURE) ;
		InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}


	//TBM_SETRANGE
	LRESULT	 ctrlTrackbarTBMSetRange (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR pTrackbar ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;

		mMinimum = LOWORD (lParam) ;
		mMaximum = HIWORD (lParam) ;

		if (wParam)
			InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}


	//TBM_SETRANGEMAX  -- fRedraw
	LRESULT	 ctrlTrackbarTBMSetRangeMax (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR pTrackbar ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;
		mMaximum = (LONG) (lParam) ;

		if (wParam)
			InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}


	//TBM_SETRANGEMIN
	LRESULT	 ctrlTrackbarTBMSetRangeMin (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR pTrackbar ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;
		mMinimum = (LONG) (lParam) ;

		if (wParam)
			InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}


	//TBM_GETRANGEMAX:
	LRESULT	 ctrlTrackbarTBMGetRangeMax (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR pTrackbar ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;

		return mMaximum ;
	}


	//TBM_GETRANGEMIN
	LRESULT	 ctrlTrackbarTBMGetRangeMin (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR pTrackbar ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;

		return mMinimum ;
	}


	//TBM_SETPOS 
	LRESULT	 ctrlTrackbarTBMSetPos (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR	pTrackbar ;
		LONG			lPosition ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;

		lPosition = (LONG)lParam ;

		lPosition = max (lPosition, mMinimum) ;
		lPosition = min (lPosition, mMaximum) ;


		mCurrentValue = lPosition ;

		if (wParam)
			InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}


	//TBM_GETPOS 
	LRESULT	 ctrlTrackbarTBMGetPos (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR	pTrackbar ;

		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;

		return (LRESULT)(mCurrentValue) ;
	}


	//WM_LAYOUT
	LRESULT	 ctrlTrackbarLayout (HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PCTRL_TRACKBAR			pTrackbar ;
		PCTRL_TRACKBAR_LAYOUT	pTrackLayout ;

		pTrackLayout = (PCTRL_TRACKBAR_LAYOUT)lParam ;
		pTrackbar = (PCTRL_TRACKBAR)GetWindowLong (hWnd, 0)  ;

		mLayout.m_hInstance		  = pTrackLayout->m_hInstance ;
		mLayout.m_iImageBk		  = pTrackLayout->m_iImageBk ;
		mLayout.m_iImageThumb	  = pTrackLayout->m_iImageThumb ;
		mLayout.m_iImageTrackDone = pTrackLayout->m_iImageTrackDone ;
		mLayout.m_sizeThumb		  = pTrackLayout->m_sizeThumb ;
		mLayout.m_rcMarginTrack	  = pTrackLayout->m_rcMarginTrack ;

		if (wParam)
			InvalidateRect (hWnd, NULL, TRUE) ;

		return 0 ;
	}



	//Static Control ��Ϣ����ص����� 
	LRESULT CALLBACK ctrlTrackbarWndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{

		switch (Msg)
		{
		case WM_CREATE:
			return ctrlTrackbarCreate (hWnd, wParam, lParam) ;

		case WM_SETFOCUS:
			return 0 ;

		case WM_KILLFOCUS:
			return 0 ;

		case TBM_GETRANGEMAX:
			return ctrlTrackbarTBMGetRangeMax (hWnd, wParam, lParam) ;

		case TBM_GETRANGEMIN:
			return ctrlTrackbarTBMGetRangeMin (hWnd, wParam, lParam) ;

		case TBM_SETPOS:
			return ctrlTrackbarTBMSetPos (hWnd, wParam, lParam) ;

		case TBM_GETPOS:
			return ctrlTrackbarTBMGetPos (hWnd, wParam, lParam) ;

		case TBM_SETRANGE:
			return ctrlTrackbarTBMSetRange (hWnd, wParam, lParam) ;

		case TBM_SETRANGEMAX:
			return ctrlTrackbarTBMSetRangeMax (hWnd, wParam, lParam) ;

		case TBM_SETRANGEMIN:
			return ctrlTrackbarTBMSetRangeMin (hWnd, wParam, lParam) ;

		case WM_PAINT:
			return ctrlTrackbarPaint (hWnd, wParam, lParam) ;

		case WM_KEYDOWN:
			return ctrlTrackbarKeydown (hWnd, wParam, lParam) ;

		case WM_KEYUP:
			return ctrlTrackbarKeyup (hWnd, wParam, lParam) ;
			break ;

		case WM_LBUTTONDOWN:
			return ctrlTrackbarLbuttondown (hWnd, wParam, lParam) ;

		case WM_MOUSEMOVE:
			return ctrlTrackbarMousemove (hWnd, wParam, lParam) ;

		case WM_LBUTTONUP:
			return ctrlTrackbarLbuttonup (hWnd, wParam, lParam) ;

		case WM_LAYOUT:
			return ctrlTrackbarLayout (hWnd, wParam, lParam) ;

		case WM_DESTROY:
			return ctrlTrackbarDestory (hWnd, wParam, lParam) ;

		}

		return DefWindowProc (hWnd, Msg, wParam, lParam) ;
	}
#endif
}