#pragma once
#include "TcpClient.h"
#include "TelnetConnection.h"
#include <memory>
#include <ntshell.h>
#include "EsriInternalCommands.h"
#include <atomic>

namespace libESRI
{
  class EsriHandlerFactory;
  class EsriHandler;

  class EsriClientThread
  {
  public:
    EsriClientThread(toni::TcpClient& tcpClient, EsriHandlerFactory& handlerFactory);
    void EntryPoint();
  private:
    void SendWelcomeMessage();
    void SetAutocompleteToNtshell(ntshell_t& shell);
    void OnPrompt();
    void OnSendToTerminal(const char* sourceData, int sourceDataSize);
    int OnShellRequiresRead(char* targetBuffer, int bytesToRead);
    int OnShellWantsToWrite(const char* sourceData, int sourceDataSize);
    int OnShellCallback(const char* textFromTerminal);
  private:
    static bool isDisconnectChar(const char controlCode);
    static bool isAbortChar(const char controlCode);
  private:
    std::unique_ptr<EsriHandler> m_handler;
    EsriInternalCommands m_InternalHandler;
    TelnetConnection m_Telnet;
    ntshell_t terminalEmulation;
    std::atomic<bool> m_CommandIsRunning;
  };
}

