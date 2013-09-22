// Cue.cpp: implementation of the CCue class.
//
//////////////////////////////////////////////////////////////////////



/************************************************************************/
/*code :ok
  test :none                                                            */
/************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "player.h"

CCue::CCue()
{
	int i;
	m_filepath="";
	m_index_song=0;
	for (i=0;i<SONGS_MAX;i++)
	{
		m_songs->len=0;
		m_songs->m_strname="";
		m_songs->star=0;
	}
	m_strbuff="";
	m_total_song=0;
}

CCue::~CCue()
{
	log_i("------~CCue-------");
}
/************************************************************************/
/*  �ú����������CUE�ļ�                                               */
/************************************************************************/
int CCue::file_load(const char *pfile)//��ʼ����ÿ�μ����ļ���ʱ���� 
{
	
	CFile myf;
	int len,ret,i;
	char buff[10000];

	
	memset(buff,0,sizeof(buff));
	ret=myf.Open(pfile,"r");
	if (ret==0)
	{
		return -1;//�ļ���ʧ��
	}
	len=myf.GetLength();
	ret=myf.Read(buff,len);
	if (ret<len)
	{
		myf.Close();
		return -1;//���ļ����� 
	}
	myf.Close();
	//return ret;
	m_strbuff.Format("%s",buff);

	//ret=this->performer_parser();
	//ret+=this->title_parser();
	ret=this->songs_parser();
	//m_filepath.Format("%s",pfile);
 
	return ret;//������������д���ֱ�ӷ��ش���
}
/************************************************************************/
/* �ú�����ȡ��������Ϣ                                                 */
/************************************************************************/
int CCue::performer_parser()
{
	int off1,off2,off3;
	CString  str_cue,temp;
	char tag1[] = "PERFORMER" ,tag2[] = "\"",tag3[]="\r\n";

	off1 = m_strbuff.Find(tag1,0);//һ���ո�+һ������
	
	if(off1 == -1)
		return -1;
	
	off1 = off1 + strlen(tag1);
	
	off2 = m_strbuff.Find(tag2,off1);
	
	if(off2 == -1)
		return -1;
	
	off2 = off2 + 1;
	
	off3 = m_strbuff.Find(tag2,off2);
	
	if(off3 == -1)
		return -1;
	
	m_strperformer=m_strbuff.Mid(off2,off3-off2);

	log_i("m_strperformer=%s",m_strperformer.getString());
	return 0;
}
/************************************************************************/
/* �ú�����ȡ��Ƭ������                                                 */
/************************************************************************/
int CCue::title_parser()
{
	int off1,off2;
	CString  str_cue,temp;
	char tag1[]="TITLE",tag2[]="\r\n";
	
	off1=m_strbuff.Find(tag1,0)+strlen(tag1)+1+1;//һ���ո�+һ������ 
	off2=m_strbuff.Find(tag2,off1)-1;

	if (off1==-1||off2==-1)//����һ��û���ҵ����ƻ����ļ�
	{
		return -1;
	}
	
	if ((off2-off1)>64)//����ƫ��̫Զ��������ļ� 
	{
		return -1;
	}
	
	m_strtitle=m_strbuff.Mid(off1,off2-off1);
	log_i("m_strtitle=%s",m_strtitle.getString());
	return 0;
}
/************************************************************************/
/*����ƫ�ƻ�ȡĳ������Ϣ                                                */
/************************************************************************/
int CCue::song_parser(int off,int index)
{
	int off1,off2,ret1,ret2,min,sec;
	char  *tags[4]={"TITLE","\r\n","INDEX 01","PERFORMER"};//�����ؼ���ǩ
	CString str2;

	ret1=m_strbuff.Find(tags[0],off);
	off1=ret1+strlen(tags[0])+2;
	
	ret2=m_strbuff.Find(tags[1],off1);
	off2=ret2-1;
	
	if (ret1==-1||ret2==-1)//����һ��û���ҵ����ƻ����ļ�
	{
		return -1;
	}
	
//	if ((off2-off1)>128)//����ƫ��̫Զ��������ļ� 
//	{
//		return -1;
//	}

//	m_songs[index].m_strname=m_strbuff.Mid(off1,off2-off1);
	m_strbuff.Mid(off1,off2-off1,m_songs[index].m_strname);

	off1 = m_strbuff.Find(tags[2],off) + strlen(tags[2])+1;
	
//	str2 = m_strbuff.Mid(off1,2);
	m_strbuff.Mid(off1,2,str2);
	min = atoi(str2.getString());
	
//	str2 = m_strbuff.Mid(off1+3,2);
	m_strbuff.Mid(off1+3,2,str2);
	sec = atoi(str2.getString());

	if (min<0||sec<0) //ʱ�䳤��Ϊ���� ��������ļ�
	{
		return -1;
	}
	
	m_songs[index].star = min*60 + sec;

	return 0;
}
/************************************************************************/
/* �ú�������CUE�ļ������е�����Ϣ                                      */
/************************************************************************/
int CCue::songs_parser()
{
	char tag[]="TRACK";
	int len=0,off=0,i;	

	while (1)
	{
		off = m_strbuff.Find(tag,off);
		if (off == -1)//�Ҳ���TRACE��ǩ��������ļ� 
		{
			break;
		}
		off += strlen(tag);
		song_parser(off,this->m_total_song);
		this->m_total_song++;	
	}
	
	for (i=0;i<m_total_song-1;i++)
	{
		m_songs[i].len = m_songs[i+1].star - m_songs[i].star;
	}
	
	m_songs[m_total_song-1].len = 0;
	
	log_i("songs_parser m_total_song=%d",m_total_song);
	
	return 0;
}
/************************************************************************/
/* �����һ��                                                           */
/************************************************************************/
int CCue::get_next_song(song_t *psong)
{
	if (m_total_song<=0)
	{
		return -1;
	}
	*psong=m_songs[(++m_index_song)%m_total_song];
}
/************************************************************************/
/* �����һ��                                                           */
/************************************************************************/
int CCue::get_last_song(song_t *psong)
{
	if (m_total_song<=0)
	{
		return -1;
	}
	if (m_index_song<0)
	{
		m_index_song+=m_total_song;
	}
	*psong=m_songs[((--m_index_song)+m_total_song)%m_total_song];

	return 0;
}
/************************************************************************/
/* ����ض���Ŀ                                                         */
/************************************************************************/
int CCue::get_cert_song(int index,song_t *psong)
{
	//log_i("get_cert_song index=%d",index);
	if (index<0||index>=m_total_song)
	{
		return -1;
	}
	//log_i("----");
	*psong=m_songs[index];
	//log_i("----");
	m_index_song=index;
	//log_i("get_cert_song sucess.");
	return 0;
}

