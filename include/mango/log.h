
#ifndef _MANGO_LOG_H
#define _MANGO_LOG_H

#define LOG_VERBOSE		0x01
#define LOG_DEBUG		0x02
#define LOG_INFO		0x04
#define LOG_WARING		0x08
#define LOG_ERROR		0x10
#define LOG_MARKER		0x20
#define LOG_ALL			0xff


#define _CRT_WARN           0
#define _CRT_ERROR          1
#define _CRT_ASSERT         2
#define _CRT_ERRCNT         3

namespace mango
{
	EXPORT_API void addLogRule(int type, const char *tag, const char *threadName);
	EXPORT_API void log(int type, const char* tag, const char* fileName, const char *functionName, int line, ...);
};

//检测输入
//#define _DEBUG_INPUT   

#define log_v_fun(...)	mango::log(LOG_VERBOSE,	NULL,	 NULL, __FUNCTION__, __LINE__, __VA_ARGS__);	 //v代表verbose啰嗦
#define log_d_fun(...)	mango::log(LOG_DEBUG,	NULL,	 NULL, __FUNCTION__, __LINE__, __VA_ARGS__);	 //debug 
#define log_i_fun(...)	mango::log(LOG_INFO,	NULL,	 NULL, __FUNCTION__, __LINE__, __VA_ARGS__);	 //一般提示性的消息information
#define log_w_fun(...)	mango::log(LOG_WARING,	NULL,	 NULL, __FUNCTION__, __LINE__, __VA_ARGS__);	 //warning
#define log_e_fun(...)	mango::log(LOG_ERROR,	NULL,	 NULL, __FUNCTION__, __LINE__, __VA_ARGS__);	 //error
#define log_m_fun(...)	mango::log(LOG_MARKER,	NULL,	 NULL, __FUNCTION__, __LINE__, __VA_ARGS__);	 //marker

#define logn_v_fun(...)	mango::log(LOG_VERBOSE,	LOG_TAG, NULL, __FUNCTION__, __LINE__, __VA_ARGS__); //v代表verbose啰嗦
#define logn_d_fun(...)	mango::log(LOG_DEBUG,	LOG_TAG, NULL, __FUNCTION__, __LINE__, __VA_ARGS__); //debug 
#define logn_i_fun(...)	mango::log(LOG_INFO,	LOG_TAG, NULL, __FUNCTION__, __LINE__, __VA_ARGS__); //一般提示性的消息information
#define logn_w_fun(...)	mango::log(LOG_WARING,	LOG_TAG, NULL, __FUNCTION__, __LINE__, __VA_ARGS__); //warning
#define logn_e_fun(...)	mango::log(LOG_ERROR,	LOG_TAG, NULL, __FUNCTION__, __LINE__, __VA_ARGS__); //error

#ifdef _DEBUG
#define log_v			log_v_fun	//v代表verbose啰嗦
#define log_d			log_d_fun	
#define log_i			log_i_fun	//一般提示性的消息information
#define log_w			log_w_fun	//warning
#define log_e			log_e_fun	//error
#define log_m			log_m_fun	//marker
#define logn_v			logn_v_fun	//v代表verbose啰嗦
#define logn_d			logn_d_fun	
#define logn_i			logn_i_fun	//一般提示性的消息information
#define logn_w			logn_w_fun	//warning
#define logn_e			logn_e_fun	//error
#else
#define log_v(...)
#define log_d(...)
#define log_i			log_i_fun
#define log_w			log_w_fun
#define log_e			log_e_fun
#define log_m			log_m_fun	//marker
#define logn_v(...)
#define logn_d(...)
#define logn_i			logn_i_fun
#define logn_w			logn_w_fun
#define logn_e			logn_e_fun
#endif


#ifndef WIN32
#ifdef _DEBUG
#define _ASSERT(expr) \
        do { if (!(expr)) \
			{while (1){log_i("_ASSERTE %s:%d\n", __FUNCTION__, __LINE__); sleep (1);}}} while (0)

#define _ASSERTE(expr) \
        do { if (!(expr)) \
			{while (1){log_i("_ASSERTE %s:%d\n", __FUNCTION__, __LINE__); sleep (1);}}} while (0)
#else
#define _ASSERT(expr)
#define _ASSERTE(expr)
#endif
#endif


#endif

