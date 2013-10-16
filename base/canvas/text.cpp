

#include <mango.h>


namespace mango
{

#define TEXT_BITBLT_8_TO_32_ALPHA_MIX {\
	dwRed	 = (dwDest & 0x00ff0000) * (255 - dwAlpha) + (dwSrc & 0x00ff0000) * dwAlpha ;\
	dwRed    = dwRed / 255 ;\
	dwRed   &= 0x00ff0000 ;\
	\
	dwGreen  = (dwDest & 0x0000ff00) * (255 - dwAlpha) + (dwSrc & 0x0000ff00) * dwAlpha ;\
	dwGreen  = dwGreen / 255 ;\
	dwGreen &= 0x0000ff00 ;\
	\
	dwBlue   = (dwDest & 0x000000ff) * (255 - dwAlpha) + (dwSrc & 0x000000ff) * dwAlpha ;\
	dwBlue   = dwBlue / 255 ;\
	dwBlue  &= 0x000000ff ;\
	\
	*pBmpBuf =  ((*pBmpBuf) & 0xff000000) | dwRed | dwBlue | dwGreen ;\
}


#define TEXT_BITBLT_MASK_PIXEL_IF_VALID \
	if(1)

#define TEXT_BITBLT_MASK_PIXEL_INCREMENT

