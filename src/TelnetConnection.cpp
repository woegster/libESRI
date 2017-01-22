#include "stdafx.h"
#include "TelnetConnection.h"
#include "TcpClient.h"

libESRI::TelnetConnection::TelnetConnection(toni::TcpClient& tcpConnection)
  : m_TcpConnection(tcpConnection)
  , m_TelnetHandle(nullptr, [](telnet_t* toBeDeleted)
                            {
                              //telnet_free has no nullptr check, but its inside unique_ptr
                              telnet_free(toBeDeleted);
                            })
  , m_DataByte('\0')
  , m_hasReadDataByte(false)
  , m_hasNetworkError(false)
{
  static const telnet_telopt_t telopts[] = {
    { TELNET_TELOPT_ECHO,	TELNET_WILL, TELNET_DONT },
    { TELNET_TELOPT_SGA,	TELNET_WILL, TELNET_DO },
    { -1, 0, 0 }
  };


  m_TelnetHandle.reset(telnet_init(telopts, &TelnetConnection::libTelnetEventProxy, 0, this));
}

void libESRI::TelnetConnection::WriteText(const char* text, size_t textLength)
{
  telnet_send(m_TelnetHandle.get(), text, textLength);
}

char libESRI::TelnetConnection::ReadChar()
{
  m_hasReadDataByte = false;
  while (!m_hasReadDataByte)
  {
    char receiveBuffer = '\0';
    if (m_TcpConnection.Recv(&receiveBuffer, sizeof(receiveBuffer)) > 0)
    {
      //how this works: fetch 1 byte from the network and move that into the libtelnet state machine
      //the state machine does its work, eventually calling OnTelnetEvent() with one byte of data
      //this machanism makes sure that no bytes are lost
      telnet_recv(m_TelnetHandle.get(), &receiveBuffer, sizeof(receiveBuffer));
    }
    else
    {
      OnNetworkError();
      return '\0';
    }
  }

  return m_DataByte;
}

void libESRI::TelnetConnection::libTelnetEventProxy(telnet_t *telnet, telnet_event_t *ev, void *user_data)
{
  if (ev && user_data)
  {
    static_cast<TelnetConnection*>(user_data)->OnTelnetEvent(*ev);
  }
}

void libESRI::TelnetConnection::OnTelnetEvent(telnet_event_t& ev)
{
  switch (ev.type)
  {
    case TELNET_EV_DATA:
      m_DataByte = ev.data.buffer[0];
      m_hasReadDataByte = true;
      break;
    case TELNET_EV_SEND:
      if (m_TcpConnection.Send(ev.data.buffer, ev.data.size) <= 0)
      {
        OnNetworkError();
      }
      break;
  }
}

void libESRI::TelnetConnection::OnNetworkError()
{
  m_hasNetworkError = true;
}

bool libESRI::TelnetConnection::hasNetworkError() const
{
  return m_hasNetworkError;
}
