
#ifndef _MANGO_CONTROLS_H
#define _MANGO_CONTROLS_H

namespace mango
{
	class MSkBitmap{
	public:		
		int* mBits;
		int width;
		int height;
	
		MSkBitmap(){mBits = NULL;};
		
		void create(int *bit,int w,int h){
			mBits = new int[w*h*4];
			memcpy(mBits,bit,w*h*4);
			width = w;
			height = h;
		}

		void release(){
			if(mBits != NULL){
				delete mBits;
				mBits = NULL;
			}
			width = 0;
			height = 0;			
		}
		bool isVaild(){
			return width>0&&height>0;
		}
	};
	
	class Mstring{
		public:
			char* mstr;
			int pos;
			int len;
			Mstring(void){}
			Mstring(int n){mstr = new char[n];pos=0;len=n;}
			
			int mSprintf(const char* str,int n);
			int mfloatSprintf(const char* str,float n);
			void clear(){delete mstr;mstr = new char[len];pos=0;}
			void setPlayTime(int n);
	};

	class StaticView : public View
	{
	public:
		StaticView(void);
		StaticView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);

		virtual ~StaticView(void);

	public:
		virtual int onPaint(Canvas& canvas);
	};

	class Button : public View
	{
	public:
		Button(void);
		Button(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		~Button(void);

	public:
		virtual int onPaint(Canvas& canvas);

		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);

	public:
		int setNormalImageId(int imageId);
		int setPressedImageId(int imageId);
		int setImageResId(int normalId,int pressId);
	private:
		int  mPressedImageId;
		int  mNormalImageId;
		bool mPressed;
	};

	class ImageView : public View
	{
		public:
			ImageView(void);
			ImageView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
			~ImageView(void);
			void setImageResoure(int Id);
			void setImagePath(char* path);
			void setSkBitmap(int *bit,int w,int h);
			virtual int onPaint(Canvas& canvas);
		private:
			char *mImagePath;
			int resId;
			int ResType;
			MSkBitmap *mMSkBitmap;
	};

#define TEXT_LAYOUT_LEFT		0
#define TEXT_LAYOUT_CENTER	1
#define TEXT_LAYOUT_RIGHT		2

	class TextView : public View
	{
		public:
			TextView(void);
			TextView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
			~TextView(void);
			void setTextResoure(int Id);
			void setTextString(char* text);
			void setTextColor(COLORREF color);
			void setTextSelectColor(COLORREF color){mSelectColor = color;}
			void setTextSize(int size);
			void setTextLayoutType(int layout);
			void setTextSelect(int n){ if(n!=mSelect){mSelect=n;invalidateRect();} }
			void setTextEnable(int n){ if(n!=mEnable){mEnable=n;invalidateRect();} }
			void computeLeft(Canvas *canvas);
			void getTextString(char *string);
			void setBackGround(int nor,int sec){mNormalBgdResId=nor,mSelectBgdResId=sec;};
			void setLog(int log){mLog = log;}
			virtual int onPaint(Canvas& canvas);
			virtual int onTouchDown(int x, int y, int flag);
			virtual int onTouchUp(int x, int y, int flag);
		private:
			char *mText;
			WCHAR *iText;
			int iTextLen;
			int resId;
			int ResType;			
			COLORREF mColor;
			COLORREF mSelectColor;
			int mNormalBgdResId;
			int mSelectBgdResId;
			int mSelect;
			int mEnable;
			int mSize;
			int mPress;
			int mLeft;
			int mTop;
			int mLayoutType;
			int mLog;
	};