#define TEXT_BITBLT_MASK_LINE_CALC



	//将 8 alpha 文件本点阵图显示到32 位图中
	BOOL TextBitBlt_8_To_32 (void *pBmpBits,	int iBmpWidth, \
		int nXBmp,  int nYBmp,  int nWidth,  int nHeight, \
		void *pTextBits, int iTextWidth,  int nXText,  int nYText,  \
		int  iBkMode, COLORREF clrFore, COLORREF clrBack)
	{

		DWORD	 *pBmpBuf    = (DWORD *)pBmpBits ;
		BYTE	 *pbyTextBuf = (BYTE *)pTextBits ; 
		int		 cx ;
		DWORD	 dwAlpha, dwBkAlpha ;

		DWORD	dwRed,  dwBlue, dwGreen ;
		DWORD	dwDest, dwSrc ;

		BOOL  bMask = FALSE ;

	
		// 定位 y 行
		pBmpBuf    += iBmpWidth  * nYBmp  ;
		pbyTextBuf += iTextWidth * nYText ;


		switch (iBkMode)
		{
		case TRANSPARENT:
			for ( ; nHeight > 0 ; nHeight--)
			{
				pBmpBuf    += nXBmp ;
				pbyTextBuf += nXText ;
				cx		    = nWidth ;

				TEXT_BITBLT_MASK_LINE_CALC

					while (cx)
					{
						TEXT_BITBLT_MASK_PIXEL_IF_VALID
						{
							dwAlpha = (DWORD)(WORD)(*pbyTextBuf) ;

							if (dwAlpha >= 255)
								*pBmpBuf = clrFore | 0xff000000 ;
							else if (dwAlpha)
							{
								//计算新目的透明度  255 - (255 - 当前图片透明) * ((255 - 目的透明度) / 255)
								//混合运算的透明度  255 目的透明度
								//dwAlpha  = (dwAlpha >> 24) ;
								dwDest   = ((*pBmpBuf) >> 24) & 0x00ff ;
								dwDest   = 255 - (255 - dwAlpha) * (255 - dwDest) / 255 ;

								//dwDest = 255 ;
								*pBmpBuf = (*pBmpBuf & 0x00ffffff) | (dwDest << 24) ;
								if (dwDest)
									dwAlpha = dwAlpha * 255 / dwDest ;
								if (dwAlpha > 255)
									dwAlpha = 255 ;

								dwDest = *pBmpBuf ; 
								dwSrc  = clrFore ;
								TEXT_BITBLT_8_TO_32_ALPHA_MIX 
							}
						}

						pBmpBuf++ ;
						cx-- ;
						pbyTextBuf++ ;

						TEXT_BITBLT_MASK_PIXEL_INCREMENT
					}

					pBmpBuf	   += iBmpWidth  - (nXBmp  + nWidth) ;
					pbyTextBuf += iTextWidth - (nXText + nWidth) ;

			}
			break ;

		case OPAQUE:
			dwBkAlpha = clrBack & 0xff000000 ;
			for ( ; nHeight > 0 ; nHeight--)
			{
				pBmpBuf    += nXBmp ;
				pbyTextBuf += nXText ;
				cx		    = nWidth ;

				TEXT_BITBLT_MASK_LINE_CALC

					while (cx)
					{
						TEXT_BITBLT_MASK_PIXEL_IF_VALID
						{
							dwAlpha = (DWORD)(WORD)(*pbyTextBuf) ;

							if (dwAlpha >= 255)
								*pBmpBuf = (clrFore & 0x00ffffff) | dwBkAlpha;
							else if (dwAlpha)
							{
								dwDest	 = clrBack ;
								dwSrc	 = clrFore ;
								TEXT_BITBLT_8_TO_32_ALPHA_MIX 
								*pBmpBuf = (*pBmpBuf & 0x00ffffff) | dwBkAlpha ;
							}
							else
								*pBmpBuf = clrBack ;
						}

						pBmpBuf++ ;
						cx-- ;
						pbyTextBuf++ ;

						TEXT_BITBLT_MASK_PIXEL_INCREMENT
					}

					pBmpBuf	   += iBmpWidth  - (nXBmp  + nWidth) ;
					pbyTextBuf += iTextWidth - (nXText + nWidth) ;


			}
			break ;

		case OPAQUE_ALPHA:
			for ( ; nHeight > 0 ; nHeight--)
			{
				pBmpBuf    += nXBmp ;
				pbyTextBuf += nXText ;
				cx		    = nWidth ;

				TEXT_BITBLT_MASK_LINE_CALC

					while (cx)
					{
						TEXT_BITBLT_MASK_PIXEL_IF_VALID
						{
							dwAlpha = (DWORD)(WORD)(*pbyTextBuf) ;
							*pBmpBuf = clrFore | (dwAlpha << 25) ;
						}

						pBmpBuf++ ;
						cx-- ;
						pbyTextBuf++ ;

						TEXT_BITBLT_MASK_PIXEL_INCREMENT
					}

					pBmpBuf	   += iBmpWidth  - (nXBmp  + nWidth) ;
					pbyTextBuf += iTextWidth - (nXText + nWidth) ;


			}
			break ;

		case SHADOW:
			for ( ; nHeight > 0 ; nHeight--)
			{
				pBmpBuf    += nXBmp ;
				pbyTextBuf += nXText ;
				cx		    = nWidth ;

				TEXT_BITBLT_MASK_LINE_CALC

					while (cx)
					{
						TEXT_BITBLT_MASK_PIXEL_IF_VALID
						{

							dwAlpha = (DWORD)(WORD)(*pbyTextBuf) ;
							if (dwAlpha)
							{
								dwSrc = *pBmpBuf ;
								dwSrc = (dwSrc >> 1) & 0x007f7f7f ;

								if (dwAlpha >= 255)
									*pBmpBuf = dwSrc ;
								else 
								{
									dwDest	 = *pBmpBuf ;
									TEXT_BITBLT_8_TO_32_ALPHA_MIX 
								}
							}
						}

						pBmpBuf++ ;
						cx-- ;
						pbyTextBuf++ ;

						TEXT_BITBLT_MASK_PIXEL_INCREMENT
					}

					pBmpBuf	   += iBmpWidth  - (nXBmp  + nWidth) ;
					pbyTextBuf += iTextWidth - (nXText + nWidth) ;

			}
			break ;

		}

		return TRUE ;
	}

