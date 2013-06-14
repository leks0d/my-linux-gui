#ifndef WIN32
#define _USE_ANDROID_LOG

#ifdef _USE_ANDROID_LOG
#define LOG_TAG "mango"
#include <android/log.h>  
#endif
#endif

//#define ONLEY_ERROR_LOG


#include "mango.h"

namespace mango
{

	class LogRuleItem : public Object
	{
	public:
		LogRuleItem(void);
		~LogRuleItem(void);

	public:
		LIST_HEAD mList;

		int		mType;
		char*	mTagName;
		char*	mThreadName;

	public:
		void set(int type, const char *tag, const char *thread);
	};

	LogRuleItem::LogRuleItem(void)
	{
		mType = 0;
		mTagName = NULL;
		mThreadName = NULL;
	}

	LogRuleItem::~LogRuleItem(void)
	{
		safeFree(mTagName);
		safeFree(mThreadName);
	}

	void LogRuleItem::set(int type, const char *tag, const char *thread)
	{
		safeFree(mTagName);
		safeFree(mThreadName);

		int i;
		
		mType = type;

		if (tag)
		{
			i = strlen(tag);
			mTagName = (char *)malloc (i + 1);
			strcpy (mTagName, tag);
		}

		if (thread)
		{
			i = strlen(thread);
			mThreadName = (char *)malloc (i + 1);
			strcpy (mThreadName, thread);
		}
	}



	class Log : public Object
	{
	public:
		Log(void);
		virtual ~Log(void);

	private:
		Mutex  mMutex;
		List<LogRuleItem>  mRules;
		char   mLogText[1024];


	public:
		int  addRule(int type, const char *tag, const char *thread); 
		void log(int type, const char* tag, const char *threadName, const char* fileName, const char *functionName, int line, const char* format, va_list argList);
	};


	Log::Log(void)
	{
		
	}

	Log::~Log(void)
	{

	}

	int  Log::addRule(int type, const char *tag, const char *thread)
	{
		int count = -1;
		LogRuleItem* item;

		mMutex.lock();

		item = new LogRuleItem;
		item->set(type, tag, thread);
		mRules.add(item);

		count = mRules.getCount(); 

		mMutex.unlock();

		return count;
	}



	void Log::log(int type, const char* tag, const char *threadName, const char* fileName, const char *functionName, int line, const char* format, va_list argList)
	{
		LogRuleItem* rule;
		BOOL		 bOut = FALSE;

	//	va_list pArgList ;
#ifdef _USE_ANDROID_LOG
		int logPrio = ANDROID_LOG_INFO;
#endif
		int len = 0;

		mMutex.lock();

		rule = mRules.getFirst();

		//无规则，无名，则输出; 错误则一定输出
		if (((!rule) && (!tag)) \
			|| (type == LOG_ERROR || type == LOG_MARKER)) 
		{
			bOut = TRUE; 
		}

#ifndef WIN32
#define _stricmp	strcasecmp
#endif
		while(rule &&(!bOut))
		{
			if ((rule->mType & type) \
				&& ((tag == NULL) || (rule->mTagName == NULL) || (_stricmp(tag, rule->mTagName) == 0)) \
				&& ((threadName == NULL)|| (rule->mThreadName == NULL) || (_stricmp(threadName, rule->mThreadName) == 0)))
			{
				bOut = TRUE;
			}

			rule = mRules.getNext(rule, FALSE);
		}

		if (!bOut)
		{
			mMutex.unlock();
			return;
		}

		const char* typeName = "";
		switch(type)
		{
		case LOG_VERBOSE:
			typeName = "verbose";
#ifdef _USE_ANDROID_LOG
			logPrio = ANDROID_LOG_VERBOSE;
#endif
			break;

		case LOG_DEBUG:
			typeName = "debug";
#ifdef _USE_ANDROID_LOG
			logPrio = ANDROID_LOG_DEBUG;
#endif
			break;

		case LOG_INFO:
			typeName = "info";
#ifdef _USE_ANDROID_LOG
			logPrio = ANDROID_LOG_INFO;
#endif
			break;

		case LOG_WARING:
			typeName = "warning";
#ifdef _USE_ANDROID_LOG
			logPrio = ANDROID_LOG_WARN;
#endif
			break;

		case LOG_ERROR:
			typeName = "error";
#ifdef _USE_ANDROID_LOG
			logPrio = ANDROID_LOG_ERROR;
#endif
			break;

		case LOG_MARKER:
			typeName = "marker";
#ifdef _USE_ANDROID_LOG
			logPrio = ANDROID_LOG_INFO;
#endif
			break;
		}


		sprintf(mLogText, "%s(%d) : %s : ", functionName, line, typeName);

		len = strlen(mLogText);
		vsprintf(mLogText + len, format, argList); 
		len = strlen(mLogText);

		if (tag && threadName)
			sprintf(mLogText + len, " ---[%s (%s)]", tag, threadName); 
		else if (tag)
			sprintf(mLogText + len, " ---[%s ()]", tag); 
		else if (threadName)
			sprintf(mLogText + len, " ---[ (%s)]", threadName); 
		else
			sprintf(mLogText + len, " ---[ ]");

		len = strlen(mLogText);
		_ASSERT(len < 1024);

#ifdef 	_USE_ANDROID_LOG
		__android_log_write(logPrio, LOG_TAG, mLogText);
#else
		printf ("%s\n", mLogText);
#endif

		if (type == LOG_ERROR)
			_ASSERT(0);

		mMutex.unlock();
	}

	Log gLog;

	void log(int type, const char* tag, const char* fileName, const char *functionName, int line, ...)
	{
#ifdef ONLEY_ERROR_LOG
		if (type != LOG_ERROR && type != LOG_MARKER)
			return;
#endif
		const char *threadName;
		const char *format;
		va_list pArgList ;

		threadName = Thread::getCurrentName();

		va_start (pArgList, line);

		format = va_arg(pArgList, const char*);

		gLog.log(type, tag, threadName, fileName, functionName, line, format, pArgList);
		va_end (pArgList);
	}


	void addLogRule(int type, const char *tag, const char *threadName)
	{
		gLog.addRule(type, tag, threadName);
	}

}
