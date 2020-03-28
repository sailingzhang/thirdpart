#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdint.h>
#include <thread>

#include <sstream>
#include <string>

using namespace std;

//#define STDOUT_LOG
//#define LOG4CPLUS_LOG
//#define TVU_RECOG_LOG
#define EASYLOGGING
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef EASYLOGGING
#include "easylogging++.h"
void log_init(string logconf ="test.conf");

#define LOG_TRACE(logEvent)  LOG(TRACE)<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<"["<<this_thread::get_id()<<"] "<<logEvent;
#define LOG_DEBUG(logEvent) LOG(DEBUG)<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<"["<<this_thread::get_id()<<"] "<<logEvent;
#define LOG_INFO(logEvent)  LOG(INFO)<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<"["<<this_thread::get_id()<<"] "<<logEvent;
#define LOG_WARN(logEvent)	LOG(WARNING)<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<"["<<this_thread::get_id()<<"] "<<logEvent;
#define LOG_ERROR(logEvent) LOG(ERROR)<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<"["<<this_thread::get_id()<<"] "<<logEvent;
#define LOG_FATAL(logEvent) LOG(FATAL)<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<"["<<this_thread::get_id()<<"] "<<logEvent;

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef GLOG_LOG
#include <glog/logging.h>
#define DIY_LOG(severity) LOG(severity)
//#define DIY_LOG(severity) std::cout<<__FUNCTION__<<": "

#define FUN_BEGIN_LOG  DIY_LOG(INFO)<<__FUNCTION__<<" begin"<<endl;
#define FUN_END_LOG  DIY_LOG(INFO)<<__FUNCTION__<<" end"<<endl;
void log_init(char * program_name);
#endif




////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef STDOUT_LOG

void log_init();

#define LOG_TRACE(logEvent) cout<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent<<endl; 
#define LOG_DEBUG(logEvent) cout<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent<<endl; 
#define LOG_INFO(logEvent)  cout<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent<<endl; 
#define LOG_WARN(logEvent)	cout<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent<<endl; 
#define LOG_ERROR(logEvent) cout<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent<<endl; 
#define LOG_FATAL(logEvent) cout<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent<<endl; 

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef LOG4CPLUS_LOG

#include "include/log4cplus/logger.h"
#include "include/log4cplus/consoleappender.h"
#include "include/log4cplus/layout.h"
#include "include/log4cplus/configurator.h"
#include "include/log4cplus/loggingmacros.h"
using namespace log4cplus;
using namespace log4cplus::helpers;


extern Logger g_logger;
void log_init();

#define LOG_TRACE(logEvent) LOG4CPLUS_TRACE(g_logger,"["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent)
#define LOG_DEBUG(logEvent) LOG4CPLUS_DEBUG(g_logger,"["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent)
#define LOG_INFO(logEvent)  LOG4CPLUS_INFO(g_logger,"["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent)
#define LOG_WARN(logEvent)	LOG4CPLUS_WARN(g_logger,"["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent)
#define LOG_ERROR(logEvent) LOG4CPLUS_ERROR(g_logger,"["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent)
#define LOG_FATAL(logEvent) LOG4CPLUS_FATAL(g_logger,"["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent)

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef  TVU_RECOG_LOG




//extern InitRecogParams_t gInitParms;
//extern InitCognitionParamsMain_1_t::Log_ft gRemoteLog;
void log_init(string logconf = "test.conf");
void logFun(LogLevel level, const char* msg);




#define LOG_TRACE(logEvent)  do{stringstream FormatStream;\
								string strr;\
								FormatStream<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent; \
								strr = FormatStream.str();\
								const char * logInfo = strr.c_str();\
								logFun(DEBUGLVL,logInfo);}while (0);

#define LOG_DEBUG(logEvent)  LOG_TRACE(logEvent) 

#define LOG_INFO(logEvent)   do{stringstream FormatStream;\
								string strr;\
								FormatStream<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent; \
								strr = FormatStream.str();\
								const char * logInfo = strr.c_str();\
								logFun(INFOLVL,logInfo);}while (0);

#define LOG_WARN(logEvent)  do{stringstream FormatStream;\
								string strr;\
								FormatStream<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent; \
								strr = FormatStream.str();\
								const char * logInfo = strr.c_str();\
								logFun(WARNLVL,logInfo);}while (0);

#define LOG_ERROR(logEvent)  do{stringstream FormatStream;\
								string strr;\
								FormatStream<<" lib_vision ["<<__FUNCTION__<<":"<<__LINE__<<"] "<<"["<<this_thread::get_id()<<"]"<<logEvent; \
								strr = FormatStream.str();\
								const char * logInfo = strr.c_str();\
								logFun(ERRORLVL,logInfo);}while (0);

#define LOG_FATAL(logEvent)  LOG_ERROR(logEvent)

#endif


#endif