static int isChatTh(WCHAR	 wChar){
	int ret = 0;
	if((wChar>=0x0E34&&wChar<=0x0E3A)||(wChar>=0x0E47&&wChar<=0x0E4E)||(wChar==0x0E31)){
		ret = 1; 
	}else if(wChar==0x0E33){
		ret = 2;
	}
	return ret;
}

	int Canvas::drawSingleLineText(Rect& clipRect, int x, int y, const TCHAR* lpString, int cbString)
	{
		Rect			rect, rcChar ;
		Size			size	;
		int				dxExtra, dyExtra ;
		WCHAR			wChar	;
		LPBYTE			pbyCharBits ;
		int				iCharWidth ;
		int 			oCharWidth ;

		if ((!lpString) || ((cbString <= 0) && (cbString != -1)))
			return x ;

		do
		{
			wChar = *lpString ;
			lpString++ ;

			if ((wChar == 0) || (wChar == 0x0a) || (wChar == 0x0b))
				break ;

			if (cbString == 0)
				break ;

			cbString = cbString > 0 ? cbString - 1 : cbString ; 
			
			pbyCharBits = mFont->getCharBmp(wChar, &size, &dyExtra) ;
			iCharWidth = mFont->getWidth(wChar) ;

			if (pbyCharBits == NULL)
			{
				x += iCharWidth ;
				continue ;
			}

			dxExtra = (iCharWidth - size.cx) / 2 ;

			rcChar.setEx(x + dxExtra, y + dyExtra, size.cx, size.cy);
			
			if(isChatTh(wChar) == 1)
				rect.setEx(x + dxExtra - oCharWidth, y + dyExtra, size.cx, size.cy);
			else if(isChatTh(wChar) == 2)
				rect.setEx(x + dxExtra - oCharWidth, y + dyExtra, size.cx, size.cy);
			else
				rect.setEx(x + dxExtra, y + dyExtra, size.cx, size.cy);
			
			if (rect.intersect(clipRect))
			{
				TextBitBlt_8_To_32(mBitmap->getBits(), mBitmap->getWidth(), \
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, \
					pbyCharBits, size.cx, rect.left - rcChar.left, rect.top - rcChar.top, \
					mBkMode, mTextColor, mBkColor);
			}
			
			if(isChatTh(wChar) == 1)
				x;
			else if(isChatTh(wChar) == 2)
				x += size.cx/2 + mCharExtra;
			else
				x += iCharWidth + mCharExtra;
			
			oCharWidth = iCharWidth;
			
			if (x > clipRect.right)
				break ;
			
		} while (1) ;

		return x ;
	} 


	//计算RECT 中行数及每一行字符数
	int Canvas::getMultiLinesTextDrawParam (const TCHAR* lpString, int cbString, \
		int cxView, const TCHAR **lpptStartPerLine, int *piCharsPerLine, int *piWidthPerline, int iMaxLines, Size* lpSize)
	{
		BOOL  bSuccess = FALSE ;
		int	  iLines = 0 ;
		int	  iCharsPerLine, cxChar ;
		int	  cx ;


		iCharsPerLine = 0 ;
		cx			  = 0 ;

		if (lpptStartPerLine)
			*lpptStartPerLine++ = lpString ;

		while (cbString && *lpString)
		{
			cxChar   = mFont->getWidth(*lpString) ;
			cxChar  += mCharExtra ;

			cbString =  (cbString > 0) ? cbString - 1 : cbString ;

			if (*lpString == '\r')
			{

			}
			else if (*lpString == '\n')
			{
				if (iLines >= iMaxLines - 1)
					break ;

				iLines++ ;

				if (piCharsPerLine)
					*piCharsPerLine++   = iCharsPerLine ;

				if (piWidthPerline)
					*piWidthPerline++   = cx ;

				if (lpptStartPerLine)
				{
					*lpptStartPerLine++ = lpString + 1 ;
				}

				cx			  = 0 ;
				iCharsPerLine = 0 ;
			}
			else if ((cx + cxChar) > cxView)
			{
				if (iLines >= iMaxLines - 1)
					break ;

				iLines++ ;

				if (piCharsPerLine)
					*piCharsPerLine++   = iCharsPerLine ;

				if (piWidthPerline)
					*piWidthPerline++   = cx ;

				if (lpptStartPerLine)
					*lpptStartPerLine++ = lpString ;

				cx			  = cxChar ;
				iCharsPerLine = 1 ;
			}
			else
			{
				iCharsPerLine++  ;
				cx += cxChar	;
			}

			lpString++ ;
		}

		if (piCharsPerLine)
			*piCharsPerLine++ = iCharsPerLine ;

		if (piWidthPerline)
			*piWidthPerline++   = cx ;

		iLines++ ;

		lpSize->cy = iLines * mFont->getMaxHeight();
		lpSize->cx = iLines > 1 ? cxView : cx ;

		return iLines ;
	}




	#define DRAWTEXT_MAX_LINES   10


	void Canvas::drawText(const TCHAR *string, int count, Rect &rect, int format)
	{
		Size	size ;
		Rect	rcDcClip, rcClip, rcText, rcFill ;
		int		iLine,  iCharPerLine [DRAWTEXT_MAX_LINES + 1] ;
		int		iWidthPerLine [DRAWTEXT_MAX_LINES + 1] ;
		const TCHAR*	lptStartPerLine[DRAWTEXT_MAX_LINES + 1] ;

		int		iLineCount ;
		int		iLineSpace ;
		int		iWidth, iHeight ;
		int		iLeft ;
		BOOL	bEllipsis = FALSE ;

		if (string == NULL)
			return;

		if (!getClipRect(rcDcClip))
			return;

		iWidth  = rect.right - rect.left ;
		iHeight = rect.bottom - rect.top ;

		if (!(format & DT_SINGLELINE))
		{
			iLineCount = getMultiLinesTextDrawParam (string, count, \
				iWidth, lptStartPerLine, iCharPerLine, iWidthPerLine, DRAWTEXT_MAX_LINES, &size) ;

			if ((size.cy > iHeight) && iLineCount)
			{
				iLineSpace = (size.cy / iLineCount) ;
				iLineCount = iHeight / iLineSpace ;
				size.cy	   = iLineCount * iLineSpace ;

				if (format & DT_END_ELLIPSIS)
					bEllipsis = TRUE ;
			}
		}
		else
		{
			getTextExtentPoint(string, count, size);

			iLineCount			= 1 ;
			lptStartPerLine[0]	= string ;
			iWidthPerLine[0]	= size.cx ; 

			if (count >= 0)
				iCharPerLine[0]	= count ;
			else
				iCharPerLine[0]	= String::lstrlen(string) ;

			if ((format & DT_END_ELLIPSIS) && (size.cx > iWidth))
				bEllipsis = TRUE ;
		}

		rcText = rect ;
		viewToCanvas(rcText);
		if (size.cx < iWidth)
		{
			//需要 针对每一行进行处理
			//if (uFormat & DT_CENTER)
			//	rcText.left += (iWidth - size.cx) / 2 ;
			//else if (uFormat & DT_RIGHT)
			//	rcText.left  = rcText.right  - size.cx ;
		}

		if (size.cy < iHeight)
		{
			if (format & DT_VCENTER)
				rcText.top += (iHeight - size.cy) / 2 ;
			else if (format & DT_BOTTOM)
				rcText.top  = rcText.bottom  - size.cy ;
		}

		rcClip = rcDcClip;
		if (!rcClip.intersect(rcText))
			return;

		if (mBkMode == OPAQUE)
		{
			rcFill = rcText ;
			rcFill.right = rcText.left + size.cx ;
			rcFill.bottom = rcText.top + size.cy ;
			fillRectColor(rcDcClip, rcFill, mBkColor) ;
		}

		for (iLine = 0 ; iLine < iLineCount ; iLine++)
		{
			if (format & DT_CENTER)
				iLeft = rcText.left + (iWidth - iWidthPerLine[iLine]) / 2 ;
			else if (format & DT_RIGHT)
				iLeft  = rcText.right  - iWidthPerLine[iLine] ;
			else
				iLeft = rcText.left ;

			if (bEllipsis && (iLine == iLineCount - 1))
			{
				Size	sizeLine ;
				int cxDot  = mFont->getWidth('.') + mCharExtra ;
				int cchText ;
				int x ;

				sizeLine.cy = mFont->getMaxHeight();
				sizeLine.cx = iWidth - 3 * cxDot;
				cchText		= iCharPerLine[iLine];
				if (!checkTextExtentPoint(lptStartPerLine[iLine], &cchText, sizeLine))
					break ;

				x = drawSingleLineText(rcClip, rcText.left, rcText.top, lptStartPerLine[iLine], cchText) ;
				drawSingleLineText(rcClip, x, rcText.top, TEXT("..."), 3) ;
			}
			else
			{
				drawSingleLineText (rcClip, iLeft, rcText.top, lptStartPerLine[iLine], iCharPerLine[iLine]) ;
			}

			rcText.top += size.cy / iLineCount ;

			if ((rcText.top + size.cy / iLineCount) > rcText.bottom)
				break ;

		}
	}


	void Canvas::drawText(const char *string, int count, Rect &rect, int format)
	{
		WCHAR  wcBuf[81] ;
		int    i ;
		if(string == NULL)
			return;
#if 0
		for (i = 0 ; i < 80 && (*string) && count ; i++, count--) {
			wcBuf[i] =  *string ;
			string++ ;
		}
		wcBuf[i] = '\0';
#else
		if(Charset::isTextUtf8(string)){
			i = Charset::multiByteToWideChar(CP_UTF8, string, strlen(string), wcBuf, 80);
		}else{
			i = Charset::multiByteToWideChar(CP_ACP, string, strlen(string), wcBuf, 80);
		}
#endif

		return drawText(wcBuf, i, rect, format);
	}


	void Canvas::drawText(const TCHAR *string, int count, int x, int y)
	{
		Rect	clipRect;

		if (!getClipRect(clipRect))
			return;

		viewToCanvas(x, y);
		drawSingleLineText(clipRect, x, y, string, count);
	}


	void Canvas::drawText(const char *string, int count, int x, int y)
	{
		WCHAR  wcBuf[81] ;
		int    i ;
		if(string == NULL)
			return;
#if 0
		for (i = 0 ; i < 80 && (*string) && count ; i++, count--) {
			wcBuf[i] =  *string ;
			string++ ;
		}
		wcBuf[i] = '\0' ;
#else
		if(Charset::isTextUtf8(string)){
			i = Charset::multiByteToWideChar(CP_UTF8, string, strlen(string), wcBuf, 80);
		}else{
			i = Charset::multiByteToWideChar(CP_ACP, string, strlen(string), wcBuf, 80);
		}
#endif
		return drawText(wcBuf, i, x, y);
	}
	void Canvas::charToWCHAR(char *string,WCHAR *des){
		WCHAR  wcBuf[81] ;
		int    i,count;
		
		count = strlen(string);

		for (i = 0 ; i < 80 && (*string) && count ; i++, count--) {
			wcBuf[i] =  *string ;
			string++ ;
		}
		wcBuf[i] = '\0' ;
		memcpy(des,wcBuf,81*sizeof(WCHAR));

	}

	int Canvas::getTextCharacterExtra(void)
	{
		return mCharExtra ;
	}


	int  Canvas::setTextCharacterExtra(int charExtra)
	{
		int	prevCharExtra;

		prevCharExtra = mCharExtra;
		mCharExtra    = charExtra;

		return mCharExtra ;
	}

	//得到指定长度的字符宽度
	bool Canvas::getTextExtentPoint(const TCHAR* string,int count, Size& size)
	{
		int	cx = 0 ;

		while (count && *string)
		{
			cx += mFont->getWidth(*string) + mCharExtra;

			string++ ;
			count =  (count > 0) ? count - 1 : count;
		}

		size.cx = cx;
		size.cy = mFont->getMaxHeight();

		return true;
	}



	//The function computes the numbers of chars to  display int the specified size area. 
	bool Canvas::checkTextExtentPoint (const TCHAR* string, int *count, Size& size)
	{
		Size        szDraw ;
		int         cbString, cbDraw ;

		szDraw.cx = 0 ;
		szDraw.cy = 0 ;
		cbString  = *count ;
		cbDraw    = 0 ;

		*count = 0 ;


		if (cbString == -1)
			cbString = String::lstrlen (string);


		while (cbDraw < cbString)
		{
			szDraw.cx += mFont->getWidth(*string) + mCharExtra;
			if (szDraw.cx < size.cx)
				cbDraw++ ;
			else
				break ;

			string++ ;
		}

		*count = cbDraw ;
		return true;
	}