class ValueTextView : public View
{
	public:
		ValueTextView(void);
		ValueTextView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		~ValueTextView(void);
		void setTextResoure(int Id);
		void setTextString(char* text);
		void setTextColor(COLORREF color);	
		void setTextSize(int size);
		void setTextLayoutType(int layout);
		void computeLeft(Canvas *canvas);
		void setBackGround(int nor,int sec){mNormalBgdResId=nor,mSelectBgdResId=sec;};
		virtual int onPaint(Canvas& canvas);
		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);
	private:
		char *mText;
		
		int resId;
		int ResType;			
		COLORREF mColor;
		int mNormalBgdResId;
		int mSelectBgdResId;
		int mSize;
		int mPress;
		int mLeft;
		int mTop;
		int mLayoutType;
		int mKeyLen;
};
#define SEEKBAR_TOUCH_DOWM 0
#define SEEKBAR_TOUCH_CHANGE 1
#define SEEKBAR_TOUCH_UP 2
	class SeekBar : public View
	{
		public:
			SeekBar(void);
			SeekBar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
			~SeekBar(void);
			void setImageResoure(int bkgId,int seekId,int thumbId){mBkgImage=bkgId;mSeekImage=seekId;mThumbImage=thumbId;}
			void setProgress(int n);
			int getProgress();
			void setTouchX(int x);
			void setMax(int n){mMax=n;log_i("SeekBar setMax = %d",mMax);}
			
			virtual int onPaint(Canvas& canvas);
			virtual int onTouchDown(int x, int y, int flag);
			virtual int onTouchMove(int x, int y, int flag);
			virtual int onTouchUp(int x, int y, int flag);
		private:
			int mBkgImage;
			int mSeekImage;
			int mThumbImage;
			int mProgress;
			int mMax;
			int mWidth;
			int onTouch;
			int mSeekWidth;
			int mThumbX;
			int mBkgLeft;
			int mSeekLeft;
	};

#define VSEEKBAR_TOUCH_DOWM 		0xF0000001
#define VSEEKBAR_TOUCH_CHANGE 	0xF0000002
#define VSEEKBAR_TOUCH_UP 		0xF0000003
#define VSEEKBAR_TEXTVIEW_UP 		0xF0000004


class VerticalSeekBar : public View
{
	public:
		VerticalSeekBar(void);
		VerticalSeekBar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		~VerticalSeekBar(void);
		void setImageResoure(int bkgId,int seekId,int thumbId){mBkgImage=bkgId;mSeekImage=seekId;mThumbImage=thumbId;}
		void setProgress(int n);
		int getProgress();
		void setTouchY(int x);
		void setMax(int n){mMax=n;}
		void setEnable(int n);
		virtual int onPaint(Canvas& canvas);
		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchMove(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);
	private:
		int mBkgImage;
		int mSeekImage;
		int mThumbImage;
		int mProgress;
		int mMax;
		int mWidth;
		int mHeight;
		int onTouch;
		int mSeekWidth;
		int mThumbX;
		int mThumbY;
		int mBkgLeft;
		int mSeekLeft;
		int mEnable;
};



#define TBS_VERT                0x0002
#define TBS_HORZ                0x0000
#define TBS_NOTICKS             0x0010
#define TBS_NOTHUMB             0x0080
#define TBS_REVERSED            0x0200  // Accessibility hint: the smaller number (usually the min value) means "high" and the larger number (usually the max value) means "low"
#define TBS_DOWNISLEFT          0x0400  // Down=Left and Up=Right (default is Down=Right and Up=Left)

	typedef struct tagCTRL_TRACKBAR_LAYOUT
	{
		int		m_iImageBk ;
		int		m_iImageThumb ;
		int		m_iImageTrackDone ;

		Size	m_sizeThumb ;
		Rect	m_rcMarginTrack  ; //滑道边距	 


	} CTRL_TRACKBAR_LAYOUT, *PCTRL_TRACKBAR_LAYOUT ;

	class StatuBar : public View
	{
		public:
			StatuBar(void);
			StatuBar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
			void setVolume(int volume);
			void setVolumeIcon(int id){mVolumeImage = id;}
			void setBattery(int bat);
			void setBatteryIcon(int id){mBatteryImage = id;}
			virtual ~StatuBar(void);
			virtual int onCreate();
			virtual int onDestroy();
			virtual int onPaint(Canvas& canvas);
			virtual int onNotify(View* fromView, int code, void* parameter);
			virtual int onKeyDown(int keyCode, int flag);
			virtual int onKeyUp(int keyCode, int flag); 
			virtual int onCommand(int id, int code, View* fromView);
			
		private:
			Button*  mVolumeButton;
			ImageView* mBatteryIcon;
			TextView* mBatteryText;
			TextView* mVolumeText;
			int mBatteryImage;
			int mVolumeImage;
			int mVolume;
			int mBattery;
	};

	class Trackbar : public View
	{
	public:
		Trackbar(void);
		Trackbar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		~Trackbar(void);

	public:
		virtual int onPaint(Canvas& canvas);

		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);

	public:

	private:
		int	 getDoneLength (Rect& trackRect);
		void getThumbDisplayPosition(Point& displayPosition);
		int  getPosition(Point& thumbCenter);

	private:
		bool	mOperating;
		bool	mPositionChange;
		Point	mCursorPosition;

		int		mMinimum;
		int		mMaximum;
		int		mCurrentValue;

		CTRL_TRACKBAR_LAYOUT	mLayout;

	};



