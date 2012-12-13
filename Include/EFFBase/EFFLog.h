// efflog.h: interface for the efflog class.
// 输出日志文件
//////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdio>
#include <cerrno>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#ifdef	WIN32
	#define	vsnprintf _vsnprintf
#endif

/*
 * 错误处理：
	+ 1. err_msg;		输出错误信息，返回
	+ 2. err_exit;		输出错误信息，马上调用exit返回errno
	+ 3. err_abort;	输出错误信息，马上调用abort退出(这个函数在linux下有可能会产生core)
 */
class EffLog
{
public:
	void log_msg(const char *format, ...);
	void log_exit(const char *format, ...);
	void log_abort(const char *format, ...);

	void vlog_msg(const char *format, va_list ap);
	void vlog_exit(const char *format, va_list ap);
	void vlog_abort(const char *format, va_list ap);

	void fclose();
	int open(const char *filename);
	EffLog();
	EffLog(const char *filename);
	virtual ~EffLog();
	inline bool is_open()	{ return (m_file_is_open); }

private:
	/*
	 * 标记文件打开是否成功
	 * 此标记被is_open函数返回
	 * 定义此标记主要是测试构造函数用
	 * if the value is false, then m_file == stderr;
	 */
	bool m_file_is_open;
	FILE *m_file;					// 文件句柄
};

/* 使用方法 
	EffLog log( "appInfo.Log");
	log.log_msg( "INFO:Test int 	:%d", 15 );
	log.log_msg( "INFO:Test float 	:%f", 1.09f );

*/