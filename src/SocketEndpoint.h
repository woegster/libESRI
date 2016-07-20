#pragma once
#include "networkCommon.h"

namespace toni
{
  class SocketEndpoint
  {
  public:
    SocketEndpoint();
    void Set(const sockaddr_in& inv4);
    void Set(const uint32_t inv4, uint16_t port);
    void Set(const sockaddr_in6& inv6);
    void Set(const in6_addr& inv6, uint16_t port);
    sockaddr_in6 const * const GetIpV6() const;
    sockaddr_in const * const GetIpV4() const;
    ADDRESS_FAMILY GetAddressFamily() const;
    sockaddr const * const GetIpGeneric() const;
    size_t GetGenericIpSize() const;
  private:
    void ZeroOutSockAddr();
  private:
    ADDRESS_FAMILY m_addressFamily;
    sockaddr_unified m_SockAddr;
  };

}

