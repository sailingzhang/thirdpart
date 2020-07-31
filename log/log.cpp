#include <stdio.h>
#include <chrono>
#include "log.h"
#include <iostream>
#include <thread>
using namespace std;
using namespace std::chrono;
using namespace tvu_bridge;
extern InitRecogParams_t gInitParms;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INITIALIZE_EASYLOGGINGPP
#ifdef EASYLOGGING

#define MaxFileNum 10
void rolloutHandler(const char* filename, std::size_t size)
{
	/// 备份日志
	for (auto i = 0; i < MaxFileNum; i++) {
		auto targetfile = string(filename) +"."+ to_string(MaxFileNum - i);
		auto srcfile = string(filename) +"."+ to_string(MaxFileNum - i - 1);
		auto movefilecommand = string("mv ") + srcfile + " " + targetfile;
		LOG_INFO("begin move file,command=" << movefilecommand);
		system(movefilecommand.c_str());

		ofstream testfile("/tmp/test.txt");
		testfile << movefilecommand << endl;
	}
	auto movefilecommand = string("mv ") + filename + " " + filename + ".0";
	LOG_INFO("begin move file,command=" << movefilecommand);
	system(movefilecommand.c_str());

	ofstream testfile("/tmp/test.txt");
	testfile << movefilecommand << endl;
}

void rolloutHandler2(const char* filenameptr, std::size_t size)
{
	auto filename = string(filenameptr);
	system_clock::duration d = system_clock::now().time_since_epoch();
	auto mil = duration_cast<milliseconds>(d).count();
	rename(filename.c_str(),(string(filename)+"."+to_string(mil)).c_str());

	string command="rm -rf `ls -t "+filename+R"(*|sed -n 12,13p`)";
	auto fun=[=](){
		system(command.c_str());
		// ofstream testfile("/tmp/test.txt");
		// testfile <<command<< endl;
	};
	std::thread logrmthread(fun);
	logrmthread.detach();
}

void log_init(string logconf) {
	el::Configurations conf(logconf.c_str());
	el::Loggers::reconfigureLogger("sailingdefault", conf);
	el::Loggers::reconfigureAllLoggers(conf);
	el::Helpers::installPreRollOutCallback(rolloutHandler2);
}
#endif // EASYLOGGING

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef GLOG_LOG
//void log_init(char * program_name)
//{
//	google::InitGoogleLogging(program_name);
//	FLAGS_logbufsecs = 0;
//	google::InstallFailureSignalHandler();
//	google::SetLogDestination(google::GLOG_INFO, "./LOG/log_info_");
//	google::SetLogDestination(google::GLOG_WARNING, "./LOG/log_warnig_");
//	google::SetLogDestination(google::GLOG_ERROR, "./LOG/log_error_");
//	google::SetLogDestination(google::GLOG_FATAL, "./LOG/log_fatal_");
//
//	DIY_LOG(INFO) << "log init end ";
//	return;
//}
#endif 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef STDOUT_LOG


void log_init() {
	cout << "stdout log init" << endl;
	return;
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef LOG4CPLUS_LOG

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

Logger g_logger = Logger::getRoot();
void log_init() {
	//log4cplus::initialize();
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("sailing_clog4plus.properties"));
}

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef TVU_RECOG_LOG
//InitCognitionParamsMain_1_t::Log_ft gRemoteLog;

//void (CCALL *Log_ft)(LogLevel level, const char* msg)
void logFun(LogLevel level, const char* msg) {
	auto& initMain = GET_INIT_PARA_MAIN(gInitParms);
	if (NULL == initMain.logInfoFunc) {
		cout << msg<<endl;
	}else {
	//	cout << msg << endl;//test
		initMain.logInfoFunc(level, msg);

	}
}

void log_init(string logconf) {
//	gRemoteLog = logFun;
	cout << "it it TVU_RECOG_LOG init" << endl;
}


#endif // TVU_RECOG_LOG





