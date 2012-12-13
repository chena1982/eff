// EffLog.cpp: implementation of the EffLog class.
// 输出日志文件
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "assert.h"
#include "EffLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EffLog::EffLog()
{
	m_file = stderr;
	m_file_is_open = false;
}

EffLog::EffLog(const char *filename)
{
	m_file = stderr;
	m_file_is_open = false;
	if (open(filename))					// 打开文件失败
		EffLog();
}

EffLog::~EffLog()
{
	fclose();
}

/* 打开文件
 * return :  0 = success
 *			other = errno
 */
int EffLog::open(const char *filename)
{
	if (!filename || filename[0] == '\0')
		return -1;

	if (m_file_is_open)
		fclose();												// 先关闭已打开的文件

	if (!(m_file = fopen(filename, "a+")))					// 打开文件失败
	{
		m_file = stderr;
		fprintf(stderr, "Open file error (%s) : %s\n", filename, strerror(errno));
		return (errno);
	}
	m_file_is_open = true;
	return 0;
}

void EffLog::fclose()
{
	if (m_file && (m_file != stderr))
	{
		if (::fclose(m_file) == EOF)
		{
			fprintf(stderr, "Error while close file : %s\n", strerror(errno));
		}
		m_file_is_open = false;
	}

	return;
}

/*
 * 错误处理：
	+ 1. err_msg;		输出错误信息，返回
 */
void EffLog::log_msg(const char *format, ...)
{
	/*if( g_eIntercept == IS_END )
		return;*/
	assert( m_file_is_open );
	va_list ap;
	va_start(ap, format);
	vlog_msg(format, ap);
	va_end(ap);

	fflush( m_file );
	return;
}

/*
 * 错误处理：
	+ 2. err_fexit;		输出错误信息，调用其它的错误处理函数, 马上调用exit返回errno
 */
void EffLog::log_exit(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vlog_exit(format, ap);
	va_end(ap);

	exit(errno);
}

/*
 * 错误处理：
	+ 3. err_fabort;	输出错误信息，调用其它的错误处理函数, 马上调用abort退出(这个函数在linux下有可能会产生core)
 */
void EffLog::log_abort(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vlog_abort(format, ap);
	va_end(ap);

	abort();
}


void EffLog::vlog_msg(const char *format, va_list ap)
{
	/* vfprintf 调用错误，调用fprintf输出到标准错误 */
	if (vfprintf(m_file, format, ap) == -1)
		fprintf(stderr, "vfprintf err ( %s ) : %s", format, strerror(errno));

	return;
}

void EffLog::vlog_exit(const char *format, va_list ap)
{
	/* vfprintf 调用错误，调用fprintf输出到标准错误 */
	if (vfprintf(m_file, format, ap) == -1)
		fprintf(stderr, "vfprintf err ( %s ) : %s", format, strerror(errno));

	exit(errno);
}

void EffLog::vlog_abort(const char *format, va_list ap)
{
	/* vfprintf 调用错误，调用fprintf输出到标准错误 */
	if (vfprintf(m_file, format, ap) == -1)
		fprintf(stderr, "vfprintf err ( %s ) : %s", format, strerror(errno));

	abort();
}

