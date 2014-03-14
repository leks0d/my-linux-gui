
#ifndef _MANGO_CONTROLS_H
#define _MANGO_CONTROLS_H

namespace mango
{
	class MSkBitmap{
	public:		
		int* mBits;
		int width;
		int height;
		char *mFilePath;
		
		MSkBitmap(){mBits = NULL;mFilePath=NULL;}
		~MSkBitmap(){release();}
		int saveToFile(char* path){
			if(isVaild() && mBits!=NULL){
				int headr[2];
				FILE *fb = fopen(path, "wb");
				headr[0] = width;
				headr[1] = height;
				fwrite(headr,1,sizeof(int)*2,fb);
				fseek(fb,sizeof(int)*2,SEEK_SET);
				fwrite(mBits,1,sizeof(int)*width*height,fb);
				fclose(fb);
				return 1;
			}
			return 0;
		}
		void createFile(char* path){
			int headr[2],pathlen,ret;
			FILE* mFile;
			
			if(path == NULL){
				release();
				return;
			}
			if(mFilePath!=NULL && strcmp(mFilePath,path) == 0){
				log_i("invail img path");
				return;
			}
			
			release();
			
			pathlen = strlen(path)+1;
			mFilePath = new char[pathlen];
			memcpy(mFilePath,path,pathlen);

			mFile = fopen(mFilePath, "rb");
			if (mFile == NULL) {
				log_e ("Can't open resource file %s", path);
				safeDelete(mFilePath);
				return;
			}

			ret = fread(headr, 1, sizeof(int)*2, mFile);
			if(ret == sizeof(int)*2){
				width = headr[0];
				height = headr[1];
			}else{
				release();
				goto exit;
			}
			mBits = new int[width*height];
			
			fseek(mFile,sizeof(int)*2,SEEK_SET);
			ret = fread(mBits,1,sizeof(int)*width*height,mFile);
			if(ret != sizeof(int)*width*height){
				release();
				goto exit;
			}
exit:				
			fclose(mFile);
		}
		void create(int *bit,int w,int h){
			int i,count;
			
			release();

			mBits = new int[w*h];
			count = w*h;
			
			for(i = 0;i<count;i++){
				int r = bit[i]&0x00FF0000;
				int b = bit[i]&0x000000FF;
				mBits[i] = (bit[i]&0xFF00FF00) | (r>>16) | (b<<16) | 0xFF000000;
			}
			
			width = w;
			height = h;
		}
		void createNormal(int *bit,int w,int h){
			int i,count;
			
			release();

			mBits = new int[w*h];
			count = w*h;
			
			for(i = 0;i<count;i++){
				//int r = bit[i]&0x00FF0000;
				//int b = bit[i]&0x000000FF;
				mBits[i] = bit[i];//&0xFF00FF00) | (r>>16) | (b<<16);
			}
			
			width = w;
			height = h;
		}

