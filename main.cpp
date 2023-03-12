#include "NLog_define.h"
#include <iostream>


int func()
{
	NLOGGER;
	LOG_INFO << "this is info log" << std::string("123");
	LOG_WARN << "this is warn log" << std::string("456");
	LOG_ERROR << "this is error log" << std::string("789");
	return 0;
}

int main()
{
	NLogger nlog(__func__, __FILE__, __LINE__, "./log.txt", NLogger::rules::toBoth, true, true, true);
	nlog.info(__LINE__) << "this is info log" << std::string("123");
	nlog.warn(__LINE__) << "this is warn log" << std::string("456");
	nlog.error(__LINE__) << "this is error log" << std::string("789");

	func();
	return 0;
}



