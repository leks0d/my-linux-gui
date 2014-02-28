
#ifndef _MANGO_GRAPHIC_H

namespace mango
{
	#define BS_SOLID            0

	class Brush : public Object
	{
	public:
		Brush(void);
		Brush(COLORREF clr);
		virtual ~Brush(void);

		void set(int style, COLORREF clr, int hatch = 0);
		COLORREF getColor() {return mColor;};

	private:
		int mStyle;
		COLORREF mColor;
		int	mHatch;
	};


	class Pen : public Object
	{
	public:
		Pen(void);
		Pen(int style, int width, COLORREF color);
		virtual ~Pen(void);

		void set(int style, int width, COLORREF color);
		inline COLORREF  getColor() {return mColor;};

	private:
		int	mStyle; 
		int mWidth; 
		COLORREF mColor;
	};


	class Bitmap : public Object
	{
	public:
		Bitmap(void);
		Bitmap(int width, int hegiht, int bitsPixel);
		virtual ~Bitmap(void);

		bool create(int width, int height, int bitsPixel);
		void set(int width, int height, int bitsPixel = 32, void *bits = NULL);
		void setBits(void* bits);

		bool release();
		inline void* getBits() {return mBits;};
		inline int getWidth() {return mWidth;};
		inline int getHeight() {return mHeight;};
		inline int getBitsPixel() {return mBitsPixel;};


	private:
		int	mWidth;
		int	mHeight;
		int	mWidthBytes;
		int	mBitsPixel; 
		void* mBits; 
		bool mBitsNeedFree;
	};


	class Region : public Object
	{
	public:
		Region(void);
		Region(Rect &rect);
		Region(int left, int top, int right, int bottom);
		virtual ~Region(void);

		void set(int left, int top, int right, int bottom);
		bool reset(Rect &rect);
		bool ptIn(int x, int y);
		bool offset(int dx, int dy);
		bool validRect(Rect &rect);
		bool invalidRect(Rect &rect);

		inline void* getMask() {return mMask;};
		inline Rect& getRect() {return mRect;};
		inline int getMaskWidthBytes() {return mMaskWidthBytes;};

	private:
		void init(int left, int top, int right, int bottom);

	private:
		Rect	mRect;
		int		mMaskWidthBytes; //掩码区宽度字节数
		int		mMaskBytes;		 //掩码区字节数
		unsigned char* mMask;	 //掩码区地址

	};


	#define FONT_WIDTH_BUFF_BYTES  (65536 + 4)

	class Font : public Object
	{
	public:
		Font();
		~Font();

		int setSize(int size);

		int getWidth (WCHAR wchar);
		int getHeight(void);

		int getMaxWidth(void);
		int getMaxHeight(void);

		unsigned char *getCharBmp(WCHAR wchar, Size *size, int *dyExtra);


	private:
		int	  mFontSize;
		unsigned char*	mWidthBuffer;
	};


	#define FONT_CACHE_MAX_FONT	28


	class FontCache : public Object
	{
	public:
		FontCache();
		~FontCache();

		bool getCharBmp(int fontSize, WCHAR wchar, Size *size, int *dyExtra, unsigned char** fontBmp);
		void setCharBmp (int fontSize, WCHAR wchar, Size& size, int dyExtra, unsigned char*	fontBmp);
		void resetCache();


	private:
//	public:
		bool init(int iMaxFontSize, void *pMemoryAddr, int iMemoryBytes);
		void* mCache;
	};

	extern FontCache  gFontCache;

	class UnicodeLanguge{
		public:
			static int getWcharLanguge(WCHAR wchar);
	};

	typedef struct _Screen 
	{
		int		mYOffset;

		int		mWidth;
		int		mHeight;
		int      mBitsPerPixel;

		Rect	mInvalidRect ; //无效区, 须要更新部份
		unsigned char*	mBuffer;

	} Screen ;

	typedef struct _Surface
	{
		int		mWidth;
		int		mHeight;
		int		mWidthBytes;
		int		mBitsPixel;

		Rect	mInvalidRect; 
		Rect	m_UpdatedRect; 

		void*   mBits;

	} Surface;


	//各种备用Object 的数量 
	#define STOCK_BRUSH_NUM		3
	#define STOCK_PEN_NUM		3  //((int)FONT_MAX - (int)FONT_MIN -1)
	#define STOCK_FONT_NUM		2  //FONTSIZE_COUNT
	#define STOCK_RGN_NUM		8
	#define STOCK_DC_NUM		4
	#define STOCK_BITMAP_NUM	4


#ifdef WIN32
	typedef struct tagEM_FRAMEBUFFER
	{
		HANDLE  m_hMap ;

		int		m_cx ;
		int		m_cy ;

		LPVOID  m_pAddr ;
		int		m_yres  ;
		int		m_iLineBytes ;

		BOOL    m_bVideo ;
		BOOL	m_bVisible ;
		int     m_iBitsPixel; 
		int     m_iBacklightLum ; //背光亮度

		unsigned char Chromakey_mask[3];
		unsigned char Chromakey_keyvalue[3];
		BOOL		  m_bChromakey ;

		int mTouchX;
		int mTouchY;
		int mTouchPress;
		int mTouchFlag;

		int mKeyCode;
		int mKeyPress;
		int mKeyFlag;

	} EM_FRAMEBUFFER;
#endif

};




#endif