
namespace mango
{
	class MediaListView : public ListView
	{
	public:
		MediaListView(void);
		MediaListView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~MediaListView(void);

	public:
		virtual int  compare(LISTVIEW_RECORD* reocrd1, LISTVIEW_RECORD* reocrd2);
	};


	class MediaView: public View
	{
	public:
		MediaView(void);
		MediaView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~MediaView(void);

	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onPaint(Canvas& canvas);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 


	private:
		void fillDirectoryFile(TCHAR*  path);
		void renewFillViewList();

		void insertFileToListView(UINT mask, TCHAR* name, int iconId, int fileAttributes,int itext = 0);
		bool isRootDirectory();
		void backToParentDirectory();
		void initMainList();



	public:
		MediaListView*  mListView;

	private:
		int		mMode;					//menu菜单 媒体库index 收藏夹 或 浏览路径
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];



	};
};
