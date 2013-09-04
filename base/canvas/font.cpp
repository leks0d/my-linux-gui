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
		SIZE   bmpSize = {0, 0};

		if (gFontCache.getCharBmp(mFontSize, wchar, size, dyExtra, &bits))
			return bits;

		//if (wchar <= 256)
		//{
			ft_ex_iTrueTypeSetSize (0, mFontSize);
			bits = ft_ex_TrueTypeGetCharBmp(0, wchar, &bmpSize, dyExtra);
		//}
		//else
		//{
		//	ft_ex_iTrueTypeSetSize (1, mFontSize);
		//	bits = ft_ex_TrueTypeGetCharBmp(1, wchar, &bmpSize, dyExtra);
		//}

		size->cx = bmpSize.cx;
		size->cy = bmpSize.cy;

		gFontCache.setCharBmp(mFontSize, wchar, *size, *dyExtra, bits);

		return bits;
	}



	//算法 最久未使用页面替换算法
	#define FONT_CACHE_CHARS	65536

	//cahche 条目
	typedef struct tagFONT_CACHE_ENTRY
	{
		LIST_HEAD	mList;

		WCHAR	mCode;	//字符内码
		WORD	m_wReserve;
		unsigned char	mFontSize;
		unsigned char	mWidth;
		unsigned char	mHeight;
		char	mDyExtra;

		unsigned char  mData[4];

	} FONT_CACHE_ENTRY, *PFONT_CACHE_ENTRY ;

	//cache header
	typedef struct tagFONT_CACHE_HEADER
	{
		LIST_HEAD	mEntryHead; 

		PFONT_CACHE_ENTRY	mEntry; //条目数开始地址
		int mTotalEntries;	//总条目数
		int	mBytesOfEntry;  //条目占的字节数

		int	 mMaxFontSize;
		WORD mCharEntryMap[FONT_CACHE_CHARS];

	} FONT_CACHE_HEADER, *PFONT_CACHE_HEADER;


	FontCache::FontCache()
	{
		mCache = NULL;

		int memBytes  = 1024 * 1024;
		void *memAddr = malloc(memBytes);

		init(16, memAddr, memBytes);

	}


	FontCache::~FontCache()
	{

		safeFree(mCache);

	}


	//字体Cache 全局初始化
	bool FontCache::init (int maxFontSize, void *memAddr, int memBytes)
	{
		PFONT_CACHE_HEADER	cacheHeader;
		PFONT_CACHE_ENTRY	cacheEntry;	
		int	i;

		if (memAddr == NULL)
			return false ;

		if (memBytes < sizeof (FONT_CACHE_HEADER))
		{
			log_e("memory is too small \n");
			return false;
		}

		cacheHeader = (PFONT_CACHE_HEADER)memAddr;
		memset (cacheHeader, 0, sizeof (FONT_CACHE_HEADER));

		cacheHeader->mBytesOfEntry = (sizeof (FONT_CACHE_ENTRY) - 4 + maxFontSize * maxFontSize + 3) & (~0x03);
		cacheHeader->mTotalEntries = (memBytes - sizeof (FONT_CACHE_HEADER)) / cacheHeader->mBytesOfEntry;
		cacheHeader->mEntry		   = (PFONT_CACHE_ENTRY)((PFONT_CACHE_HEADER)memAddr + 1);
		cacheHeader->mTotalEntries = min(cacheHeader->mTotalEntries, FONT_CACHE_CHARS - 1);

		cacheEntry = cacheHeader->mEntry;

		INIT_LIST_HEAD(&(cacheHeader->mEntryHead));

		for (i = 0; i < cacheHeader->mTotalEntries; i++)
		{
			list_add_tail(&(cacheEntry->mList), &(cacheHeader->mEntryHead));
			cacheEntry->mCode = 0;
			cacheEntry = (PFONT_CACHE_ENTRY)((unsigned char *)cacheEntry + (cacheHeader->mBytesOfEntry));
		}
		mCache = memAddr;
		return true;
	}



	//从字体Cache 获取字体位图
	bool FontCache::getCharBmp(int fontSize, WCHAR wchar, Size *size, int *dyExtra, unsigned char** fontBmp)
	{
		PFONT_CACHE_HEADER	cacheHeader;
		PFONT_CACHE_ENTRY	cacheEntry;	
		int	order;

		if (mCache == NULL)
			return false;

		cacheHeader = (PFONT_CACHE_HEADER)mCache;
		order  = (int)(DWORD)(cacheHeader->mCharEntryMap[(DWORD)(WORD)wchar]);
		if (order >= cacheHeader->mTotalEntries)
			return false;

		cacheEntry = (PFONT_CACHE_ENTRY)((char *)cacheHeader->mEntry + order * cacheHeader->mBytesOfEntry);
		if (cacheEntry->mCode != wchar)
			return false;

		if (cacheEntry->mFontSize != fontSize)
			return false;

		size->cx = (int)(WORD)cacheEntry->mWidth; 
		size->cy = (int)(WORD)cacheEntry->mHeight;
		*dyExtra = (int)cacheEntry->mDyExtra;

		if (size->cx > 0)
			*fontBmp = cacheEntry->mData;
		else
			*fontBmp = NULL;

		list_move_tail(&(cacheEntry->mList), &(cacheHeader->mEntryHead));
		return true;
	}



	//将字体位图存到字体Cache
	void FontCache::setCharBmp (int fontSize, WCHAR wchar, Size& size, int dyExtra, unsigned char*	fontBmp)
	{
		PFONT_CACHE_HEADER	cacheHeader;
		PFONT_CACHE_ENTRY	cacheEntry;	
		int	 order;
		int	 byteCount;
		int	 index;

		if ((mCache == NULL) || (fontBmp == NULL && size.cx != 0))
			return;

		cacheHeader = (PFONT_CACHE_HEADER)mCache;
		byteCount   = size.cx * size.cy ;
		if (byteCount > (cacheHeader->mBytesOfEntry - (int)(sizeof (FONT_CACHE_ENTRY) - 4)))
			return;

		index  = (int)(DWORD)wchar;
		order  = (int)(DWORD)(cacheHeader->mCharEntryMap[index]);
		if (order < cacheHeader->mTotalEntries)
		{
			cacheEntry = (PFONT_CACHE_ENTRY)((char *)cacheHeader->mEntry + order * cacheHeader->mBytesOfEntry);
			if (cacheEntry->mCode == wchar && cacheEntry->mFontSize == fontSize)
				return;
		}

		cacheEntry = list_entry((cacheHeader->mEntryHead.next), FONT_CACHE_ENTRY, mList); 

		list_move_tail(&(cacheEntry->mList), &(cacheHeader->mEntryHead));

		cacheEntry->mCode	  = wchar;
		cacheEntry->mWidth    = (unsigned char)size.cx;
		cacheEntry->mHeight   = (unsigned char)size.cy;
		cacheEntry->mDyExtra  = dyExtra;
		cacheEntry->mFontSize = (unsigned char)fontSize;

		memcpy(cacheEntry->mData, fontBmp, byteCount);
		cacheHeader->mCharEntryMap[index] = ((char *)cacheEntry - (char *)cacheHeader->mEntry) / cacheHeader->mBytesOfEntry;
	}

	FontCache  gFontCache;
}