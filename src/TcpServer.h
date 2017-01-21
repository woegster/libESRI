#pragma once
#include "SocketEndpoint.h"
#include <memory>

namespace toni
{
  class TcpClient;

  class TcpServer
  {
  public:
    ~TcpServer();
    bool Start(const SocketEndpoint& localEndpoint, int Backlog);
    std::unique_ptr<TcpClient> Accept();
    void ShutdownListenSocket();
  private:
    void CleanUp();
  private:
    SOCKET m_listenSocket = INVALID_SOCKET;
    SocketEndpoint m_localEndpoint;
  };
}



