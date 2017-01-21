#include "stdafx.h"
#include "EsriHandlerFactory.h"
#include "EsriHandler.h"

libESRI::EsriHandlerFactory::EsriHandlerFactory(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  fnHandlerOnExit onExit,
  void* userData)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
  , m_fnOnHandlerExit(onExit)
  , m_userData(userData)
{

}

std::unique_ptr<libESRI::EsriHandler> libESRI::EsriHandlerFactory::CreateNewHandler(EsriTerminal& terminal)
{
  return std::unique_ptr<libESRI::EsriHandler>(new EsriHandler(
    m_fnHandlerOnProvideWelcomeMessage,
    m_fnHandlerOnGetCurrentDirectory,
    m_fnHandlerOnProvideCommands,
    m_fnHandlerOnCommitCommand,
    m_fnOnHandlerExit,
    terminal, m_userData));
}

