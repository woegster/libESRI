#pragma once
#include "../include/libESRI.h"

namespace libESRI
{
  class EsriTerminal;

  class EsriHandler
  {
  public:
    EsriHandler(
      fnHandlerOnProvideWelcomeMessage,
      fnHandlerOnGetCurrentDirectory,
      fnHandlerOnProvideCommands,
      fnHandlerOnCommitCommand,
      fnHandlerOnExit,
      libESRI::EsriTerminal&);
    ~EsriHandler();

    char const * const OnProvideWelcomeMessage();
    char const * const OnGetCurrentDirectory();
    char const * const OnProvideCommands();
    void OnCommitCommand(const char * const command);
    void SendToTerminal(const char * const text) const;
    
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
    fnHandlerOnExit m_fnHandlerOnExit;
    libESRI::EsriTerminal& m_Terminal;
  };
}
