#include "stdafx.h"
#include "EsriHandler.h"
#include "string.h"

libESRI::EsriHandler::EsriHandler(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  fnHandlerOnExit onExit,
  libESRI::EsriTerminal& terminal)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
  , m_fnHandlerOnExit(onExit)
  , m_Terminal(terminal)
{

}

libESRI::EsriHandler::~EsriHandler()
{
  if (m_fnHandlerOnExit)
  {
    m_fnHandlerOnExit(this);
  }
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
    m_fnHandlerOnCommitCommand(this, &m_Terminal, command);
  }
}

void libESRI::EsriHandler::SendToTerminal(const char * const text) const
{
  EsriSendToTerminal(&m_Terminal, text, strlen(text));
}
