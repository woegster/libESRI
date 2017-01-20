#pragma once
#include "TcpClient.h"
#include "libtelnet.h"
#include "ntshell.h"
#include <memory>

namespace libESRI
{
  class EsriHandler;
  class EsriInternalCommands;

  class EsriClientThread
  {
  public:
    EsriClientThread(toni::TcpClient* tcpClient, EsriHandler* handler);
    ~EsriClientThread();
    void EntryPoint();
  private:
    bool SendWelcomeMessage();
    bool SendCurrentDirectory();
    bool HandleAutoComplete(std::string& input);
    void AutoComplete(std::string& input, std::string& commonStartOfAllCandidates, std::vector<std::string>& candidates);
    void OnTelnetEvent(telnet_event_t *ev);
    int OnShellRequiresRead(char* targetBuffer, int bytesToRead);
    int OnShellWantsToWrite(const char* sourceData, int sourceDataSize);
    int OnShellCallback(const char* textFromTerminal);
  private:
    std::vector<char> m_recvBufferAfterTelnet;
    ntshell_t m_nthshell;
    telnet_t* m_TelnetLib;
    toni::TcpClient* m_tcpClient;
    EsriHandler* m_handler;
    EsriInternalCommands* m_InternalHandler;
  };
}

