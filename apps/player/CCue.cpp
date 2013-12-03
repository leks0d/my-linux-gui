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
/*  该函数负责加载CUE文件                                               */
/************************************************************************/
int CCue::file_load(const char *pfile)//初始化在每次加载文件的时候做 
{
	
	CFile myf;
	int len,ret,i;
	char buff[10000];

	
	memset(buff,0,sizeof(buff));
	ret=myf.Open(pfile,"r");
	if (ret==0)
	{
		return -1;//文件打开失败
	}
	
	len=myf.GetLength();
	if(len>=10000){
		return -1;
	}
	
	ret=myf.Read(buff,len);
	if (ret<len)
	{
		myf.Close();
		return -1;//读文件错误 
	}
	myf.Close();
	//return ret;
	m_strbuff.Format("%s",buff);

	//ret=this->performer_parser();
	//ret+=this->title_parser();
	ret=this->songs_parser();
	//m_filepath.Format("%s",pfile);
 
	return ret;//如果后续操作有错误，直接返回错误
}
/************************************************************************/
/* 该函数获取演奏者信息                                                 */
/************************************************************************/
int CCue::performer_parser()
{
	int off1,off2,off3;
	CString  str_cue,temp;
	char tag1[] = "PERFORMER" ,tag2[] = "\"",tag3[]="\r\n";

	off1 = m_strbuff.Find(tag1,0);//一个空格+一个“号
	
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
/* 该函数获取碟片主题名                                                 */
/************************************************************************/
int CCue::title_parser()
{
	int off1,off2;
	CString  str_cue,temp;
	char tag1[]="TITLE",tag2[]="\r\n";
	
	off1=m_strbuff.Find(tag1,0)+strlen(tag1)+1+1;//一个空格+一个“号 
	off2=m_strbuff.Find(tag2,off1)-1;

	if (off1==-1||off2==-1)//其中一个没有找到，破坏的文件
	{
		return -1;
	}
	
	if ((off2-off1)>64)//两个偏移太远，错误的文件 
	{
		return -1;
	}
	
	m_strtitle=m_strbuff.Mid(off1,off2-off1);
	log_i("m_strtitle=%s",m_strtitle.getString());
	return 0;
}
/************************************************************************/
/*根据偏移获取某单曲信息                                                */
/************************************************************************/
#if 0
int CCue::song_parser(int off,int index)
{
	int off1,off2,ret1,ret2,min,sec,ms;
	char  *tags[4]={"TITLE","\r\n","INDEX 01","PERFORMER"};//三个关键标签
	CString str2;

	ret1=m_strbuff.Find(tags[0],off);
	off1=ret1+strlen(tags[0])+2;
	
	ret2=m_strbuff.Find(tags[1],off1);
	off2=ret2-1;
	
	if (ret1==-1||ret2==-1)//其中一个没有找到，破坏的文件
	{
		return -1;
	}
	
//	if ((off2-off1)>128)//两个偏移太远，错误的文件 
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

	if (min<0||sec<0) //时间长度为负数 ，错误的文件
	{
		return -1;
	}
	
	m_songs[index].star = min*60 + sec;

	return 0;
}
#else
int CCue::song_parser(int off,int index)
{
	int poff,eoff,off1,off2,ret1,ret2,min,sec,ms;
	char  *tags[4]={"TITLE","\r\n","INDEX 01","PERFORMER"};//三个关键标签
	char  tag1[] = "\"",tag2[]=":",tag3[]=" ";		
	CString str2;

	poff = off;
//----------------------------TITLE-------------------------------------------------	
	ret1=m_strbuff.Find(tags[0],poff);//寻找TITLE
	if(ret1 == -1){
		return -1;
	}
	ret1+=strlen(tags[0]);//定位到TITLE 之后
	ret2=m_strbuff.Find(tag1,ret1);//找到第一个"
	eoff=m_strbuff.Find(tags[1],ret1);//找到TITLE这一行的换行符
	if(eoff == -1){
		return -1;
	}else if(ret2 == -1 || ret2>=eoff-2){//如果符号没有找到，或者找的"在下一行
		off1 = ret1+1;
		off2 = eoff-1;
	}else{
		off1 = ret2+1;
		ret2 = m_strbuff.Find(tag1,off1);//寻找第二个"
		if(ret2 == -1 || ret2 >= eoff){
			off2 = eoff-1;
		}else{
			off2 = ret2;
		}
	}
	m_strbuff.Mid(off1,off2-off1,m_songs[index].m_strname);
//----------------------------PERFORMER-------------------------------------------------		
	ret1=m_strbuff.Find(tags[3],poff);//寻找PERFORMER
	if(ret1 == -1){
		;
	}else{
		ret1+=strlen(tags[3]);//定位到PERFORMER 之后
		ret2=m_strbuff.Find(tag1,ret1);//找到第一个"
		eoff=m_strbuff.Find(tags[1],ret1);//找到PERFORMER 这一行的换行符
		if(eoff == -1){
			return -1;
		}else if(ret2 == -1 || ret2>=eoff-2){//如果符号没有找到，或者找的"在下一行,eoff-2是为了判断错误的情况:artist"\r\n
			off1 = ret1+1;
			off2 = eoff-1;
		}else{
			off1 = ret2+1;
			ret2 = m_strbuff.Find(tag1,off1);//寻找第二个"
			if(ret2 == -1 || ret2 >= eoff){
				off2 = eoff-1;
			}else{
				off2 = ret2;
			}
		}
		m_strbuff.Mid(off1,off2-off1,m_songs[index].m_strart);
	}
//----------------------------INDEX 01-------------------------------------------------			
	ret1 = m_strbuff.Find(tags[2],off);//find INDEX 01	
	if(ret1 == -1)
		return -1;			
	ret1+=strlen(tags[2]);
	eoff = m_strbuff.Find(tags[1],ret1);//find "\r\n"
	ret2 = m_strbuff.Find(tag2,ret1);//find symby ':'
	if(ret2 == -1 || ret2 >= eoff-1)
		return -1;
	
	if(m_strbuff.string[ret2-3]<='9' && m_strbuff.string[ret2-3]>='0'){
		off1 = ret2-3;//min start position 3
		m_strbuff.Mid(off1,3,str2);
		min = atoi(str2.getString());
	}else if(m_strbuff.string[ret2-2]<='9' && m_strbuff.string[ret2-2]>='0'){
		off1 = ret2-2;//min start position 2
		m_strbuff.Mid(off1,2,str2);
		min = atoi(str2.getString());
	}else if(m_strbuff.string[ret2-1]<='9' && m_strbuff.string[ret2-1]>='0'){
		off1 = ret2-1;//min start position 1
		m_strbuff.Mid(off1,1,str2);
		min = atoi(str2.getString());
	}
	
	m_strbuff.Mid(ret2+1,2,str2);
	sec = atoi(str2.getString());

	m_strbuff.Mid(ret2+4,2,str2);
	ms = atoi(str2.getString());
	
	if (min<0||sec<0||ms<0) //时间长度为负数 ，错误的文件
	{
		return -1;
	}
	
	m_songs[index].star = (min*60 + sec)*1000+ms*10;

	return 0;
}
#endif
/************************************************************************/
/* 该函数解析CUE文件中所有单曲信息                                      */
/************************************************************************/
int CCue::songs_parser()
{
	char tag[]="TRACK";
	int len=0,off=0,i;	

	while (1)
	{
		off = m_strbuff.Find(tag,off);
		if (off == -1)//找不到TRACE标签，错误的文件 
		{
			break;
		}
		
		off += strlen(tag);
		
		if(song_parser(off,this->m_total_song)>=0)
			this->m_total_song++;	
	}
	
	for (i=0;i<m_total_song-1;i++)
	{
		m_songs[i].len = m_songs[i+1].star - m_songs[i].star;
		if(m_songs[i].len > 1000)
			m_songs[i].len -= 1000;
	}
	
	m_songs[m_total_song-1].len = 0;
	
	log_i("songs_parser m_total_song=%d",m_total_song);
	
	return 0;
}
/************************************************************************/
/* 获得下一首                                                           */
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
/* 获得上一首                                                           */
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
/* 获得特定曲目                                                         */
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
	
	safeDelete(string);
	
	if(path == NULL)
	{
		mlen = 0;
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
//ret:0-fail,1-sucess.
int CString::toIneger(int *val){
	int ret = 0;
	if(string){
		//log_i("string=%s",string);
		ret = sscanf(string,"%d",val);
	}else{
		*val = 0;
		ret = 1;
	}
	return ret;
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
CString& CString::operator=(int n){
	char *ptr,str[50];

	ptr = str;
	sprintf(ptr,"%d",n);
	Format("%s",str);
	
	return *this;
}
bool CString::operator==(const char *str){
	int ret = 0;

	if(str == NULL){
		if(string == NULL)
			return true;
		else
			return false;
	}else if(string == NULL){
		return false;
	}

	ret = strcmp(string,str);

	if(ret == 0){
		return true;
	}else{
		return false;
	}
}
bool CString::operator!=(const char *str){
	return !(*this == str);
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
void CStringArray::remove(int val){
	int i;
	if(val>=0&&val<mLen){
		for(i=val;i<mLen-1;i++){
			mList[i] = mList[i+1];
		}
		mList[mLen-1] = NULL;
		mLen--;
	}
}
int CStringArray::addCString(CString& cstr){
	if(mLen>=mMax){
		CString *temp;
		int i,count;
		
		if(mMax == 0){
			mMax = ARRAY_LIST_NUM;
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
int CStringArray::addString(const char *str){
	CString cstr;
	cstr = str;
	addCString(cstr);
}
void CStringArray::addInteger(int val){
	CString cstr;
	cstr = val;
	addCString(cstr);
}
int CStringArray::setInteger(int index,int val){
	//log_i("setInteger");
	if(index<mLen && index>=0){
		mList[index] = val;
	}else{
		return -1;
	}
}
int CStringArray::setCString(int index,CString& cstr){
	if(index<mLen && index>=0){
		mList[index] = cstr;
	}else{
		return -1;
	}	
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
bool CStringArray::isEmpty(){
	if(mLen<=0)
		return true;
	else
		return false;
}
int CStringArray::isExiteStr(char *str){
	int i;

	for(i=0;i<mLen;i++){
		if(mList[i] == str)
			return i;
	}

	return -1;
}
CStringArray& CStringArray::operator=(CStringArray& cStringArray){
	int i,count;

	this->~CStringArray();
	count = cStringArray.getCount();
	
	for(i=0;i<count;i++){
		CString cstr;
		cStringArray.getCString(i,cstr);
		addCString(cstr);
		//log_i("%s",cstr.string);
	}
	return *this;
}
