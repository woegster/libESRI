#include "stdafx.h"
#include "EsriHandler.h"
#include "string.h"

libESRI::EsriHandler::EsriHandler(
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  fnHandlerOnExit onExit,
  fnHandlerOnAbortCommand onAbortCommand,
  std::function<void(void)>&& onPrompt,
  std::function<void(const char * const text, int textLength)>&& onSendText,
  void* userData)
  : m_fnHandlerOnProvideWelcomeMessage(onProvideWelcomeMessage)
  , m_fnHandlerOnGetCurrentDirectory(onGetCurrentDirectory)
  , m_fnHandlerOnProvideCommands(onProvideCommands)
  , m_fnHandlerOnCommitCommand(onCommitCommand)
  , m_fnHandlerOnExit(onExit)
  , m_fnHandlerOnAbortCommand(onAbortCommand)
  , m_onPrompt(std::move(onPrompt))
  , m_onSendText(std::move(onSendText))
  , m_userData(userData)
{

}

libESRI::EsriHandler::~EsriHandler()
{
  if (m_fnHandlerOnExit)
  {
    m_fnHandlerOnExit(this, m_userData);
  }
}

char const * const libESRI::EsriHandler::OnProvideWelcomeMessage()
{
  if (m_fnHandlerOnProvideWelcomeMessage)
  {
    return m_fnHandlerOnProvideWelcomeMessage(this, m_userData);
  }
  return nullptr;
}

char const * const libESRI::EsriHandler::OnGetCurrentDirectory()
{
  if (m_fnHandlerOnGetCurrentDirectory)
  {
    return m_fnHandlerOnGetCurrentDirectory(this, m_userData);
  }
  return nullptr;
}

char const * const libESRI::EsriHandler::OnProvideCommands()
{
  if (m_fnHandlerOnProvideCommands)
  {
    return m_fnHandlerOnProvideCommands(this, m_userData);
  }
  return nullptr;
}

void libESRI::EsriHandler::OnCommitCommand(const char * const command)
{
  if (m_fnHandlerOnCommitCommand)
  {
    m_fnHandlerOnCommitCommand(this, command, m_userData);
  }
}

void libESRI::EsriHandler::OnAbortCommand()
{
  if (m_fnHandlerOnAbortCommand)
  {
    m_fnHandlerOnAbortCommand(this, m_userData);
  }
}
void libESRI::EsriHandler::DoPromptTerminal()
{
  if (m_onPrompt)
  {
    m_onPrompt();
  }
}

void libESRI::EsriHandler::DoSendTextToTerminal(const char * const text, int textLength)
{
  if (m_onSendText)
  {
    m_onSendText(text, textLength);
  }
}