CFile::CFile()
{
	mFile = NULL;
	mLen = -1;
}
bool CFile::Open(const char *path,const char* mode){
	Close();
	mFile = fopen(path,mode);
	if(mFile != NULL)
		return true;
	else
		return false;
}
long CFile::GetLength(){
	if(mLen<0 && mFile!=NULL){
		fseek(mFile,0,SEEK_SET);
		fseek(mFile,0,SEEK_END);
		mLen = ftell(mFile);
	}
	return mLen;
}
size_t CFile::Read(char *buf,size_t len){
	size_t ret;
	if(mFile!=NULL){
		fseek(mFile,0,SEEK_SET);
		ret = fread(buf,1,len,mFile);
	}else{
		ret = 0;
	}
	return ret;
}
void CFile::Close(){
	if(mFile != NULL)
		fclose(mFile);
	mFile = NULL;
	mLen = -1;
}

CFile::~CFile()
{
	Close();
}

CString::CString(){
	string = NULL;
	mlen = 0;
}
CString::~CString(){
	safeDelete(string);
	mlen = 0;
}
void CString::Format(const char *format,const char *path){
	char *ptr;
	int len = 0;
	
	if(path == NULL){
		safeDelete(string);
		return;
	}
	
	len = strlen(format) + strlen(path);
	
	string = new char[len];
	
	ptr = string;

	ptr+=sprintf(ptr,format,path);

	mlen += (ptr-string);

	//log_i("mlen=%d,str=%s",mlen,string);
}
int CString::getLen(){
	return mlen;
}
char* CString::getString(){
	return string;
}
int CString::Find(const char *tag,int offset){
	char *temp,*ret;
	
	if(tag == NULL || offset>=mlen || offset<0)
		return -1;

	//log_i("tag=%s,offset=%d",tag,offset);
	
	temp = string + offset;
	
	ret = strstr(temp,tag);
	
	//log_i("strstr ret=0x%x,string=0x%x,temp=0x%x",ret,string,temp);
	//log_i("strstr find position=%d",ret-string);
	
	if(ret == NULL)
		return -1;
	else
		return ret-string;
}

char* CString::Mid(int start,int len){
	char *temp;

	if(len>0&&len<=mlen){
		temp = new char[len+1];
		memset(temp,0,len+1);
		memcpy(temp,string+start,len);
	}
	
	return temp;
}
int CString::Mid(int start,int len,CString& out){
	char *temp;

	if(len>0&&len<=mlen){
		temp = new char[len+1];
		memset(temp,0,len+1);
		memcpy(temp,string+start,len);
		out = temp;
	}
	
	return 0;
}
CString& CString::operator=(CString& cstring){
	//log_i("---0x%x",cstring.getString());
	Format("%s",cstring.getString());
	//log_i("---");
	return *this;
}
CString& CString::operator=(const char *str){
	Format("%s",str);
	return *this;
}
CStringArray::CStringArray(){
	mList = NULL;
	mLen = mMax = 0;
}
CStringArray::~CStringArray(){
	if(mList != NULL){
		delete[] mList;
		mList = NULL;
	}
	mLen = mMax = 0;
}
int CStringArray::addCString(CString& cstr){
	if(mLen>=mMax){
		CString *temp;
		int i,count;
		
		if(mMax == 0){
			mMax = 8;
		}else{
			mMax*=2; 
		}

		temp = new CString[mMax];

		for(i=0;i<mLen;i++){
			temp[i] = mList[i];
		}

		delete[] mList;
		mList = temp;
	}
	mList[mLen] = cstr;
	mLen++;
	return mLen;
}
int CStringArray::getCString(int index,CString& out){
	if(index<mLen && index>=0){
		out = mList[index];
		return index;
	}else{
		return -1;
	}
}
int CStringArray::getCount(){
	return mLen;
}