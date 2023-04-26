#ifndef _NLOG_H_
#define _NLOG_H_

#pragma once

#include <string>
#include <fstream>

std::string getTime();

class NLog
{
public:
	enum rules { none, toConsole, toFile, toBoth };
	class LineLogger
	{
	private:
		NLog &nlogger;
		std::ostream *os = nullptr;
		std::ofstream *log_file;
	public:
		LineLogger(NLog &_nlogger);
		~LineLogger();

		template <typename T>
		LineLogger& operator<<(const T& val)
		{
			if (os)
				*os << val;
			*log_file << val;
			return *this;
		}
	};

    NLog(const char* _func_name, const char* _path, int line_num, bool _on_time = false, bool _on_path_line = true, const char* _log_path = "", rules _rule = toConsole);

    NLog(const char* _func_name, const char* _path, int line_num, const char* _log_path = "", rules _rule = toConsole, bool _on_time = false, bool _on_path_line = true)
        : NLog(_func_name, _path, line_num, _on_time, _on_path_line, _log_path, _rule)	{	}

	NLog(const char* _path)
        : NLog("", _path, 0, false, true)	{	}

    NLog(const char* _func_name, const char* _path, int line_num)
        : NLog(_func_name, _path, line_num, false, true)	{	};

	void end(int line);

	~NLog();

	LineLogger output(const char* level, int line_num);

	LineLogger info(int line_num);   //函数返回一个临时对象，该对象在析构时会自动添加换行符。

	LineLogger warn(int line_num);

	LineLogger error(int line_num);

	static LineLogger s_info(int line_num, const char* _path);

	static LineLogger s_warn(int line_num, const char* _path);

	static LineLogger s_error(int line_num, const char* _path);

    static void s_hex(int line_num, const char* str_name, const char* str, int len, const char *_path);

	void hex(int line_num, const char* str_name, const char* str, int len);

private:
    bool on_time, on_path_line;
	std::string func_name, msg, path, log_path;
	int end_line = 0;
	rules rule;
};


#define NLOGGER \
    NLog nlog(__func__, __FILE__, __LINE__, true, true, "./log.txt", NLog::rules::toBoth)

#define NLOG_INFO \
	nlog.info(__LINE__)

#define NLOG_WARN \
	nlog.warn(__LINE__)

#define NLOG_ERROR \
	nlog.error(__LINE__)

#define NLOG_HEX(msg, len) \
	nlog.hex(__LINE__, #msg, msg, len)

#define N_return \
	nlog.end(__LINE__); \
	return


// 旧日志接口
#define NO_LOG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
int messLog(const char * log_path, char const * _sourcefilename, size_t line, int level, int mode, char const * logdata, size_t logdatalen);
/*
	打印日志,日志文件固定为ntbp8903log.txt,日志允许保存1M的文件，超出1M，日志将会被删除，重新新建文件记录新的日志。
	入参：
		log_path:日志保存路径.
		sourcefilename:源文件名,调用时直接传宏 __FILE__。
		line:打印日志的行，调用时直接传宏  __LINE__。
		level:宏定义级别，取值为LOG_INFO、LOG_WARN和LOG_ERROR。INFO最详细，包括WARN和ERROR。WARN次详细，包括ERROR。ERROR最少，只包含ERROR类日志。
		mode:打印方式，0 ---直接打印，1---每个字符按16进制格式逐一打印
		logdata：日志数据
		logdatalen:日志数据长度
	返回：0 成功，其他失败！
*/

#define _LOG_PATH_ "./ntbp8903.log"
extern char ntlog_buf[5120];

#define LogWritef(format, args...) \
	sprintf(ntlog_buf, format, args); \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf))

#define LogWriteERRORf(format, args...) \
	sprintf(ntlog_buf, format, args); \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_ERROR, 0, ntlog_buf, strlen(ntlog_buf))


#define LogWriteLine(msg) \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_INFO, 0, msg, strlen(msg))

#define LogWriteERROR(msg) \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_ERROR, 0, msg, strlen(msg))



#endif // _NLOG_H_
