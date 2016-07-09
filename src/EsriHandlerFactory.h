#pragma once
#include "../include/libESRI.h"

namespace libESRI
{
  class EsriTerminal;
  class EsriHandler;

  class EsriHandlerFactory
  {
  public:
    EsriHandler* CreateNewHandler(EsriTerminal * terminal);
    void DeleteHandler(EsriHandler * handler);
  public:
    EsriHandlerFactory(
      fnHandlerOnProvideWelcomeMessage,
      fnHandlerOnGetCurrentDirectory,
      fnHandlerOnProvideCommands,
      fnHandlerOnCommitCommand);
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
  };
}
