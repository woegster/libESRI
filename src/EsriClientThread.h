#pragma once
#include "TcpClient.h"
#include "ntshell.h"
#include "TelnetConnection.h"
#include <memory>

namespace libESRI
{
  class EsriHandler;
  class EsriInternalCommands;

  class EsriClientThread
  {
  public:
    EsriClientThread(toni::TcpClient& tcpClient, EsriHandler& handler);
    void EntryPoint();
  private:
    bool SendWelcomeMessage();
    bool SendCurrentDirectory();
    bool HandleAutoComplete(std::string& input);
    void AutoComplete(std::string& input, std::string& commonStartOfAllCandidates, std::vector<std::string>& candidates);
    int OnShellRequiresRead(char* targetBuffer, int bytesToRead);
    int OnShellWantsToWrite(const char* sourceData, int sourceDataSize);
    int OnShellCallback(const char* textFromTerminal);
  private:
    ntshell_t m_nthshell;
    toni::TcpClient& m_tcpClient;
    EsriHandler& m_handler;
    TelnetConnection m_Telnet;
    std::unique_ptr<EsriInternalCommands> m_InternalHandler;
  };
}

