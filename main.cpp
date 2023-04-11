#include <iostream>
#include <cstring>
#include "NLog.h"

#define NLOGGER \
	NLog nlog(__func__, __FILE__, __LINE__, true, false, true, "./log.txt", NLog::rules::toBoth)

#define VERSION "v313.2"

int func1()
{
	NLOGGER;
	LOG_INFO << "this is info log:" << std::string("123");
	LOG_WARN << "this is warn log:" << std::string("456");
	LOG_ERROR << "this is error log:" << std::string("789");

	N_return 0;
}

int func2()
{
	NLOGGER;
	LOG_INFO << "this is info log" << std::string("123");

	char str1[320] = "7353454812341245756341245756253412457562573534548123412457562524523432432452343243";

	// nlog.hex(__LINE__, "str", str, 31);
	LOG_HEX(str1, strlen(str1));

	N_return 0;
}

int func3()
{
	int abc = 21387;
	NLog::s_info(__LINE__, __FILE__) << "this is a static info log" << abc << 66666666;
	NLog::s_warn(__LINE__, __FILE__) << "this is a static warn log" << abc;
	NLog::s_error(__LINE__, __FILE__) << "this is a static error log" << abc << 66666666;

	return 0;
}


NLog nlog(__FILE__);
int func4()
{
	nlog.info(__LINE__) << "This is an info message.";
	nlog.warn(__LINE__) << "This is a warning message.";
	nlog.error(__LINE__) << "This is an error message.";

	return 0;
}


int main()
{
	NLog nlog(__func__, __FILE__, __LINE__, "./log.txt", NLog::rules::toBoth, true, true, true);
	nlog.info(__LINE__) << "this is info log" << std::string("123");
	nlog.warn(__LINE__) << "this is warn log" << std::string("456");
	nlog.error(__LINE__) << "this is error log" << std::string("789");

	// func1();
	// func2();
	func3();
	func4();
	nlog.end(__LINE__);	return 0;
}



