#pragma once
#include "SocketEndpoint.h"
#include <string>

namespace toni
{
  SocketEndpoint SocketEndpointFromIPv4(const std::string& ipv4, uint16_t port);
  SocketEndpoint SocketEndpointFromIPv6(const std::string& ipv6, uint16_t port);
  std::string IPv4FromSocketEndpoint(const SocketEndpoint& endpoint);
  std::string IPv6FromSocketEndpoint(const SocketEndpoint& endpoint);
  std::string IPv4WithPortFromSocketEndpoint(const SocketEndpoint& endpoint);
  std::string IPv6WithPortFromSocketEndpoint(const SocketEndpoint& endpoint);
}



