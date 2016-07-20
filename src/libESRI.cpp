#include "stdafx.h"
#include "../include/libESRI.h"
#include "EsriInstance.h"
#include "EsriHandlerFactory.h"
#include "EsriTerminal.h"
#include "EsriHandler.h"

extern "C"
{
void* EsriCreateInstance()
{
#ifdef _WINDOWS
  WSADATA wDat = { 0 };
  WSAStartup(MAKEWORD(2, 2), &wDat);
#endif
  return new libESRI::EsriInstance();
}
}
void EsriDeleteInstance(void* instance)
{
#ifdef _WINDOWS
  WSACleanup();
#endif
  delete instance;
}

void EsriSetHandlersForInstance(
  void* instance,
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand,
  fnHandlerOnExit onExit)
{
  auto* capiHandlerFactory = new libESRI::EsriHandlerFactory(onProvideWelcomeMessage, onGetCurrentDirectory, onProvideCommands, onCommitCommand, onExit);
  auto typedInstance = (libESRI::EsriInstance*)instance;
  typedInstance->SetAndTakeOwnershipOfHandlerFactory(capiHandlerFactory);
}


int EsriStartInstance(void* instance, unsigned short port, int maxConnections)
{
  auto typedInstance = (libESRI::EsriInstance*)instance;
  return typedInstance->Start(port, maxConnections) ? 1 : 0;
}

int EsriSendToTerminal(void* terminal, char const * const message, int messageLength)
{
  auto typedTerminal = (libESRI::EsriTerminal*)terminal;
  return typedTerminal->EsriSendToTerminal(message, messageLength) ? 1 : 0;
}

int EsriPromptTerminal(void* handler, void* terminal)
{
  auto typedHandler = (libESRI::EsriHandler*)handler;
  auto typedTerminal = (libESRI::EsriTerminal*)terminal;

  char const * const currentDir = typedHandler->OnGetCurrentDirectory();
  if (currentDir)
  {
    std::string currentDirWithPrompt = currentDir;
    currentDirWithPrompt += ">";
    return typedTerminal->EsriSendToTerminal(currentDirWithPrompt.c_str(), currentDirWithPrompt.size()) ? 1 : 0;
  }

  return true;
}
