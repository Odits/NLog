#include "NLog.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static bool first = true;
#define VERSION "421.1"


std::string getTime()
{
	using namespace std::chrono;										  // 获取当前时间点
	auto now = system_clock::now();										  // 获取自纪元以来的时间
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000; // 转换为time_t格式
	std::time_t t = system_clock::to_time_t(now);						  // 格式化输出
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

NLog::NLog(const char *_func_name, const char *_path, int line_num, bool _on_time, bool _on_path_line, const char *_log_path, rules _rule)
    : func_name(_func_name), rule(_rule), log_path(_log_path), path(_path), on_time(_on_time), on_path_line(_on_path_line)
{
    if (_on_path_line)
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
        info(end_line) << "-------------------Exit  " + func_name + "-------------------";
}

static int tmp_len = 0;

NLog::LineLogger NLog::output(const char *level, int line_num)
{
	std::string tmp;
	if (on_time)
        tmp += "[" + getTime() + "] ";
    if (on_path_line)
    {
        tmp += "[" + path + ":" + std::to_string(line_num) + "] ";

        unsigned int fill = 0;
        if (tmp.size() < tmp_len)           //新长度小于旧长度
            fill = (tmp_len - tmp.size());  //补齐到旧长度
        tmp.append(fill, ' ');
        tmp_len = tmp.size();
    }

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

void NLog::hex(int line_num, const char *str_name, const char *str, int len)
{
	std::ostringstream oss;
	if (len > 32)
		oss << "\n\t";
	oss << std::hex << std::uppercase << std::setfill('0');
	for (int i = 0; i < len; i++)
	{
		if (i && 0 == i % 42)
			oss << "\n\t";
		oss << std::setw(2) << static_cast<int>(str[i]) << ' ';
	}
	info(line_num) << str_name << "(HEX) = " << oss.str();
}

// 静态函数
NLog::LineLogger NLog::s_info(int line_num, const char *_path)
{
    NLog nlog("", _path, line_num, true, true);

	return nlog.info(line_num);
}

NLog::LineLogger NLog::s_warn(int line_num, const char *_path)
{
    NLog nlog("", _path, line_num, true, true);

	return nlog.warn(line_num);
}

NLog::LineLogger NLog::s_error(int line_num, const char *_path)
{
    NLog nlog("", _path, line_num, true, true);

	return nlog.error(line_num);
}

void NLog::s_hex(int line_num, const char* str_name, const char* str, int len, const char *_path)
{
    NLog nlog("", _path, 0, true, true);
    nlog.hex(line_num, str_name, str, len);
}

// 旧日志接口
char ntlog_buf[5120];

int messLog(const char *log_path, char const *_sourcefilename, size_t line, int level, int mode, char const *logdata, size_t logdatalen)
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
{
	if (level == NO_LOG)
		return 0;

    NLog nlog("", _sourcefilename, 0, true, true, log_path, NLog::rules::toBoth);

	if (mode == 1)
	{
		nlog.hex(line, "", logdata, logdatalen);
	}
	std::string logstr(logdata, logdatalen);
	switch (level)
	{
	case LOG_INFO:
		nlog.info(line) << logstr;
		break;
	case LOG_WARN:
		nlog.warn(line) << logstr;
		break;
	case LOG_ERROR:
		nlog.error(line) << logstr;
		break;
	}
    return 0;
}
