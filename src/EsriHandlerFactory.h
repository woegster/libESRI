#pragma once
#include "../include/libESRI.h"
#include "../include/IEsriHandlerFactory.h"

namespace libESRI
{
  class EsriHandlerFactory : public IEsriHandlerFactory
  {
  public:
    virtual IEsriHandler* CreateNewHandler(IEsriTerminal * terminal) override;
    virtual void DeleteHandler(IEsriHandler * handler) override;
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