#define CTRL_LISTVIEW_TIMER_ID 10
#define CTRL_LISTVIEW_FUN_ICON_CX  48
#define CTRL_LISTVIEW_FUN_ICON_CY  48

	class ListViewItem
	{
	public:
		ListViewItem();
		~ListViewItem();

	public:
		UINT mask;
		int iItem;
		int iSubItem;
		UINT state;
		UINT stateMask;
		LPWSTR pszText;
		int cchTextMax;
		int iImage;
		int background;
		void* lParam;
		int paramType;
		int iText ;
	} ;

#define LVIF_TEXT               0x00000001
#define LVIF_IMAGE              0x00000002
#define LVIF_PARAM              0x00000004
#define LVIF_STATE              0x00000008
#define LVIF_INDENT             0x00000010
#define LVIF_ITEXT				 0x00001000
#define LVIF_ADAPTER			 0x00010000

#define LIST_PARAM_FILE		1
#define LIST_PARAM_MAIN		2
#define LIST_PARAM_SETTING	3
#define LIST_PARAM_MUSIC		4

	//Item
	typedef struct tagLISTVIEW_RECORD
	{
		LIST_HEAD	m_list	  ;	 //正序 
		LIST_HEAD	m_listSub ;	 //正序

		Rect		m_rect   ;	 //Item 显示矩形域
		int			m_iFun	;    //功能项
		ListViewItem		m_lvItem ;

	} LISTVIEW_RECORD;

	//SubItem
	typedef struct tagCTRL_LISTVIEW_SUBITEM
	{
		LIST_HEAD				m_list  ;	//正序
		LISTVIEW_RECORD*		m_pItem ;	//指向本行的ITEM

		ListViewItem			m_lvItem ;

	} LISTVIEW_RECORD_ITEM;



//listview 排版参数
typedef struct tagCTRL_LISTVIEW_LAYOUT
{
	void* m_hInstance ; //图标所在模块句柄 

	SIZE	m_sizeItem		;	//每一项的大小
	SIZE	m_sizeItemIcon	;	//ICON 大小, 每一项的大小
	SIZE	m_sizeItemExtra ;	//额外范围
	Rect	m_rcMargin ;		//边距

	int		m_iImageActive ;	//激活标志 需要与ICON 一般大
	int		m_iImageArrow  ;	//箭头标志 需要与ICON 一般大

}CTRL_LISTVIEW_LAYOUT , *PCTRL_LISTVIEW_LAYOUT ;

#define LVM_GETZONEY  (LVM_FIRST + 1000)
#define LVM_SETZONEY  (LVM_FIRST + 1001)

#define LVS_ICON                0x0000
#define LVS_REPORT              0x0001
#define LVS_SMALLICON           0x0002
#define LVS_LIST                0x0003




