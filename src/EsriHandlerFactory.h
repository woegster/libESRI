#pragma once
#include "../include/libESRI.h"

namespace libESRI
{
  class EsriTerminal;
  class EsriHandler;

  class EsriHandlerFactory
  {
  public:
    std::unique_ptr<EsriHandler> CreateNewHandler(EsriTerminal& terminal);
  public:
    EsriHandlerFactory(
      fnHandlerOnProvideWelcomeMessage,
      fnHandlerOnGetCurrentDirectory,
      fnHandlerOnProvideCommands,
      fnHandlerOnCommitCommand,
      fnHandlerOnExit);
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
    fnHandlerOnExit m_fnOnHandlerExit;
  };
}
