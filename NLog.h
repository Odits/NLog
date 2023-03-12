#ifndef _NLOG_H_
#define _NLOG_H_

#pragma once

#include <string>
#include <fstream>

std::string getTime();

class NLogger
{
public:
	enum rules { none, toConsole, toFile, toBoth };
	class LineLogger
	{
	private:
		NLogger &nlogger;
		std::ostream *os = nullptr;
		std::ofstream *log_file;
	public:
		LineLogger(NLogger &_nlogger);
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
	NLogger();

	NLogger(const char* _func_name, const char* _path, int line_num, const char* _log_path = "", rules _rule = toConsole, bool _on_time = false, bool _on_path = false, bool _on_line = true);

	void end(int line);

	~NLogger();

	LineLogger output(const char* level, int line_num);

	LineLogger info(int line_num);   //函数返回一个临时对象，该对象在析构时会自动添加换行符。

	LineLogger warn(int line_num);

	LineLogger error(int line_num);

private:
	bool on_time, on_path, on_line;
	std::string func_name, msg, path, log_path;
	int end_line = 0;
	rules rule = toConsole;
};




#endif //_NLOG_H_
