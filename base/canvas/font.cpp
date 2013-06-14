#include <mango.h>


namespace mango
{


	Font::Font()
	{
		mFontSize    = gSessionLocal.mFontSize[2];
		mWidthBuffer = gSessionLocal.mFontWidthBuffer[2];
	}


	Font::~Font()
	{


	}

	int Font::setSize(int size)
	{
		int i;

		for (i = 0 ; i < STOCK_FONT_OBJECT_NUM; i++)
		{
			if (size < gSessionLocal.mFontSize[i])
				break;
		}

		if (i > 0) 
			i--;

		mFontSize    = gSessionLocal.mFontSize[i];
		mWidthBuffer = gSessionLocal.mFontWidthBuffer[i];

		return 0;
	}


	int Font::getWidth (WCHAR wchar)
	{
		int		 width   ; 

		if (!mWidthBuffer)
			return 0 ;

		width = (int)(WORD)mWidthBuffer[(DWORD)(WORD)wchar] ;
		if (wchar == '\t')
		{
			width = (int)(WORD)(mWidthBuffer[FONT_WIDTH_BUFF_BYTES - 2]) ;
			width = (width >> 2) * 4 ;	
		}
		else if (width == 0)
		{
			width = (int)(WORD)(mWidthBuffer[FONT_WIDTH_BUFF_BYTES - 2]) ;
			width = width >> 2 ;	
		}

		return width ;
	}

	int Font::getHeight(void)
	{
		return mFontSize;
	}


	int Font::getMaxWidth(void)
	{
		if (!mWidthBuffer)
			return 0 ;

		return (int)(WORD)(mWidthBuffer[FONT_WIDTH_BUFF_BYTES - 4]) ;
	}


	int Font::getMaxHeight(void)
	{
		if (!mWidthBuffer)
			return 0 ;

		return (int)(WORD)(mWidthBuffer[FONT_WIDTH_BUFF_BYTES - 3]) ;
	}



	unsigned char *Font::getCharBmp(WCHAR wchar, Size *size, int *dyExtra)
	{
		unsigned char *bits;
		SIZE   bmpSize;


		if (wchar <= 256)
		{
			ft_ex_iTrueTypeSetSize (0, mFontSize);
			bits = ft_ex_TrueTypeGetCharBmp(0, wchar, &bmpSize, dyExtra);
		}
		else
		{
			ft_ex_iTrueTypeSetSize (1, mFontSize);
			bits = ft_ex_TrueTypeGetCharBmp(1, wchar, &bmpSize, dyExtra);
		}

		size->cx = bmpSize.cx;
		size->cy = bmpSize.cy;

		return bits;
	}









	//算法 最久未使用页面替换算法
	#define FONT_CACHE_CHARS	65536

	//cahche 条目
	typedef struct tagFONT_CACHE_ENTRY
	{
		LIST_HEAD	m_list ;

		WCHAR	m_wCode		;	//字符内码
		WORD	m_wReserve	;
		BYTE	m_FontSize ;
		BYTE	m_Width	   ;
		BYTE	m_Height   ;
		char	m_dyExtra  ;

		BYTE  m_Data[4]  ;

	} FONT_CACHE_ENTRY, *PFONT_CACHE_ENTRY ;

	//cache header
	typedef struct tagFONT_CACHE_HEADER
	{
		LIST_HEAD			m_listEntry ; //

		PFONT_CACHE_ENTRY	m_pbEntry ; //条目数开始地址
		int					m_iTotalEntries ;	 //总条目数
		int					m_iBytesOfEntry ;	 //条目占的字节数

		int					m_iMaxFontSize ;
		WORD				m_wCharEntryMap[FONT_CACHE_CHARS] ;

	} FONT_CACHE_HEADER, *PFONT_CACHE_HEADER ;

	//本地字体Cache
	typedef struct tagFONT_CACHE_LOCAL
	{
		void	*m_pCahceAddr		;
		int		 m_iBytesOfBmpBuf	;
		BYTE	 m_CharBmpBuf[1]	;

	} FONT_CACHE_LOCAL, *PFONT_CACHE_LOCAL ;


	FontCache::FontCache()
	{

	}


	FontCache::~FontCache()
	{


	}

#if 0


	//字体Cache 全局初始化
	BOOL turglFontCacheSessionInit (int iMaxFontSize, void *pMemoryAddr, int iMemoryBytes)
	{
		PFONT_CACHE_HEADER	pHeader ;
		PFONT_CACHE_ENTRY	pEntry  ;	
		int					i ;

		if (pMemoryAddr == NULL)
			return FALSE ;

		if (iMemoryBytes < sizeof (FONT_CACHE_HEADER))
		{
			log_e ("memory is too small \n") ;
			return FALSE ;
		}

		pHeader = (PFONT_CACHE_HEADER)pMemoryAddr ;
		memset (pHeader, 0, sizeof (FONT_CACHE_HEADER)) ;

		pHeader->m_iBytesOfEntry = (sizeof (FONT_CACHE_ENTRY) - 4 + iMaxFontSize * iMaxFontSize + 3) & (~0x03);
		pHeader->m_iTotalEntries = (iMemoryBytes - sizeof (FONT_CACHE_HEADER)) / pHeader->m_iBytesOfEntry ;
		pHeader->m_pbEntry		 = (PFONT_CACHE_ENTRY)((PFONT_CACHE_HEADER)pMemoryAddr + 1) ;

		pHeader->m_iTotalEntries = min (pHeader->m_iTotalEntries, FONT_CACHE_CHARS - 1) ;

		pEntry = pHeader->m_pbEntry ;

		INIT_LIST_HEAD (&(pHeader->m_listEntry)) ;

		for (i = 0 ; i < pHeader->m_iTotalEntries ; i++)
		{
			list_add_tail (&(pEntry->m_list), &(pHeader->m_listEntry)) ;
			pEntry->m_wCode = 0 ;
			pEntry = (BYTE *)pEntry + (pHeader->m_iBytesOfEntry) ;
		}

		return TRUE ;
	}

