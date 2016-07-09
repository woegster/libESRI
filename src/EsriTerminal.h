#pragma once

namespace toni
{
  class TcpClient;
}

namespace libESRI
{
  class EsriTerminal
  {
  public:
    EsriTerminal(toni::TcpClient* tcpClient);
    bool EsriSendToTerminal(char const * const text, size_t textLen);
  private:
    toni::TcpClient* m_tcpClient;
  };
}