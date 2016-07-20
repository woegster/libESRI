#pragma once
#include "Platform.h"

namespace toni
{
  union sockaddr_unified
  {
    sockaddr_in6 ipv6;
    sockaddr_in ipv4;
  };
}