#undef  RESOURCE_STRING_MAX_CHARS
#define RESOURCE_STRING_MAX_CHARS  256 


	void Canvas::drawTextResource(int id, int x, int y)
	{
		WCHAR	wStrBuf[RESOURCE_TEXT_MAX_CHARS + 1] ;

		if (gSessionLocal.mResource.loadString(id, wStrBuf, RESOURCE_STRING_MAX_CHARS + 1) > 0,mLanguageId)
			drawText((WCHAR*)wStrBuf, -1, x, y);

	}


	void Canvas::drawTextResource(int id, Rect &rect, int format)
	{
		WCHAR	wStrBuf[RESOURCE_TEXT_MAX_CHARS + 1] ;

		if (gSessionLocal.mResource.loadString(id, wStrBuf, RESOURCE_STRING_MAX_CHARS + 1) > 0,mLanguageId)
			drawText((WCHAR*)wStrBuf, -1, rect, format);
	}


	COLORREF Canvas::setTextColor(COLORREF color)
	{
		COLORREF oldColor = mTextColor;
		mTextColor = color;
		return oldColor;
	}


	COLORREF Canvas::getTextColor()
	{
		return mTextColor;
	}

	void Canvas::setTextSize(int size)
	{
		mFont->setSize(size);
	}

	void Canvas::setTextBackColor(COLORREF color)
	{
		mBkColor = color;
	}

	void Canvas::setTextLanguage(int lanId){
		mLanguageId = lanId;
	}
	
	int Canvas::getTextLanguage(){
		return mLanguageId;
	}

}


