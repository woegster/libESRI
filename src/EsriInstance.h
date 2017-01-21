#pragma once
#include "TcpServer.h"
#include <thread>
#include <memory>
#include <vector>

namespace libESRI
{
  class EsriHandlerFactory;

  class EsriInstance
  {
  public:
    bool Start(unsigned short port, int maxConnections);
    void SetAndTakeOwnershipOfHandlerFactory(EsriHandlerFactory* newHandlerFactory);
    ~EsriInstance();
  private:
    void AcceptThread_Routine();
    void ClientThread_Routine(std::shared_ptr<toni::TcpClient> tcpClient);
  private:
    std::unique_ptr<EsriHandlerFactory> m_HandlerFactory;
    toni::TcpServer m_TcpServer;
    std::unique_ptr<std::thread> m_NetworkAcceptThread;
    std::vector<std::thread> m_clientThreads;
  };
}