#pragma once
#include "../include/libESRI.h"
#include "../include/IEsriHandler.h"
#include "../include/IEsriTerminal.h"

namespace libESRI
{
  class EsriHandler : public IEsriHandler
  {
  public:
    virtual char const * const OnProvideWelcomeMessage() override;
    virtual char const * const OnGetCurrentDirectory() override;
    virtual char const * const OnProvideCommands() override;
    virtual void OnCommitCommand(const char * const command) override;
  public:
    EsriHandler(
      fnHandlerOnProvideWelcomeMessage,
      fnHandlerOnGetCurrentDirectory,
      fnHandlerOnProvideCommands,
      fnHandlerOnCommitCommand,
      libESRI::IEsriTerminal*);
  private:
    fnHandlerOnProvideWelcomeMessage m_fnHandlerOnProvideWelcomeMessage;
    fnHandlerOnGetCurrentDirectory m_fnHandlerOnGetCurrentDirectory;
    fnHandlerOnProvideCommands m_fnHandlerOnProvideCommands;
    fnHandlerOnCommitCommand m_fnHandlerOnCommitCommand;
    libESRI::IEsriTerminal* m_Terminal;
  };
}
