#if defined(WIN32) || defined(_WIN32_WCE)
	#include <winsock2.h>	
	#include <ws2tcpip.h>
#ifndef _WIN32_WCE
	#include <sys/types.h>
#endif // _WIN32_WCE
#endif // WIN32 || _WIN32_WCE

#include "rtptypes_win.h"
