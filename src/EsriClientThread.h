#pragma once
#include "TcpClient.h"
#include "../include/IEsriHandler.h"

namespace libESRI
{
  class EsriClientThread
  {
  public:
    EsriClientThread(toni::TcpClient* tcpClient, IEsriHandler* handler);
    void EntryPoint();
  private:
    bool SendWelcomeMessage();
    bool SendCurrentDirectory();
    bool HandleAutoComplete(const std::string& input);
    bool HandleCommitCommand(const std::string& input);
    void AutoComplete(const std::string& input, std::string& commonStartOfAllCandidates, std::vector<std::string>& candidates);
  private:
    toni::TcpClient* m_tcpClient;
    IEsriHandler* m_handler;
  };
}

