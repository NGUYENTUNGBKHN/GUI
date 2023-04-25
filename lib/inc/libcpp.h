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
#endif
typedef unsigned long long		QWORD;


