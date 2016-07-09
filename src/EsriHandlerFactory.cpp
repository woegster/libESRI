#include "stdafx.h"
#include "EsriHandlerFactory.h"
#include "EsriHandler.h"

libESRI::EsriHandlerFactory::EsriHandlerFactory(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
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
  delete handler;
}


