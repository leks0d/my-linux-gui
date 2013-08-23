
#ifndef _MANGO_GRAPHIC_H

namespace mango
{

/* Background Modes */
#define TRANSPARENT         1
#define OPAQUE              2
//扩展, 阴影
#define SHADOW				3
#define OPAQUE_ALPHA		4



#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */

#define DSTTRANSP			(DWORD)0x10000001 /* 用源透明度*/
#define DSTTRANSPCOLORNESS	(DWORD)0x10000002 /* 源透明色不显示 并且 用源透明度进行计算*/
#define DSTTRANSPHALF		(DWORD)0x10000003 /* 用目的透明度, 半源颜色*/
#define DSTTRANSPMID		(DWORD)0x10000004 /* 用目的透明色, 半透明*/
#define DSTTRANSPMIDCOLORNESS	(DWORD)0x10000005 /* 源透明色不显示 并且 用源透明度进行计算*/
#define DSTTRANSPQUARTER	(DWORD)0x10000007 /* 1/4 */
#define SRCALPHA			(DWORD)0x10000008	/*源透明度处理*/
#define SRCDIMALPHA			(DWORD)0x10000009	/*源半暗之后透明度处理*/
#define SRCDIM				(DWORD)0x1000000A	/*源半暗*/
#define SRCGRAYALPHA		(DWORD)0x1000000B	/*源灰化后透明度处理*/
#define SRCGRAY				(DWORD)0x1000000C	/*源灰化*/
#define SRCALPHADSTALPHA	(DWORD)0x1000000D	/*源透明度与目的透明度混合处理*/


#define DT_TOP              0x00000000
#define DT_LEFT             0x00000000
#define DT_CENTER           0x00000001
#define DT_RIGHT            0x00000002
#define DT_VCENTER          0x00000004
#define DT_BOTTOM           0x00000008
#define DT_WORDBREAK        0x00000010
#define DT_SINGLELINE       0x00000020
#define DT_EXPANDTABS       0x00000040
#define DT_TABSTOP          0x00000080
#define DT_NOCLIP           0x00000100
#define DT_EXTERNALLEADING  0x00000200
#define DT_CALCRECT         0x00000400
#define DT_NOPREFIX         0x00000800
#define DT_INTERNAL         0x00001000

#define DT_EDITCONTROL      0x00002000
#define DT_PATH_ELLIPSIS    0x00004000
#define DT_END_ELLIPSIS     0x00008000
#define DT_MODIFYSTRING     0x00010000
#define DT_RTLREADING       0x00020000
#define DT_WORD_ELLIPSIS    0x00040000

	class Canvas : public Object
	{
	public:
		Canvas(void);
		virtual ~Canvas(void);

		bool moveTo(int x, int y, Point* point);
		bool lineTo(int x, int y);

		bool polyline(const Point *point, int count);
		bool polylineTo(const Point *point, int count);


		bool fillRectAlpha (Rect& fillRect, unsigned char alpha);
		bool fillRect(Rect &rect, Brush &brush);
		bool frameRect (Rect &rect, Brush &brush);

		bool invertRect(Rect &rect);

		bool rectangle (int left, int top, int right, int bottom);
		bool rectangle (Rect &rect);

		void drawText(const TCHAR *string, int count, Rect &rect, int format);
		void drawText(const char *string, int count, Rect &rect, int format);
		void drawText(const TCHAR *string, int count, int x, int y);
		void drawText(const char *string, int count, int x, int y);

		void drawTextResource(int id, int x, int y);
		void drawTextResource(int id, Rect &rect, int format);

		COLORREF setTextColor(COLORREF color);
		COLORREF getTextColor();

		void setTextSize(int size);
		void setTextBackColor(COLORREF color);
		
		void setTextLanguage(int lanId);
		int getTextLanguage();

		int  getTextCharacterExtra(void);
		int  setTextCharacterExtra(int charExtra);

		bool getTextExtentPoint(const TCHAR* string,int count, Size& size);
		bool checkTextExtentPoint (const TCHAR* string, int *count, Size& size);


		Bitmap* select(Bitmap* bitmap);
		Pen*    select(Pen* pen);
		Brush*  select(Brush* brush);
		Font*   select(Font* font);
		Region* select(Region* region);

		bool drawImageResource(int id, int x, int y, bool alpha = true);
		bool drawImageResource(int id, int x, int y,int width,int hight,bool alpha = true);
		bool drawBitmap(int* colors, int x, int y, int width, int height, bool alpha = true); 
		bool drawBitmapRGB(int* colors, int x, int y, int width, int height);
	public:
		void renew(class View* view);
		void swapScreenFrontBuffer();
		static void charToWCHAR(char *string,WCHAR *des);
		void copyScreenFrontToBack();
		Point& getViewPos(){return mViewPos;}
	private:
		bool drawLine32(Rect& clipRect, int xStart, int yStart, int xEnd, int yEnd);
		bool drawLine(Rect& clipRect, int xStart, int yStart, int xEnd, int yEnd);

		bool fillRectColor32(Rect& clipRect, Rect& fillRect, COLORREF clr);
		bool fillRectColor(Rect& clipRect, Rect& fillRect, COLORREF clr);

		bool fillRectAlpha (Rect& clipRect, Rect& fillRect, unsigned char alpha);

		void viewToCanvas(int& x, int& y);
		void viewToCanvas(Point& pt);
		void viewToCanvas(Rect& rect);

		bool getClipRect(Rect& rect);

		int drawSingleLineText(Rect& clipRect, int x, int y, const wchar_t* lpString, int cbString);

		int getMultiLinesTextDrawParam (const TCHAR* lpString, int cbString, \
					  int cxView, const TCHAR **lpptStartPerLine, int *piCharsPerLine, int *piWidthPerline, int iMaxLines, Size* lpSize);


		bool bitBlt_32(void *pDestBits,	int iDestWidth, int nXDest,  int nYDest,  int nWidth,  int nHeight, \
				void *pSrcBits,  int iSrcWidth,  int nXSrc,   int nYSrc,  \
				DWORD dwRop, DWORD dwAlpha, Region* rgn);
		

	private:
		Region*  mInitRegion;
		Region*  mRegion;
		Bitmap*  mBitmap;
		Pen*     mPen;
		Brush*   mBrush;
		Font*    mFont;

		Point    mPenPos;  //当前笔的位置
		Point    mViewPos;  //当前View在画布中的位置
		Rect     mViewRect; //当前View在画布中的区域

		int      mCharExtra;

		COLORREF	mTextColor;
		COLORREF	mBkColor;

		int		mRop2;		//Binary raster ops
		int		mBkMode;		//
		int		mAlpha;		//0 -- 全透明, 255 -- 不透

		int 	mLanguageId;
	};
}




#endif