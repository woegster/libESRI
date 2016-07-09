#pragma once
#include "EsriExport.h"

/*
  libESRI - A telnet framework library for applications
*/

typedef char const * const (ESRIAPI*fnHandlerOnProvideWelcomeMessage)(void* handler);
typedef char const * const (ESRIAPI*fnHandlerOnGetCurrentDirectory)(void* handler);
typedef char const * const (ESRIAPI*fnHandlerOnProvideCommands)(void* handler);
typedef void               (ESRIAPI*fnHandlerOnCommitCommand)(void* handler, void* terminal, const char * const command);

extern "C"
{
  ESRIEXPORT void* ESRIAPI EsriCreateInstance();
  ESRIEXPORT void ESRIAPI EsriDeleteInstance(void* instance);
  ESRIEXPORT void ESRIAPI EsriSetHandlersForInstance(void* instance, fnHandlerOnProvideWelcomeMessage, fnHandlerOnGetCurrentDirectory, fnHandlerOnProvideCommands, fnHandlerOnCommitCommand);
  ESRIEXPORT int ESRIAPI EsriStartInstance(void* instance, unsigned short port, int maxConnections);
  ESRIEXPORT int ESRIAPI EsriSendToTerminal(void* terminal, char const * const message, int messageLength);
}