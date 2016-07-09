#pragma once
#include "../include/IEsriInstance.h"
#include "TcpServer.h"
#include <thread>
#include <memory>

namespace libESRI
{
  class EsriInstance : public IEsriInstance
  {
  public:
    bool Start(unsigned short port, int maxConnections) override;
    void SetAndTakeOwnershipOfHandlerFactory(IEsriHandlerFactory* newHandlerFactory) override;
    virtual ~EsriInstance();
  private:
    void AcceptThread_Routine();
    void ClientThread_Routine(toni::TcpClient* tcpClient);
  private:
    IEsriHandlerFactory* m_HandlerFactory = nullptr;
    toni::TcpServer m_TcpServer;
    std::unique_ptr<std::thread> m_NetworkAcceptThread;
  };
}