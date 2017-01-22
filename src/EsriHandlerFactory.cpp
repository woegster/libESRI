#include "stdafx.h"
#include "EsriHandlerFactory.h"
#include "EsriHandler.h"

libESRI::EsriHandlerFactory::EsriHandlerFactory(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  fnHandlerOnExit onExit,
  fnHandlerOnAbortCommand onAbortCommand,
  void* userData)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
  , m_fnOnHandlerExit(onExit)
  , m_fnHandlerOnAbortCommand(onAbortCommand)
  , m_userData(userData)
{

}

std::unique_ptr<libESRI::EsriHandler>
libESRI::EsriHandlerFactory::CreateNewHandler(std::function<void(void)>&& onPrompt,
                                              std::function<void(const char * const text, int textLength)>&& onSendText)
{
  return std::unique_ptr<libESRI::EsriHandler>(new EsriHandler(
    m_fnHandlerOnProvideWelcomeMessage,
    m_fnHandlerOnGetCurrentDirectory,
    m_fnHandlerOnProvideCommands,
    m_fnHandlerOnCommitCommand,
    m_fnOnHandlerExit,
    m_fnHandlerOnAbortCommand,
    std::move(onPrompt),
    std::move(onSendText),
    m_userData));
}

