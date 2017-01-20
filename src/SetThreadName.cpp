#include "stdafx.h"
#include "SetThreadName.h"

#ifdef _WINDOWS

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
  DWORD dwType; // Must be 0x1000.
  LPCSTR szName; // Pointer to name (in user addr space).
  DWORD dwThreadID; // Thread ID (-1=caller thread).
  DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
#endif

void SetThreadNameOfCurrentThread(const char* threadName)
{
#ifdef _WINDOWS
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = threadName;
  info.dwThreadID = -1;
  info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
  __try
  {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
  }
  __except (EXCEPTION_EXECUTE_HANDLER)
  {
  }
#pragma warning(pop)
#else
  pthread_setname_np(pthread_self(), threadName);
#endif
}
