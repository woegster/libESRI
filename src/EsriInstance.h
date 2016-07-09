#pragma once
#include "TcpServer.h"
#include <thread>
#include <memory>

namespace libESRI
{
  class EsriHandlerFactory;

  class EsriInstance
  {
  public:
    bool Start(unsigned short port, int maxConnections);
    void SetAndTakeOwnershipOfHandlerFactory(EsriHandlerFactory* newHandlerFactory);
    virtual ~EsriInstance();
  private:
    void AcceptThread_Routine();
    void ClientThread_Routine(toni::TcpClient* tcpClient);
  private:
    EsriHandlerFactory* m_HandlerFactory = nullptr;
    toni::TcpServer m_TcpServer;
    std::unique_ptr<std::thread> m_NetworkAcceptThread;
  };
}