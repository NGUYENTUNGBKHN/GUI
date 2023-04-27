#pragma once


#ifdef _WINDOWS

#else // _WINDOWS
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <string.h>
	#include <time.h>
	#include <sys/types.h>
	#include <pthread.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <sys/msg.h>
	#include <errno.h>
	#include <sys/select.h>
	#include <sys/time.h>
	#include <sys/select.h>
	#include <sys/stat.h>
	#include <dirent.h>
	#include <stdarg.h>
	#include <termios.h>
	#include <signal.h>
	#include <semaphore.h>
	#include <sys/ioctl.h>
	#include <poll.h>
	#include <sched.h>
	#include <sys/syscall.h>
	#include <sys/wait.h>
	#include <sys/socket.h>
#endif // LINUX

#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

typedef unsigned char			BYTE;
typedef unsigned short			WORD;
#ifdef _WINDOWS
#else
	typedef unsigned int 		DWORD;

	typedef long long 			INT64;
	typedef char 				CHAR;
	typedef wchar_t				WCHAR;
	#ifdef _UNICODE

	#else
		#define _T(X)			X
		typedef char			TCHAR;
	#endif
#endif
typedef unsigned long long		QWORD;


#include <Array.h>

#include <MapDefPtr.h>
#include <Map.h>

#include <MyString.h>
#include <GetTickCount.h>

#ifdef _UNICODE
	#define	T2M(X)	W2M(X)
	#define	M2T(X)	M2W(X)
	#define	T2W(X)	X
	#define	W2T(X)	X
	#define	TOM(X)	W2M(X)
	#define	TOW(X)	X
	#define	T2U(X)	W2U(X)
	#define	U2T(X)	U2W(X)
#else
	#define	T2M(X)	X
	#define	M2T(X)	X
	#define	T2W(X)	M2W(X)
	#define	W2T(X)	W2M(X)
	#define	TOM(X)	X
	#define	TOW(X)	M2W(X)
	#define	T2U(X)	String(X)
	#define	U2T(X)	String(X)
#endif


