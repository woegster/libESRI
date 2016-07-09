#include "stdafx.h"
#include "EsriHandlerFactory.h"
#include "EsriHandler.h"

libESRI::EsriHandlerFactory::EsriHandlerFactory(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  fnHandlerOnExit onExit)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
  , m_fnOnHandlerExit(onExit)
{

}

libESRI::EsriHandler * libESRI::EsriHandlerFactory::CreateNewHandler(EsriTerminal* terminal)
{
  return new EsriHandler(
    m_fnHandlerOnProvideWelcomeMessage,
    m_fnHandlerOnGetCurrentDirectory,
    m_fnHandlerOnProvideCommands,
    m_fnHandlerOnCommitCommand,
    terminal);
}

void libESRI::EsriHandlerFactory::DeleteHandler(libESRI::EsriHandler * handler)
{
  if (m_fnOnHandlerExit)
    m_fnOnHandlerExit(handler);
  delete handler;
}


