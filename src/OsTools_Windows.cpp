#include "stdafx.h"
#include "OsTools.h"
#include "psapi.h"

namespace libESRI
{
  uint64_t OsTools::GetMemUsageOfCurrentProcessBytes()
  {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
  }
}