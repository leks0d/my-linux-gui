// Cue.h: interface for the CCue class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_CUE_H__8AC1B864_AD1D_4F0A_8A7F_84C62201DED9__INCLUDED_)
#define AFX_CUE_H__8AC1B864_AD1D_4F0A_8A7F_84C62201DED9__INCLUDED_


#define  SONGS_MAX  64

class CString{
public:
	CString();
	~CString();
	void Format(const char *fromat,const char *path);
	int Find(const char *tag,int offset);
	int getLen();
	char* getString();
	char* Mid(int start,int len);
	int Mid(int start,int len,CString& out);
	CString& operator=(CString& cstring);
	CString& operator=(const char *str);
	bool operator==(const char *str);
	bool operator!=(const char *str);
public:
	char *string;
	int mlen;
};
class CStringArray{
public:
	CStringArray();
	~CStringArray();
	int addCString(CString& cstr);
	int getCString(int index,CString& out);
	int getCount();
private:
	CString *mList;
	int mLen;
	int mMax;
};

class CFile{
	public:
		CFile();
		~CFile();
		bool Open(const char *path,const char* mode);
		long GetLength();
		size_t Read(char *buf,size_t len);
		void Close();
	private:
		FILE *mFile;
		long mLen;
};

typedef struct song_t
{	
	int     star;
	int     len;
	CString m_strname;
	CString m_strart;
	CString m_strpath;
}song_t;

class CCue  
{
public:
	CCue();
	virtual ~CCue();
public:
	int   file_load(const char *pfile);
	int   performer_parser();
	int   title_parser();
	int   songs_parser();
	int   song_parser(int off,int index);
	int   get_next_song(song_t *psong);
	int   get_last_song(song_t *psong);
	int   get_cert_song(int index,song_t *psong);
public:
	int     m_total_song;            //����Ŀ
	int     m_index_song;        //��ǰ��Ŀ
	song_t  m_songs[SONGS_MAX];        //��Ŀ��Ϣ
	CString m_strperformer;       // ������Ϣ
	CString m_strtitle;           //��Ƭ���� 
	CString m_filepath;           //�ļ���ַ
private:
	CString m_strbuff;
};

#endif // !defined(AFX_CUE_H__8AC1B864_AD1D_4F0A_8A7F_84C62201DED9__INCLUDED_)
