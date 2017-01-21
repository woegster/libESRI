#pragma once
#include "SocketEndpoint.h"
#include <memory>
#include <vector>

namespace toni
{
  class TcpClient;

  class TcpServer
  {
  public:
    ~TcpServer();
    bool Start(const SocketEndpoint& localEndpoint, int Backlog);
    std::shared_ptr<TcpClient> Accept();
    void ShutdownListenSocket();
    void ShutdownAllClients();
  private:
    void CleanUp();
  private:
    SOCKET m_listenSocket = INVALID_SOCKET;
    SocketEndpoint m_localEndpoint;
    std::vector<std::shared_ptr<TcpClient>> m_clients;
  };
}



