#pragma once
namespace toni
{
  class TcpClient;
}

namespace libESRI
{
  class TelnetConnection;

  class EsriTerminal
  {
  public:
    EsriTerminal(libESRI::TelnetConnection& telnetConnection);
    bool EsriSendToTerminal(char const * const text, size_t textLen);
  private:
    libESRI::TelnetConnection& m_TelnetConnection;
  };
}