#include "stdafx.h"
#include "EsriTerminal.h"

namespace libESRI
{
  EsriTerminal::EsriTerminal(toni::TcpClient* tcpClient)
    : m_tcpClient(tcpClient)
  {

  }

  bool EsriTerminal::SendToTerminal(char const * const text, size_t textLen)
  {
    return m_tcpClient->Send(text, (int)textLen) > 0;
  }
}