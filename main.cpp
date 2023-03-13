#include <iostream>
#include <cstring>
#include "NLog.h"

#define NLOGGER \
	NLogger nlog(__func__, __FILE__, __LINE__, true, false, true, "./log.txt", NLogger::rules::toBoth)

#define VERSION "v313.2"

int func1()
{
	NLOGGER;
	LOG_INFO << "this is info log:" << std::string("123");
	LOG_WARN << "this is warn log:" << std::string("456");
	LOG_ERROR << "this is error log:" << std::string("789");

	LOG_return 0;
}

int func2()
{
	NLOGGER;
	LOG_INFO << "this is info log" << std::string("123");

	char str1[320] = "7353454812341245756257353454812341245756252452343243245234324373534548123412457562573534548123412457562524523432432452343243";

	// nlog.hex(__LINE__, "str", str, 31);
	LOG_HEX(str1, strlen(str1));

	LOG_return 0;
}


int main()
{
	NLogger nlog(__func__, __FILE__, __LINE__, "./log.txt", NLogger::rules::toBoth, true, true, true);
	nlog.info(__LINE__) << "this is info log" << std::string("123");
	nlog.warn(__LINE__) << "this is warn log" << std::string("456");
	nlog.error(__LINE__) << "this is error log" << std::string("789");

	func1();
	func2();
	nlog.end(__LINE__);	return 0;
}



