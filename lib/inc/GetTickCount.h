#ifndef _WINDOWS

#include <sys/time.h>

class GetTickCount
{
protected:
	DWORD	m_dwTick;
public:
	GetTickCount()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);

		m_dwTick = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	}

	operator DWORD()
	{
		return m_dwTick;
	}
};

#endif
