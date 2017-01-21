#include "stdafx.h"
#include "EsriTerminal.h"
#include "TelnetConnection.h"

namespace libESRI
{
  EsriTerminal::EsriTerminal(libESRI::TelnetConnection& telnetConnection)
    : m_TelnetConnection(telnetConnection)
  {

  }

  bool EsriTerminal::EsriSendToTerminal(char const * const text, size_t textLen)
  {
    m_TelnetConnection.WriteText(text, textLen);
    return m_TelnetConnection.hasNetworkError() == false;
  }
}