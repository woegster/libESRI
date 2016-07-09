#include "stdafx.h"
#include "EsriInstance.h"
#include "EsriClientThread.h"
#include "EsriTerminal.h"
#include "EsriHandlerFactory.h"
#include "TcpClient.h"
#include "SocketEndpointConverter.h"
#include "SetThreadName.h"


namespace libESRI
{
  void EsriInstance::SetAndTakeOwnershipOfHandlerFactory(EsriHandlerFactory* newHandlerFactory)
  {
    delete m_HandlerFactory;
    m_HandlerFactory = newHandlerFactory;
  }

  bool EsriInstance::Start(unsigned short port, int maxConnections)
  {
    if (!m_NetworkAcceptThread)
    {
      toni::SocketEndpoint localhost;
      localhost.Set(INADDR_ANY, port);

      if (m_TcpServer.Start(localhost, maxConnections))
      {
        m_NetworkAcceptThread = std::make_unique<std::thread>(&EsriInstance::AcceptThread_Routine, this);
        return true;
      }
    }

    return false;
  }

  EsriInstance::~EsriInstance()
  {
    delete m_HandlerFactory;
    m_HandlerFactory = nullptr;

    if (m_NetworkAcceptThread && m_NetworkAcceptThread->joinable())
      m_NetworkAcceptThread->join();
  }

  void EsriInstance::AcceptThread_Routine()
  {
    SetThreadName((unsigned long)-1, "ESRI: AcceptThread");

    toni::TcpClient* newClient = nullptr;
    while ((newClient = m_TcpServer.Accept()) != nullptr)
    {
      std::thread clientThread(&EsriInstance::ClientThread_Routine, this, newClient);
      clientThread.detach();
    }
  }   

  void EsriInstance::ClientThread_Routine(toni::TcpClient* tcpClient)
  {
    std::string threadName = "ESRI: " + IPv4WithPortFromSocketEndpoint(tcpClient->GetEndpoint());
    SetThreadName((unsigned long)-1, threadName.c_str());

    EsriTerminal terminalForClient(tcpClient);
    auto* handlerForThisClient = m_HandlerFactory->CreateNewHandler(&terminalForClient);

    EsriClientThread clientThread(tcpClient, handlerForThisClient);
    clientThread.EntryPoint();   
    
    m_HandlerFactory->DeleteHandler(handlerForThisClient);
    delete tcpClient;
  }
}