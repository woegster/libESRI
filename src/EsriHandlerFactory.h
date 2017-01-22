#pragma once
#include "../include/libESRI.h"
#include <functional>

namespace libESRI
{
  class EsriTerminal;
  class EsriHandler;

  class EsriHandlerFactory
  {
  public:
    std::unique_ptr<EsriHandler> CreateNewHandler(EsriTerminal& terminal, std::function<void(void)>&& onPrompt);
  public:
    EsriHandlerFactory(
      fnHandlerOnProvideWelcomeMessage,
      fnHandlerOnGetCurrentDirectory,
      fnHandlerOnProvideCommands,
      fnHandlerOnCommitCommand,
      fnHandlerOnExit,
      fnHandlerOnAbortCommand,
      void* userData);
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
    fnHandlerOnExit m_fnOnHandlerExit;
    fnHandlerOnAbortCommand m_fnHandlerOnAbortCommand;
    void* m_userData;
  };
}
