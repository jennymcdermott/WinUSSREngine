#pragma once

#if defined(WIN32)
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
//#pragma comment(lib,"Ws2_32.lib")

#include <winternl.h>
#include <shellapi.h>
#endif

#if defined(__cplusplus)
#include <strsafe.h>
#include <tchar.h>
#include <sddl.h>
#include <string>

#include <ctime>
#include <random>
#include <time.h>
#include <string>
#include <istream>
#include <ostream>
#include <iomanip>

// This will be used for pseudo rng
#include <ctime> 
#include "WinUSSREngine/Base/Base.h"
#endif