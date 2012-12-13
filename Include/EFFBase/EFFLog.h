// efflog.h: interface for the efflog class.
// �����־�ļ�
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
 * ������
	+ 1. err_msg;		���������Ϣ������
	+ 2. err_exit;		���������Ϣ�����ϵ���exit����errno
	+ 3. err_abort;	���������Ϣ�����ϵ���abort�˳�(���������linux���п��ܻ����core)
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
	 * ����ļ����Ƿ�ɹ�
	 * �˱�Ǳ�is_open��������
	 * ����˱����Ҫ�ǲ��Թ��캯����
	 * if the value is false, then m_file == stderr;
	 */
	bool m_file_is_open;
	FILE *m_file;					// �ļ����
};

/* ʹ�÷��� 
	EffLog log( "appInfo.Log");
	log.log_msg( "INFO:Test int 	:%d", 15 );
	log.log_msg( "INFO:Test float 	:%f", 1.09f );

*/