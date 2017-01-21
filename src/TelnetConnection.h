#pragma once
#include "TcpClient.h"
#include <libtelnet.h>

namespace libESRI
{
  class TelnetConnection
  {
  public:
    TelnetConnection(toni::TcpClient& tcpConnection);
  public:
    void WriteText(const char* text, size_t textLength);
    char ReadChar();
    bool hasNetworkError() const;
  private:
    static void libTelnetEventProxy(telnet_t *telnet, telnet_event_t *ev, void *user_data);
    void OnTelnetEvent(telnet_event_t& ev);
    void OnNetworkError();
  private:
    toni::TcpClient& m_TcpConnection;
    std::unique_ptr<telnet_t, decltype(&telnet_free)> m_TelnetHandle;
    char m_DataByte;
    bool m_hasReadDataByte;
    bool m_hasNetworkError;
  };
}
