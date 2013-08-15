
#ifndef _MANGO_GRAPHIC_H

namespace mango
{

/* Background Modes */
#define TRANSPARENT         1
#define OPAQUE              2
//��չ, ��Ӱ
#define SHADOW				3
#define OPAQUE_ALPHA		4



#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */

#define DSTTRANSP			(DWORD)0x10000001 /* ��Դ͸����*/
#define DSTTRANSPCOLORNESS	(DWORD)0x10000002 /* Դ͸��ɫ����ʾ ���� ��Դ͸���Ƚ��м���*/
#define DSTTRANSPHALF		(DWORD)0x10000003 /* ��Ŀ��͸����, ��Դ��ɫ*/
#define DSTTRANSPMID		(DWORD)0x10000004 /* ��Ŀ��͸��ɫ, ��͸��*/
#define DSTTRANSPMIDCOLORNESS	(DWORD)0x10000005 /* Դ͸��ɫ����ʾ ���� ��Դ͸���Ƚ��м���*/
#define DSTTRANSPQUARTER	(DWORD)0x10000007 /* 1/4 */
#define SRCALPHA			(DWORD)0x10000008	/*Դ͸���ȴ���*/
#define SRCDIMALPHA			(DWORD)0x10000009	/*Դ�밵֮��͸���ȴ���*/
#define SRCDIM				(DWORD)0x1000000A	/*Դ�밵*/
#define SRCGRAYALPHA		(DWORD)0x1000000B	/*Դ�һ���͸���ȴ���*/
#define SRCGRAY				(DWORD)0x1000000C	/*Դ�һ�*/
#define SRCALPHADSTALPHA	(DWORD)0x1000000D	/*Դ͸������Ŀ��͸���Ȼ�ϴ���*/


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

		bool drawImageResource(int id, int x, int y, bool alpha = true);
		bool drawImageResource(int id, int x, int y,int width,int hight,bool alpha = true);
		bool drawBitmap(int* colors, int x, int y, int width, int height); 
		bool drawBitmapRGB(int* colors, int x, int y, int width, int height);
	public:
		void renew(class View* view);
		void swapScreenFrontBuffer();
		static void charToWCHAR(char *string,WCHAR *des);

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

		Point    mPenPos;  //��ǰ�ʵ�λ��
		Point    mViewPos;  //��ǰView�ڻ����е�λ��
		Rect     mViewRect; //��ǰView�ڻ����е�����

		int      mCharExtra;

		COLORREF	mTextColor;
		COLORREF	mBkColor;

		int		mRop2;		//Binary raster ops
		int		mBkMode;		//
		int		mAlpha;		//0 -- ȫ͸��, 255 -- ��͸

		int 	mLanguageId;
	};
}




#endif