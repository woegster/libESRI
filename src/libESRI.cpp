#include "stdafx.h"
#include "../include/libESRI.h"
#include "../include/IEsriHandlerFactory.h"
#include "EsriInstance.h"
#include "EsriHandlerFactory.h"

void* EsriCreateInstance()
{
  WSADATA wDat = { 0 };
  WSAStartup(MAKEWORD(2, 2), &wDat);
  return new libESRI::EsriInstance();
}

void EsriDeleteInstance(void* instance)
{
  WSACleanup();
  delete instance;
}

void EsriSetHandlersForInstance(
  void* instance,
  fnHandlerOnProvideWelcomeMessage onProvideWelcomeMessage,
  fnHandlerOnGetCurrentDirectory onGetCurrentDirectory,
  fnHandlerOnProvideCommands onProvideCommands,
  fnHandlerOnCommitCommand onCommitCommand)
{
  auto* capiHandlerFactory = new libESRI::EsriHandlerFactory(onProvideWelcomeMessage, onGetCurrentDirectory, onProvideCommands, onCommitCommand);
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
  auto typedTerminal = (libESRI::IEsriTerminal*)terminal;
  return typedTerminal->EsriSendToTerminal(message, messageLength) ? 1 : 0;
}