#pragma once
#include "../include/libESRI.h"

namespace libESRI
{
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
      std::function<void(const char * const text, int textLength)>&& onSendText,
      void* userData);
    ~EsriHandler();

    char const * const OnProvideWelcomeMessage();
    char const * const OnGetCurrentDirectory();
    char const * const OnProvideCommands();
    void OnCommitCommand(const char * const command);
    void OnAbortCommand();
    void DoPromptTerminal();
    void DoSendTextToTerminal(const char * const text, int textLength);
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
    fnHandlerOnAbortCommand m_fnHandlerOnAbortCommand;
    fnHandlerOnExit m_fnHandlerOnExit;
    std::function<void(void)> m_onPrompt;
    std::function<void(const char * const text, int textLength)> m_onSendText;
    void* m_userData;
  };
}
