#pragma once
#include "SocketEndpoint.h"
#include <memory>
#include <vector>

#ifndef _WINDOWS
//on windows, closesocket() needs to be called to shutdown listening
//this may be den then called twice from different threads - use a mutex for protection
//this is not needed on unix, where shutdown is sufficent to shutdown a socket
#include <mutex>
#endif

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
  #ifndef _WINDOWS
    std::mutex m_closeMutex;
  #endif
  };
}



