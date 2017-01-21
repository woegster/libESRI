#include "stdafx.h"
#include "EsriInstance.h"
#include "EsriClientThread.h"
#include "EsriTerminal.h"
#include "EsriHandlerFactory.h"
#include "TcpClient.h"
#include "SocketEndpointConverter.h"
#include "SetThreadName.h"
#include "EsriInternalCommands.h"
#include "EsriHandler.h"

namespace libESRI
{
  void EsriInstance::SetAndTakeOwnershipOfHandlerFactory(EsriHandlerFactory* newHandlerFactory)
  {
    m_HandlerFactory.reset(newHandlerFactory);
  }

  bool EsriInstance::Start(unsigned short port, int maxConnections)
  {
    if (!m_NetworkAcceptThread)
    {
      toni::SocketEndpoint localhost;
      localhost.Set(INADDR_ANY, port);

      if (m_TcpServer.Start(localhost, maxConnections))
      {
        m_NetworkAcceptThread.reset(new std::thread(&EsriInstance::AcceptThread_Routine, this));
        return true;
      }
    }

    return false;
  }

  EsriInstance::~EsriInstance()
  {
    m_TcpServer.ShutdownListenSocket();
    if (m_NetworkAcceptThread && m_NetworkAcceptThread->joinable())
      m_NetworkAcceptThread->join();
  }

  void EsriInstance::AcceptThread_Routine()
  {
    SetThreadNameOfCurrentThread("ESRI: AcceptThread");

    std::unique_ptr<toni::TcpClient> newClient = nullptr;
    while ((newClient = m_TcpServer.Accept()) != nullptr)
    {
      std::thread clientThread(&EsriInstance::ClientThread_Routine, this, std::move(newClient));
      clientThread.detach();
    }
  }   

  void EsriInstance::ClientThread_Routine(std::unique_ptr<toni::TcpClient>&& tcpClient)
  {
    std::string threadName = "ESRI: " + IPv4WithPortFromSocketEndpoint(tcpClient->GetEndpoint());
    SetThreadNameOfCurrentThread(threadName.c_str());

    EsriTerminal terminalForClient(*tcpClient);
    auto handlerForThisClient(m_HandlerFactory->CreateNewHandler(terminalForClient));

    EsriClientThread clientThread(*tcpClient, *handlerForThisClient);
    clientThread.EntryPoint();
  }
}