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
      fnHandlerOnAbortCommand,
      std::function<void(void)>&& onPrompt,
      libESRI::EsriTerminal&,
      void* userData);
    ~EsriHandler();

    char const * const OnProvideWelcomeMessage();
    char const * const OnGetCurrentDirectory();
    char const * const OnProvideCommands();
    void OnCommitCommand(const char * const command);
    void OnAbortCommand();
    void SendToTerminal(const char * const text) const;
    void DoPromptTerminal();
    
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
    fnHandlerOnAbortCommand m_fnHandlerOnAbortCommand;
    fnHandlerOnExit m_fnHandlerOnExit;
    libESRI::EsriTerminal& m_Terminal;
    std::function<void(void)> m_onPrompt;
    void* m_userData;
  };
}
