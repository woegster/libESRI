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

    m_TcpServer.ShutdownAllClients();
    for (auto& clientThread : m_clientThreads)
    {
      if (clientThread.joinable())
        clientThread.join();
    }
  }

  void EsriInstance::AcceptThread_Routine()
  {
    SetThreadNameOfCurrentThread("EsriAcptThrd");

    std::shared_ptr<toni::TcpClient> newClient = nullptr;
    while ((newClient = m_TcpServer.Accept()) != nullptr)
    {
      m_clientThreads.emplace_back(&EsriInstance::ClientThread_Routine, this, newClient);
    }
  }   

  void EsriInstance::ClientThread_Routine(std::shared_ptr<toni::TcpClient> tcpClient)
  {
    std::string threadName = "esri" + IPv4FromSocketEndpoint(tcpClient->GetEndpoint());
    SetThreadNameOfCurrentThread(threadName.c_str());
    
    EsriClientThread clientThread(*tcpClient, *m_HandlerFactory);
    clientThread.EntryPoint();
    tcpClient->Disconnect();
  }
}