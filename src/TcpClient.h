#pragma once
#include "SocketEndpoint.h"
#include <mutex>

namespace toni
{
  class TcpClient
  {
  public:
    TcpClient();
    TcpClient(SOCKET connectedSocket, SocketEndpoint endpointOfSocket);
    ~TcpClient();

    int Recv(void* writeableData, int BufferSize);
    int Send(void const * readableData, int BufferSize);
    const SocketEndpoint& GetEndpoint() const;
    bool ConnectTo(const SocketEndpoint& endpoint);
    void Shutdown();
    void Disconnect();
  private:
    SOCKET m_connectedSocket = INVALID_SOCKET;
    SocketEndpoint m_remoteAddress;
    std::mutex m_closeMutex;
  };
}