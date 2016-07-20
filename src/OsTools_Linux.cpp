#include "stdafx.h"
#include "OsTools.h"
#include "stdio.h"
#include "string.h"

namespace libESRI
{
  int parseLine(char* line)
  {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
  }


  uint64_t OsTools::GetMemUsageOfCurrentProcessBytes()
  {
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return ((uint64_t)result)*1024;
  }
}
