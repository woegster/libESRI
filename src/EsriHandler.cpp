#include "stdafx.h"
#include "EsriHandler.h"

libESRI::EsriHandler::EsriHandler(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  libESRI::EsriTerminal* terminal)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
  , m_Terminal(terminal)
{

}

char const * const libESRI::EsriHandler::OnProvideWelcomeMessage()
{
  if (m_fnHandlerOnProvideWelcomeMessage)
  {
    return m_fnHandlerOnProvideWelcomeMessage(this);
  }
  return nullptr;
}

char const * const libESRI::EsriHandler::OnGetCurrentDirectory()
{
  if (m_fnHandlerOnGetCurrentDirectory)
  {
    return m_fnHandlerOnGetCurrentDirectory(this);
  }
  return nullptr;
}

char const * const libESRI::EsriHandler::OnProvideCommands()
{
  if (m_fnHandlerOnProvideCommands)
  {
    return m_fnHandlerOnProvideCommands(this);
  }
  return nullptr;
}

void libESRI::EsriHandler::OnCommitCommand(const char * const command)
{
  if (m_fnHandlerOnCommitCommand)
  {
    m_fnHandlerOnCommitCommand(this, m_Terminal, command);
  }
}
