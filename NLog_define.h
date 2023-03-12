#ifndef NLOG_DEFINE_H
#define NLOG_DEFINE_H

#include "NLog.h"


#define NLOGGER \
	NLogger nlog(__func__, __FILE__, __LINE__)

#define LOG_INFO \
	nlog.info(__LINE__)

#define LOG_WARN \
	nlog.warn(__LINE__)

#define LOG_ERROR \
	nlog.error(__LINE__)

#define return \
	nlog.end(__LINE__); \
	return


#endif // NLOG_DEFINE_H
