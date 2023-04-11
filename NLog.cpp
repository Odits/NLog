#include "NLog.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static bool first = true;
#define VERSION "313.1"


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


NLog::LineLogger::LineLogger(NLog &_nlogger) : nlogger(_nlogger)
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
	nlogger.msg.clear();
}


NLog::LineLogger::~LineLogger()
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


NLog::NLog(const char* _func_name, const char* _path, int line_num, bool _on_time, bool _on_path, bool _on_line, const char* _log_path, rules _rule) :
	func_name(_func_name), rule(_rule), log_path(_log_path), path(_path), on_time(_on_time), on_path(_on_path), on_line(_on_line)
{
	if (on_path)
	{
		std::size_t found = path.find_last_of("/\\");
		path = path.substr(found + 1);
	}
	if (first)
	{
		msg = "\nversion = " + std::string(VERSION) + "\n";
		first = false;
	}
	if (!func_name.empty())
		info(line_num) << "-------------------Enter " + func_name + "-------------------";
}


void NLog::end(int line)
{
	end_line = line;
}


NLog::~NLog()
{
	if (!func_name.empty())
		info(end_line) << "------------------- Exit " + func_name + "-------------------";
}


NLog::LineLogger NLog::output(const char* level, int line_num)
{
	std::string tmp;
	if (on_time)
		tmp += "time:" + getTime() + " ";
	if (on_path)
		tmp += "file:" + path + " ";
	if (on_line)
		tmp += "line:" + std::to_string(line_num) + " ";

	msg += tmp + level;
	return LineLogger(*this);
}

NLog::LineLogger NLog::info(int line_num)
{
	return output("[INFO] ", line_num);
}

NLog::LineLogger NLog::warn(int line_num)
{
	return output("[WARN] ", line_num);
}

NLog::LineLogger NLog::error(int line_num)
{
	return output("[ERROR] ", line_num);
}

void NLog::hex(int line_num, const char* str_name, const char* str, int len)
{
	std::ostringstream oss;
	if (len > 32)	oss << "\n\t";
    oss << std::hex << std::uppercase << std::setfill('0');
    for (int i = 0; i < len; i++)
	{
		if (i && 0 == i % 42)	oss << "\n\t";
        oss << std::setw(2) << static_cast<int>(str[i]) << ' ';
	}
	info(line_num) << str_name << "(HEX) = " << oss.str();
}

//静态函数
NLog::LineLogger NLog::s_info(int line_num, const char* _path)
{
	NLog nlog("", _path, line_num, true, true, true);
	
	return nlog.info(line_num);
}

NLog::LineLogger NLog::s_warn(int line_num, const char *_path)
{
	NLog nlog("", _path, line_num, true, true, true);
	
	return nlog.warn(line_num);
}

NLog::LineLogger NLog::s_error(int line_num, const char *_path)
{
	NLog nlog("", _path, line_num, true, true, true);
	
	return nlog.error(line_num);
}
