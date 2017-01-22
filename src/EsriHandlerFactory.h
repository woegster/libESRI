#pragma once
#include "../include/libESRI.h"
#include <functional>

namespace libESRI
{
  class EsriHandler;

  class EsriHandlerFactory
  {
  public:
    EsriHandlerFactory(
      fnHandlerOnProvideWelcomeMessage,
      fnHandlerOnGetCurrentDirectory,
      fnHandlerOnProvideCommands,
      fnHandlerOnCommitCommand,
      fnHandlerOnExit,
      fnHandlerOnAbortCommand,
      void* userData);
  public:
    std::unique_ptr<EsriHandler> CreateNewHandler(std::function<void(void)>&& onPrompt,
                                                  std::function<void(const char *, int)>&& onSendText);  
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
