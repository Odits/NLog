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

	NLog(const char* _func_name, const char* _path, int line_num, bool _on_time = false, bool _on_path = false, bool _on_line = true, const char* _log_path = "", rules _rule = toConsole);

	NLog(const char* _func_name, const char* _path, int line_num, const char* _log_path = "", rules _rule = toConsole, bool _on_time = false, bool _on_path = false, bool _on_line = true)
		: NLog(_func_name, _path, line_num, _on_time, _on_path, _on_line, _log_path, _rule)	{	}

	NLog(const char* _path)
		: NLog("", _path, 0, false, false, true)	{	}

	void end(int line);

	~NLog();

	LineLogger output(const char* level, int line_num);

	LineLogger info(int line_num);   //函数返回一个临时对象，该对象在析构时会自动添加换行符。

	LineLogger warn(int line_num);

	LineLogger error(int line_num);

	static LineLogger s_info(int line_num, const char* _path);

	static LineLogger s_warn(int line_num, const char* _path);

	static LineLogger s_error(int line_num, const char* _path);

	void hex(int line_num, const char* str_name, const char* str, int len);

private:
	bool on_time, on_path, on_line;
	std::string func_name, msg, path, log_path;
	int end_line = 0;
	rules rule;
};


#define NLOGGER \
	NLog nlog(__func__, __FILE__, __LINE__)

#define LOG_INFO \
	nlog.info(__LINE__)

#define LOG_WARN \
	nlog.warn(__LINE__)

#define LOG_ERROR \
	nlog.error(__LINE__)

#define LOG_HEX(msg, len) \
	nlog.hex(__LINE__, #msg, msg, len)

#define N_return \
	nlog.end(__LINE__); \
	return


#endif // _NLOG_H_