#define LVIS_FOCUSED            0x0001
#define LVIS_SELECTED           0x0002
#define LVIS_CUT                0x0004
#define LVIS_DROPHILITED        0x0008
#define LVIS_GLOW               0x0010
#define LVIS_ACTIVATING         0x0020

#define LVIS_OVERLAYMASK        0x0F00
#define LVIS_STATEIMAGEMASK     0xF000

#define LVIS_LEFTMASK			0x000F0000
#define LVIS_ARROW				0x00010000
#define LVIS_CHECKBOX			0x00020000
#define LVIS_TRACKBAR			0x00040000

	class BaseAdapter{
		public:
		BaseAdapter(void){}
		~BaseAdapter(void){}
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount();
		virtual void* getItem(int index){return NULL;}
		virtual int getId(){return mId;}

		int mId;
	};


	class ListView : public View
	{
	public:
		ListView(void);
		ListView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~ListView(void);

	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onPaint(Canvas& canvas);
		void setListAdapter(BaseAdapter* adapter){ mListAdapter = adapter;}
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 

		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchMove(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);

	public:
		virtual int insertItem(ListViewItem *item);
		virtual	int setItem (ListViewItem *item);
		virtual	int deleteItem (int index);
		virtual	int	deleteAllItems(void);
		virtual	int	getItem (ListViewItem *item);
		virtual	int	getItemCount (void);
		virtual	int	getHotItem (void);
		virtual	int	getZoneY (void);
		virtual	int	setZoneY(int y, bool update);
		virtual	int	setItemState (int index, ListViewItem* lvitem);
		virtual ListViewItem*  getSelectedItem(void);

		//record1 < reocrd2  return < 0;
		//record1 = reocrd2  return  0;
		//record1 > reocrd2  return  1;
		virtual int compare(LISTVIEW_RECORD* reocrd1, LISTVIEW_RECORD* reocrd2);

		
	public:
		//删除ListViewItem数据
		BOOL deleteRecordItem (ListViewItem* lplvitem);

		//删除一行的所有子项
		BOOL deleteRecordAllItem (LISTVIEW_RECORD*  record);

		//删除listview 中指定行项
		BOOL deleteRecord (LISTVIEW_RECORD*  record);


		//删除listview 中所有行
		BOOL deleteAllRecord (void);

		//第一项
		LISTVIEW_RECORD*  getFirsetRecord(void);

		//尾项
		LISTVIEW_RECORD*  getTailRecord(void);

		//下一项
		LISTVIEW_RECORD*  getNextRecord(LISTVIEW_RECORD* record, BOOL bCycle = false);

		//上一项
		LISTVIEW_RECORD*  getPrevRecord(LISTVIEW_RECORD* record, BOOL bCycle = false);

		//根据顺号, 获得node
		LISTVIEW_RECORD*  getRecord (int iIndex);

		//根据node, 序号
		int  getIndex ( LISTVIEW_RECORD* record);

		//根据列号(列号 > 0), 获得子项指针
		LISTVIEW_RECORD_ITEM*  getRecordItem (LISTVIEW_RECORD* record, int iSubItem);
		
		//排版, 确定各项的显示位置
		void layout();
		
		void getRecordRectInView(LISTVIEW_RECORD*	record, Rect *lpRect);
		
		//重新确zone 在客户区的坐标, 确保指定项至少是部份可以见
		BOOL ensureVisible (LISTVIEW_RECORD*	record, BOOL bPartialOK);
		
		//查找第一个显示项
		LISTVIEW_RECORD* findFirstPainRecord (void);
		
		//根据x, y 坐标找到选中项
		LISTVIEW_RECORD*  getRecordFromPoint (Point pt);
		
		//根据ListViewItem 中的mask 指定数据项赋值
		bool takeItem(ListViewItem* dest, ListViewItem* org);
		
		//获取由ListViewItem 的mask 指定的数据
		void retakeItem (ListViewItem* lpDest, ListViewItem* lpOrg);
		
		//paint
		bool paintIconRecord (Canvas& canvas, LISTVIEW_RECORD*	record, bool eraseBk);
		bool paintListRecord (Canvas& canvas, LISTVIEW_RECORD*	record, bool eraseBk);
		bool paintRecord(Canvas& canvas, LISTVIEW_RECORD* record, bool eraseBk);

		bool paintDragListItem (Canvas& canvas, LISTVIEW_RECORD*	record);
		bool paintDragItem (Canvas& canvas, LISTVIEW_RECORD*	record);
		
		
		//动画 重画新区
	//	LRESULT  ctrllvCartoonRedraw (Canvas& canvas,  LPRECT lprcRedraw);
		
		
		//重构动画帧, 并修 数据显示区的位置
	//	int ctrllvCartoonFrame (Canvas& canvas, HDC hdcFront,  int dy);
		
	//	//重构 Gearshift shortcut 一帧
	//	BOOL ctrllvGearshiftProcess(Canvas& canvas, HDC hdcFront, UINT uEvenID, int iShift, HWND hWnd, VOID *lParam);
		
		
		//恢复反白显示项 至正常模式
		void ctrllvResumeSelectedItem (BOOL bUpdate);
		
		//回归移动动画
	//	LRESULT  ctrllvCartoonTropic ();
		
		//快速移动动画
	//	LRESULT  ctrllvCartoonRiffle (int ySpeed);
		
		//拖动动画
	//	LRESULT  ctrllvCartoonDrag (int ySpace);
		
		//
	//	BOOL  ctrllvDragDropBegin (LISTVIEW_RECORD* record);
		
		//
	//	BOOL  ctrllvDragDropEnd (POINT ptCursor);
				
		//处理WM_LAYOUT 消息
		int	 setLayout(PCTRL_LISTVIEW_LAYOUT layout);
		
		COLORREF setTextColor(COLORREF color);
		
		//插入字符串
		int insertString(int index, LPCTSTR lpszItem);
		
		//插入字符串和图片ID
		int insertStringImage(int index, LPCTSTR lpszItem, int iImage);
		
		//插入字符串ID和图片ID
		int insertTextIDImage (int index, int iText, int iImage);

		
		//二分法排队插入
		void binarySortInsert(LISTVIEW_RECORD** recordArray, int count, LISTVIEW_RECORD* record);

		int	sort(void);

		void setListItemBackground(int res,int press){mItemBackground = res;mPressItemBackground=press;}
	private:
		//重画新区
		int  cartoonRedraw(Canvas& canvas, Rect& redrawRect);

		//修改正移动数据
		int cartoonModifyDistance(int dy);

		//移动zone 位置
		int cartoonMoveZone(int dy);

		int cartoonDisplay(void);

		int cartoonDrag(int dy);

		//恢复反白显示项 至正常模式
		void resumeSelectedRecord(bool update = false);

		

	private:
		LIST_HEAD	mRecordHead; //正序

		Point		mZonePoint;		//视图项区域相对窗口的坐标
		Size		mZoneSize;		//视图项区域大小
		BOOL		mNeetLayout;	//是否需要排版

		LISTVIEW_RECORD* mFirstPaintRecord ;  //第一个显示项位置, 加快查找速度
		LISTVIEW_RECORD* mFocusedRecord  ;	 //焦点项
		LISTVIEW_RECORD* mSelectedRecord;	 //选中项, 目前仅支持一个被选中
		DWORD mFocusedTime ;    //焦点项时间
		int	  mSelectedIndex ;

		CTRL_LISTVIEW_LAYOUT mLayoutIcon ;
		CTRL_LISTVIEW_LAYOUT mLayoutList ;

		Point      mTouchDownPosition;
		Point      mTouchPrevPosition;
		bool       mTouchMove;

		BOOL		m_bDrage ;
		//	HIMAGELIST	m_hImagelistDrag ;	
		COLORREF	mTextColor ;
		int mItemBackground;
		int mPressItemBackground;
		BaseAdapter* mListAdapter;
	} ;




} ;

#endif
