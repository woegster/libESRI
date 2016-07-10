#pragma once
#include "TcpClient.h"

namespace libESRI
{
  class EsriHandler;

  class EsriClientThread
  {
  public:
    EsriClientThread(toni::TcpClient* tcpClient, EsriHandler* handler);
    void EntryPoint();
  private:
    bool SendWelcomeMessage();
    bool SendCurrentDirectory();
    bool HandleAutoComplete(std::string& input);
    void AutoComplete(std::string& input, std::string& commonStartOfAllCandidates, std::vector<std::string>& candidates);
  private:
    toni::TcpClient* m_tcpClient;
    EsriHandler* m_handler;
  };
}

