#pragma once
#include "../include/IEsriTerminal.h"
#include "TcpClient.h"

namespace libESRI
{
  class EsriTerminal : public IEsriTerminal
  {
  public:
    EsriTerminal(toni::TcpClient* tcpClient);
    virtual bool SendToTerminal(char const * const text, size_t textLen) override;
    virtual ~EsriTerminal() {}
  private:
    toni::TcpClient* m_tcpClient;
  };
}