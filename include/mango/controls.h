
#ifndef _MANGO_CONTROLS_H
#define _MANGO_CONTROLS_H

namespace mango
{

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

	private:
		int  mPressedImageId;
		int  mNormalImageId;
		bool mPressed;
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
		void* lParam;
		int iText ;
	} ;

#define LVIF_TEXT               0x00000001
#define LVIF_IMAGE              0x00000002
#define LVIF_PARAM              0x00000004
#define LVIF_STATE              0x00000008
#define LVIF_INDENT             0x00000010
#define LVIF_ITEXT				0x00001000



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
	} ;




} ;

#endif