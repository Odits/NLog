#include "NLog.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>


std::string getTime()
{
	using namespace std::chrono;                                            // 获取当前时间点
	auto now = system_clock::now();                                         // 获取自纪元以来的时间
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;   // 转换为time_t格式
	std::time_t t = system_clock::to_time_t(now);                           // 格式化输出
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%m-%d/%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
	return ss.str();
}


NLogger::LineLogger::LineLogger(NLogger &_nlogger) : nlogger(_nlogger)
{
	log_file = new std::ofstream();
	switch (nlogger.rule)
	{
	case none:
		return;
	case toConsole:
		os = &std::cout;
		break;
	case toBoth:
		os = &std::cout;
	case toFile:
		log_file->open(nlogger.log_path, std::ios::app);
	}
	if (os)
		*os << nlogger.msg;
	*log_file << nlogger.msg;
}


NLogger::LineLogger::~LineLogger()
{
	if (os)
		*os << std::endl;
	*log_file << std::endl;
	if (log_file->is_open())
	{
		log_file->close();
		delete log_file;
	}
}


NLogger::NLogger(const char* _func_name, const char* _path, int line_num, const char* _log_path, rules _rule, bool _on_time, bool _on_path, bool _on_line) :
	func_name(_func_name), rule(_rule), log_path(_log_path), path(_path), on_time(_on_time), on_path(_on_path), on_line(_on_line)
{
	info(line_num) << "-------------------Enter " + func_name + "-------------------";
}


void NLogger::end(int line)
{
	end_line = line;
}


NLogger::~NLogger()
{
	info(end_line) << "------------------- Exit " + func_name + "-------------------";
}

NLogger::LineLogger NLogger::output(const char* level, int line_num)
{
	std::string tmp;
	if (on_time)
		tmp += "time:" + getTime() + " ";
	if (on_path)
		tmp += "file:" + path + " ";
	if (on_line)
		tmp += "line:" + std::to_string(line_num) + " ";

	msg = tmp + level;
	return LineLogger(*this);
}

NLogger::LineLogger NLogger::info(int line_num)
{
	return output("[INFO] ", line_num);
}

NLogger::LineLogger NLogger::warn(int line_num)
{
	return output("[WARN] ", line_num);
}

NLogger::LineLogger NLogger::error(int line_num)
{
	return output("[ERROR] ", line_num);
}
