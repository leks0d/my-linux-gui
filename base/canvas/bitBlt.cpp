
#include <mango.h>


namespace mango
{

#define BMP_BITBLT_32_BEGIN \
	for (cy = 0 ; cy < nHeight ; cy++)\
	{\
		for (cx = 0 ; cx < nWidth ; cx++)\
		{

#define BMP_BITBLT_32_END \
			pDestBuf++ ;\
			pSrcBuf++  ;\
		}\
		pDestBuf += iDestWidth - nWidth ;\
		pSrcBuf  += iSrcWidth  - nWidth ;\
	}


#define BMP_BITBLT_MASK_PIXEL_IF_VALID 
#define BMP_BITBLT_MASK_PIXEL_INCREMENT 



#define BMP_BITBLT_32_ALPHA_MIX {\
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
		*pDestBuf = ((*pDestBuf) & 0xff000000) | dwRed | dwBlue | dwGreen ;\
	}

	//COLOUR SPACES .17 ITU-R recommendation BT.601 建议
	//Y=0.299R + 0.587G+0.114B
	//简化为Y = 0.3R + 0.6G + 0.1B
#define BMP_BITBLT_32_CALC_LUM(clr) {\
		dwRed   = (clr & 0x00ff0000) >> 16 ;\
		dwGreen = (clr & 0x00ff0000) >> 16 ;\
		dwBlue  = (clr & 0x00ff0000) ;\
		dwLum   = 3 * dwRed / 10 +  dwGreen * 6 / 10 + dwBlue / 10; \
	}

	
	//内部使用, 传递位图块
	bool Canvas::bitBlt_32(void *pDestBits,	int iDestWidth, int nXDest,  int nYDest,  int nWidth,  int nHeight, \
			void *pSrcBits,  int iSrcWidth,  int nXSrc,   int nYSrc,  \
			DWORD dwRop, DWORD dwAlpha, Region* rgn)
	{
		int				cx, cy ;
		DWORD			*pDestBuf, *pSrcBuf ;
		DWORD			dwRed, dwBlue, dwGreen, dwLum ;
		DWORD			dwDest, dwSrc ;

		BOOL  bMask = FALSE ;
		int   iMaskByte, iMaskBit ;
		BYTE  *pbyMask, *pbyLineMask ;


		pDestBuf  = (DWORD *)pDestBits ;
		pSrcBuf   = (DWORD *)pSrcBits  ;

		pDestBuf += nYDest * iDestWidth + nXDest ;
		pSrcBuf  += nYSrc * iSrcWidth   + nXSrc  ;


		switch (dwRop)
		{
		case SRCCOPY:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				*pDestBuf = *pSrcBuf ;
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;

		case SRCALPHA:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				dwAlpha = (*pSrcBuf) & 0xff000000 ;
				if (dwAlpha == 0xff000000)
				{
					*pDestBuf = ((*pDestBuf) & 0xff000000) | (*pSrcBuf) ;
				}
				else if (dwAlpha)
				{
					dwAlpha = (dwAlpha >> 24) ;
					dwDest  = *pDestBuf ;
					dwSrc   = *pSrcBuf  ;

					BMP_BITBLT_32_ALPHA_MIX 
				}
			}
			//else
			//{
			//	*pDestBuf = 0xff00ff00 ;
			//}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;

		case SRCDIMALPHA:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				dwAlpha = (*pSrcBuf) & 0xff000000 ;
				if (dwAlpha == 0xff000000)
				{
					*pDestBuf = ((*pSrcBuf) & 0xff000000) | (((*pSrcBuf) >> 1) & 0x007f7f7f) ;
				}
				else if (dwAlpha)
				{
					dwAlpha = (dwAlpha >> 24) ;
					dwDest  = *pDestBuf ;
					dwSrc   = ((*pSrcBuf) >> 1) & 0x007f7f7f  ;

					BMP_BITBLT_32_ALPHA_MIX 
				}
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;

		case SRCDIM:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				*pDestBuf = ((*pSrcBuf) & 0xff000000) | (((*pSrcBuf) >> 1) & 0x007f7f7f) ;
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;

		case SRCGRAY:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				dwSrc = *pSrcBuf ;
				BMP_BITBLT_32_CALC_LUM (dwSrc) 
					*pDestBuf = (dwSrc & 0xff000000) | (dwLum << 16)  | (dwLum << 8) | dwLum ;
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;

		case SRCGRAYALPHA:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				dwSrc = *pSrcBuf ;
				BMP_BITBLT_32_CALC_LUM (dwSrc)
					dwSrc  = (dwSrc & 0xff000000) | (dwLum << 16)  | (dwLum << 8) | dwLum ;
				dwDest = *pDestBuf ;
				BMP_BITBLT_32_ALPHA_MIX
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;


		case DSTTRANSP:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				dwDest = *pDestBuf ;
				dwSrc  = *pSrcBuf  ;
				BMP_BITBLT_32_ALPHA_MIX 
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;


		case DSTTRANSPMID:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				*pDestBuf = (*pDestBuf) - (((*pDestBuf) >> 1) & 0x007f7f7f) +  (((*pSrcBuf) >> 1) & 0x007f7f7f) ;
			}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;
	

		case SRCALPHADSTALPHA:
			BMP_BITBLT_32_BEGIN
			BMP_BITBLT_MASK_PIXEL_IF_VALID
			{
				dwAlpha = (*pSrcBuf) & 0xff000000 ;
				if (dwAlpha == 0xff000000)
				{
					*pDestBuf = (*pSrcBuf) ;
				}
				else if (dwAlpha)
				{
					//计算新目的透明度  255 - (255 - 当前图片透明) * ((255 - 目的透明度) / 255)
					//混合运算的透明度  255 目的透明度
					dwAlpha  = (dwAlpha >> 24) ;
					dwDest   = ((*pDestBuf) >> 24) & 0x00ff ;
					dwDest   = 255 - (255 - dwAlpha) * (255 - dwDest) / 255 ;
					*pDestBuf = (*pDestBuf & 0x00ffffff) | (dwDest << 24) ;
					if (dwDest)
						dwAlpha = dwAlpha * 255 / dwDest ;
					if (dwAlpha > 255)
						dwAlpha = 255 ;

					dwDest  = *pDestBuf ;
					dwSrc   = *pSrcBuf  ;

					BMP_BITBLT_32_ALPHA_MIX 
				}
			}
			//else
			//{
			//	*pDestBuf = 0xff00ff00 ;
			//}
			BMP_BITBLT_MASK_PIXEL_INCREMENT 
			BMP_BITBLT_32_END
			break ;
		}

		return true;
	}
}