		void release(){
			if(mBits != NULL){
				delete mBits;
				mBits = NULL;
			}
			if(mFilePath != NULL){
				delete mFilePath;
				mFilePath = NULL;
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
			int mStringSprintf(const char* str,char* s);
			void clear(){	memset(mstr,0,len);		pos=0;}
			void setPlayTime(int n);
			~Mstring(void){
				//log_i("-----%s",mstr);
				if(mstr!=NULL)
					delete[] mstr;
				mstr=NULL;
			}
	};

	class StaticView : public View
	{
	public:
		StaticView(void);
		StaticView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~StaticView(void);

	public:
		virtual int setTitle(const TCHAR* title);
		virtual int setTextString(const char* title);
		virtual int setTextResoure(int res);
		virtual int setTextSize(int size);
		virtual int setTextColor(int color);
		virtual int setTextBkRes(int res);
		virtual int onPaint(Canvas& canvas);
		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchMove(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);

	public:
		virtual int getTitlePaintStartX();
		virtual int setTitlePaintStartX(int x);

	private:
		void updateTextPaintExtent(const TCHAR* title);
		int  cartoonDrag(int distance);
		void cartoonDisplay(void);
		void paint(Canvas& canvas);



	private:
		int		mTitlePaintStartX;
		Size    mTitlePaintExtent;
		Point   mTouchPrevPosition;
		int 	mColor;
		int 	mTextSize;
		int 	mBkRes;
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
		DWORD mTouchDownTime;
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
			void setMSkBitmap(MSkBitmap *bitmap);
			void setBitmapAlps(bool alps){mAlps = alps;}
			void setBitmapStart(int x,int y){mStartX = x;mStartY = y;}
			void setVisiable(bool visiable);
			bool getVisiable(){return mVisiable;}
			virtual int onPaint(Canvas& canvas);
			virtual int onTouchUp(int x, int y, int flag);
		private:
			char *mImagePath;
			int resId;
			int ResType;
			MSkBitmap *mMSkBitmap;
			bool mAlps;
			int mStartX;
			int mStartY;
			bool mVisiable;
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
			void setTextDisSectBgd(int n){mSelectDisBgdResId=n;}
			void computeLeft(Canvas *canvas);
			void getTextString(char *string);
			void setBackGround(int nor,int sec){mNormalBgdResId=nor,mSelectBgdResId=sec;};
			void setLog(int log){mLog = log;}
			void setVisiable(bool visiable);
			void setIconRes(int res){mIconRes = res;}
			void setShowIcon(bool show){showIcon = show;invalidateRect();}
			void setonTouchFresh(bool fresh);
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
			int mSelectDisBgdResId;
			int mSelect;
			int mEnable;
			int mSize;
			int mPress;
			int mLeft;
			int mTop;
			int mLayoutType;
			int mLog;
			bool mVisiable;
			int mIconRes;
			bool showIcon;
			int mIconX;
			int mIconY;
			bool onTouchFresh;
	};


class ValueTextView : public View
{
	public:
		ValueTextView(void);
		ValueTextView(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		~ValueTextView(void);
		void setTextResoure(int Id);
		void setTextString(char* text);
		void setTextColor(COLORREF value,COLORREF context = RGB(180,180,180));	
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
		COLORREF mValueColor;
		COLORREF mContextColor;
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
			void cartoonDisplay();
			int getProgress();
			void setTouchX(int x);
			void setMax(int n){mMax=n; if(mMax == 0) mMax = 1;log_i("SeekBar setMax = %d",mMax);}
			int getMax(){return mMax;}
			void setBackgroundRes(int resId,int x,int y);
			void setSeekgroundRes(int resId,int x,int y);
			virtual int onPaint(Canvas& canvas);
			virtual int onTouchDown(int x, int y, int flag);
			virtual int onTouchMove(int x, int y, int flag);
			virtual int onTouchUp(int x, int y, int flag);
		private:
			int mBkgImage;
			int mBkgX;
			int mBkgY;
			int mSeekImage;
			int mSeekX;
			int mSeekY;	
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
#define IMAGEVIEW_TOUCH_UP 		0xF0000005


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
		Rect	m_rcMarginTrack  ; //�����߾�	 


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
		LIST_HEAD	m_list	  ;	 //���� 
		LIST_HEAD	m_listSub ;	 //����

		Rect		m_rect   ;	 //Item ��ʾ������
		int			m_iFun	;    //������
		ListViewItem		m_lvItem ;

	} LISTVIEW_RECORD;

	//SubItem
	typedef struct tagCTRL_LISTVIEW_SUBITEM
	{
		LIST_HEAD				m_list  ;	//����
		LISTVIEW_RECORD*		m_pItem ;	//ָ���е�ITEM

		ListViewItem			m_lvItem ;

	} LISTVIEW_RECORD_ITEM;



//listview �Ű����
typedef struct tagCTRL_LISTVIEW_LAYOUT
{
	void* m_hInstance ; //ͼ������ģ���� 

	SIZE	m_sizeItem		;	//ÿһ��Ĵ�С
	SIZE	m_sizeItemIcon	;	//ICON ��С, ÿһ��Ĵ�С
	SIZE	m_sizeItemExtra ;	//���ⷶΧ
	Rect	m_rcMargin ;		//�߾�

	int		m_iImageActive ;	//�����־ ��Ҫ��ICON һ���
	int		m_iImageArrow  ;	//��ͷ��־ ��Ҫ��ICON һ���

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
		BaseAdapter(void){mYoffset = 0;}
		~BaseAdapter(void){}
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual void refresh(){return;}
		virtual int getCount();
		virtual void* getItem(int index){return NULL;}
		virtual int getId(){return mId;}
		virtual int setYoffset(int y){
			//log_i("mYoffset = %d,y=%d",mYoffset,y);
			mYoffset = y;
		}
		virtual int getYoffset(){
			//log_i("mYoffset = %d",mYoffset);
			return mYoffset;
		}
		int mId;
		int mYoffset;
	};
	class ListBarControl{
		private:
			bool isBarTouch;
			int barTouchStartY;

			int displayLen;
			int offset;
			int drawLen;
			int drawOffset;
			int mCount;
		public:
			ListBarControl();
			
			void drawListBar(Canvas& canvas,int listOffset,int count);

			bool isPointOnBar(int x,int y);
			
			int touchYtoDrawoffset(int y);	

			bool barTouchUp();

			bool isBarOnTouch(){return isBarTouch;}
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
		void setListAdapter(BaseAdapter* adapter){
			mListAdapter = adapter; 
			//mZonePoint.y = mListAdapter->getYoffset();mZonePoint.x=0;
		}
		BaseAdapter* getListAdapter(){ return mListAdapter;}
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
		virtual Point& getTouchPrevPosition(void);
		virtual void refresh();
		
		//record1 < reocrd2  return < 0;
		//record1 = reocrd2  return  0;
		//record1 > reocrd2  return  1;
		virtual int compare(LISTVIEW_RECORD* reocrd1, LISTVIEW_RECORD* reocrd2);

		
	public:
		//ɾ��ListViewItem����
		BOOL deleteRecordItem (ListViewItem* lplvitem);

		//ɾ��һ�е���������
		BOOL deleteRecordAllItem (LISTVIEW_RECORD*  record);

		//ɾ��listview ��ָ������
		BOOL deleteRecord (LISTVIEW_RECORD*  record);


		//ɾ��listview ��������
		BOOL deleteAllRecord (void);

		//��һ��
		LISTVIEW_RECORD*  getFirsetRecord(void);

		//β��
		LISTVIEW_RECORD*  getTailRecord(void);

		//��һ��
		LISTVIEW_RECORD*  getNextRecord(LISTVIEW_RECORD* record, BOOL bCycle = false);

		//��һ��
		LISTVIEW_RECORD*  getPrevRecord(LISTVIEW_RECORD* record, BOOL bCycle = false);

		//����˳��, ���node
		LISTVIEW_RECORD*  getRecord (int iIndex);

		//����node, ���
		int  getIndex ( LISTVIEW_RECORD* record);

		//�����к�(�к� > 0), �������ָ��
		LISTVIEW_RECORD_ITEM*  getRecordItem (LISTVIEW_RECORD* record, int iSubItem);
		
		//�Ű�, ȷ���������ʾλ��
		void layout();
		
		void getRecordRectInView(LISTVIEW_RECORD*	record, Rect *lpRect);
		
		//����ȷzone �ڿͻ���������, ȷ��ָ���������ǲ��ݿ��Լ�
		BOOL ensureVisible (LISTVIEW_RECORD*	record, BOOL bPartialOK);
		
		//���ҵ�һ����ʾ��
		LISTVIEW_RECORD* findFirstPainRecord (void);
		
		//����x, y �����ҵ�ѡ����
		LISTVIEW_RECORD*  getRecordFromPoint (Point pt);
		
		//����ListViewItem �е�mask ָ�������ֵ
		bool takeItem(ListViewItem* dest, ListViewItem* org);
		
		//��ȡ��ListViewItem ��mask ָ��������
		void retakeItem (ListViewItem* lpDest, ListViewItem* lpOrg);
		
		//paint
		bool paintIconRecord (Canvas& canvas, LISTVIEW_RECORD*	record, bool eraseBk);
		bool paintListRecord (Canvas& canvas, LISTVIEW_RECORD*	record, bool eraseBk);
		bool paintRecord(Canvas& canvas, LISTVIEW_RECORD* record, bool eraseBk);

		bool paintDragListItem (Canvas& canvas, LISTVIEW_RECORD*	record);
		bool paintDragItem (Canvas& canvas, LISTVIEW_RECORD*	record);
		
		
		//���� �ػ�����
	//	LRESULT  ctrllvCartoonRedraw (Canvas& canvas,  LPRECT lprcRedraw);
		
		
		//�ع�����֡, ���� ������ʾ����λ��
	//	int ctrllvCartoonFrame (Canvas& canvas, HDC hdcFront,  int dy);
		
	//	//�ع� Gearshift shortcut һ֡
	//	BOOL ctrllvGearshiftProcess(Canvas& canvas, HDC hdcFront, UINT uEvenID, int iShift, HWND hWnd, VOID *lParam);
		
		
		//�ָ�������ʾ�� ������ģʽ
		void ctrllvResumeSelectedItem (BOOL bUpdate);
		
		//�ع��ƶ�����
	//	LRESULT  ctrllvCartoonTropic ();
		
		//�����ƶ�����
	//	LRESULT  ctrllvCartoonRiffle (int ySpeed);
		
		//�϶�����
	//	LRESULT  ctrllvCartoonDrag (int ySpace);
		
		//
	//	BOOL  ctrllvDragDropBegin (LISTVIEW_RECORD* record);
		
		//
	//	BOOL  ctrllvDragDropEnd (POINT ptCursor);
				
		//����WM_LAYOUT ��Ϣ
		int	 setLayout(PCTRL_LISTVIEW_LAYOUT layout);
		
		COLORREF setTextColor(COLORREF color);
		
		//�����ַ���
		int insertString(int index, LPCTSTR lpszItem);
		
		//�����ַ�����ͼƬID
		int insertStringImage(int index, LPCTSTR lpszItem, int iImage);
		
		//�����ַ���ID��ͼƬID
		int insertTextIDImage (int index, int iText, int iImage);

		
		//���ַ��ŶӲ���
		void binarySortInsert(LISTVIEW_RECORD** recordArray, int count, LISTVIEW_RECORD* record);

		int	sort(void);

		void setListItemBackground(int res,int press){mItemBackground = res;mPressItemBackground=press;}
		void setListViewBackground(int res){mListViewBackgound = res;}
		static bool gearshiftProcess(View* view, unsigned int evenID, int shift, VOID *parameter);
	private:
		//�ػ�����
		int  cartoonRedraw(Canvas& canvas, Rect& redrawRect);

		//�޸����ƶ�����
		int cartoonModifyDistance(int dy);

		//�ƶ�zone λ��
		int cartoonMoveZone(int dy);

		int cartoonDisplay(void);

		int cartoonDrag(int dy);
		int cartoonRiffle (int ySpeed);
		
		//�ع��ƶ�����
		int cartoonTropic(void);

		//�ָ�������ʾ�� ������ģʽ
		void resumeSelectedRecord(bool update = false);


	private:
		LIST_HEAD	mRecordHead; //����

		Point		mZonePoint;		//��ͼ��������Դ��ڵ�����
		Size		mZoneSize;		//��ͼ�������С
		BOOL		mNeetLayout;	//�Ƿ���Ҫ�Ű�

		LISTVIEW_RECORD* mFirstPaintRecord ;  //��һ����ʾ��λ��, �ӿ�����ٶ�
		LISTVIEW_RECORD* mFocusedRecord  ;	 //������
		LISTVIEW_RECORD* mSelectedRecord;	 //ѡ����, Ŀǰ��֧��һ����ѡ��
		DWORD mFocusedTime ;    //������ʱ��
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
		int mListViewBackgound;
		BaseAdapter* mListAdapter;
		GestureDetector mGestureDetector;
		ListBarControl mListBarControl;
	} ;



	//Cartoon
	typedef bool (*PGEARSHIFTPROCESS)(View* view, unsigned int evenID, int shift, VOID *parameter);


	class Cartoon : public Object
	{
	public:
		Cartoon(void);
		~Cartoon(void);

	public:
		static void gearshift(View* view, int destShift, int viewSize, PGEARSHIFTPROCESS pfnGearshiftProcess, void* parameter);

	private:
		static int getGearshift(int destShift, int viewSize);
	};
} ;

#endif
