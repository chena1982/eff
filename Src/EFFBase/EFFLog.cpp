// EffLog.cpp: implementation of the EffLog class.
// �����־�ļ�
//////////////////////////////////////////////////////////////////////

#include "EFFBasePCH.h"
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
	if (open(filename))					// ���ļ�ʧ��
		EffLog();
}

EffLog::~EffLog()
{
	fclose();
}

/* ���ļ�
 * return :  0 = success
 *			other = errno
 */
int EffLog::open(const char *filename)
{
	if (!filename || filename[0] == '\0')
		return -1;

	if (m_file_is_open)
		fclose();												// �ȹر��Ѵ򿪵��ļ�

	if (!(fopen_s(&m_file, filename, "a+")))					// ���ļ�ʧ��
	{
		m_file = stderr;
		char buffer[256];
		strerror_s(buffer, 256, errno);
		fprintf(stderr, "Open file error (%s) : %s\n", filename, buffer);
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
			char buffer[256];
			strerror_s(buffer, 256, errno);
			fprintf(stderr, "Error while close file : %s\n", buffer);
		}
		m_file_is_open = false;
	}

	return;
}

/*
 * ������
	+ 1. err_msg;		���������Ϣ������
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
 * ������
	+ 2. err_fexit;		���������Ϣ�����������Ĵ�������, ���ϵ���exit����errno
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
 * ������
	+ 3. err_fabort;	���������Ϣ�����������Ĵ�������, ���ϵ���abort�˳�(���������linux���п��ܻ����core)
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
	/* vfprintf ���ô��󣬵���fprintf�������׼���� */
	if (vfprintf(m_file, format, ap) == -1)
	{
		char buffer[256];
		strerror_s(buffer, 256, errno);
		fprintf(stderr, "vfprintf err ( %s ) : %s", format, buffer);
	}

	return;
}

void EffLog::vlog_exit(const char *format, va_list ap)
{
	/* vfprintf ���ô��󣬵���fprintf�������׼���� */
	if (vfprintf(m_file, format, ap) == -1)
	{
		char buffer[256];
		strerror_s(buffer, 256, errno);
		fprintf(stderr, "vfprintf err ( %s ) : %s", format, buffer);
	}

	exit(errno);
}

void EffLog::vlog_abort(const char *format, va_list ap)
{
	/* vfprintf ���ô��󣬵���fprintf�������׼���� */
	if (vfprintf(m_file, format, ap) == -1)
	{
		char buffer[256];
		strerror_s(buffer, 256, errno);
		fprintf(stderr, "vfprintf err ( %s ) : %s", format, buffer);
	}

	abort();
}

