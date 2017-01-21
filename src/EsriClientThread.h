#pragma once
#include "TcpClient.h"
#include "TelnetConnection.h"
#include "EsriTerminal.h"
#include <memory>
#include <ntshell.h>

namespace libESRI
{
  class EsriHandlerFactory;
  class EsriHandler;
  class EsriInternalCommands;

  class EsriClientThread
  {
  public:
    EsriClientThread(toni::TcpClient& tcpClient, EsriHandlerFactory& handlerFactory);
    void EntryPoint();
  private:
    void SendWelcomeMessage();
    void SetAutocompleteToNtshell(ntshell_t& shell);
    int OnShellRequiresRead(char* targetBuffer, int bytesToRead);
    int OnShellWantsToWrite(const char* sourceData, int sourceDataSize);
    int OnShellCallback(const char* textFromTerminal);
  private:
    static bool isControlCodeToDisconnect(const char controlCode);
  private:
    std::unique_ptr<EsriInternalCommands> m_InternalHandler;
    std::unique_ptr<EsriHandler> m_handler;
    TelnetConnection m_Telnet;
    EsriTerminal m_Terminal;
  };
}