	//字体Cache 本地初始化
	PFONT_CACHE_LOCAL turglFontCacheLocalInit (int iMaxFontSize, void *pFontCacheAddr)
	{
		PFONT_CACHE_LOCAL pLocal ;

		pLocal = lcMalloc (sizeof (FONT_CACHE_LOCAL) + iMaxFontSize * iMaxFontSize) ;

		if (pLocal == NULL)
		{
			log ("can't alloc memory \n") ;
			return NULL ;
		}

		pLocal->m_pCahceAddr = pFontCacheAddr ;
		pLocal->m_iBytesOfBmpBuf = iMaxFontSize * iMaxFontSize ;

		return pLocal ;
	}

	//字体Cache 本地反初始化
	BOOL turglFontCacheLocalDeinit (PFONT_CACHE_LOCAL *ppLocal)
	{
		if (*ppLocal)
		{
			lcFree (*ppLocal) ;
			*ppLocal = NULL ;
		}

		return TRUE ;
	}


	//从字体Cache 获取字体位图
	BYTE *turglFontCacheGetCharBmp (PFONT_CACHE_LOCAL pLocal, int iFontSize, WCHAR wchar, SIZE *psize, int *pdyExtra)
	{
		PFONT_CACHE_HEADER	pHeader ;
		PFONT_CACHE_ENTRY	pEntry  ;	
		int					iEntry ;
		BYTE				*pbyBmp = NULL ;
		int					iBytes ;

		if (pLocal == NULL)
			return NULL ;

		pHeader = (PFONT_CACHE_HEADER)(pLocal->m_pCahceAddr) ;

		ENTER_FONTCACHE_CRITICAL_SECTION ;

		do
		{
			iEntry  = (int)(DWORD)(pHeader->m_wCharEntryMap[(DWORD)(WORD)wchar]) ;
			if (iEntry >= pHeader->m_iTotalEntries)
				break ;

			pEntry = pHeader->m_pbEntry + iEntry ;
			if (pEntry->m_wCode != wchar)
				break ;

			if (pEntry->m_FontSize != iFontSize)
				break ;

			iBytes = (WORD)pEntry->m_Width * (WORD)pEntry->m_Height ;
			if (iBytes > pLocal->m_iBytesOfBmpBuf)
				break ;

			memcpy (pLocal->m_CharBmpBuf, pEntry->m_Data, iBytes) ;
			pbyBmp = pLocal->m_CharBmpBuf ;

			psize->cx = (int)(WORD)pEntry->m_Width ; 
			psize->cy = (int)(WORD)pEntry->m_Height ;
			*pdyExtra = (int)pEntry->m_dyExtra ;

			list_del (&(pEntry->m_list)) ;
			list_add_tail (&(pEntry->m_list), &(pHeader->m_listEntry)) ;

		}while (0) ;

		LEAVE_FONTCACHE_CRITICAL_SECTION ;

		return pbyBmp ;
	}



	//将字体位图存到字体Cache
	void turglFontCacheSetCharBmp (PFONT_CACHE_LOCAL pLocal, int iFontSize, WCHAR wchar, SIZE size, int dyExtra, BYTE	*pbyBmp)
	{
		PFONT_CACHE_HEADER	pHeader ;
		PFONT_CACHE_ENTRY	pEntry  ;	
		int					iEntry ;
		int					iBytes ;
		int					iIndex ;

		if ((pLocal == NULL) || (pbyBmp == NULL))
			return ;

		pHeader = (PFONT_CACHE_HEADER)(pLocal->m_pCahceAddr) ;
		iBytes  = size.cx * size.cy ;
		if (iBytes > (pHeader->m_iBytesOfEntry - (int)(sizeof (FONT_CACHE_ENTRY) - 4)))
			return ;

	//	ENTER_FONTCACHE_CRITICAL_SECTION ;

		iIndex  = (int)(DWORD)wchar ;
		iEntry  = (int)(DWORD)(pHeader->m_wCharEntryMap[iIndex]) ;
		if (iEntry < pHeader->m_iTotalEntries)
		{
			pEntry = pHeader->m_pbEntry + iEntry ;
			if (pEntry->m_wCode != wchar)
				pEntry = NULL ;
		}
		else
			pEntry = NULL ;

		if (pEntry == NULL)
			pEntry = list_entry((pHeader->m_listEntry.next), FONT_CACHE_ENTRY, m_list) ; 


		list_del (&(pEntry->m_list)) ;
		list_add_tail (&(pEntry->m_list), &(pHeader->m_listEntry)) ;

		pEntry->m_wCode	   = wchar ;
		pEntry->m_Width    = (BYTE)size.cx ;
		pEntry->m_Height   = (BYTE)size.cy ;
		pEntry->m_dyExtra  = dyExtra ;
		pEntry->m_FontSize = (BYTE)iFontSize ;

		memcpy (pEntry->m_Data, pbyBmp, iBytes) ;

		pHeader->m_wCharEntryMap[iIndex] = (pEntry - pHeader->m_pbEntry) ;

	//	LEAVE_FONTCACHE_CRITICAL_SECTION ;
	}
#endif
}