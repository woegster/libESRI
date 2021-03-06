#pragma once
/*
libESRI - A telnet framework for applications
*/


#ifdef BUILDING_ESRI
  #ifdef _WINDOWS
    #define ESRIEXPORT __declspec(dllexport)
  #else
    #define ESRIEXPORT __attribute__((visibility ("default")))
  #endif
#else
#define ESRIEXPORT
#endif

#ifdef _WINDOWS
  #define ESRIAPI __stdcall
#else
  #define ESRIAPI
#endif

typedef char const * const (ESRIAPI*fnHandlerOnProvideWelcomeMessage)(void* handler, void* userData);
typedef char const * const (ESRIAPI*fnHandlerOnGetCurrentDirectory)(void* handler, void* userData);
typedef char const * const (ESRIAPI*fnHandlerOnProvideCommands)(void* handler, void* userData);
typedef void               (ESRIAPI*fnHandlerOnCommitCommand)(void* handler, const char * const command, void* userData);
typedef void               (ESRIAPI*fnHandlerOnAbortCommand)(void* handler, void* userData);
typedef void               (ESRIAPI*fnHandlerOnExit)(void* handler, void* userData);

extern "C"
{
  ESRIEXPORT void* ESRIAPI EsriCreateInstance();
  ESRIEXPORT void ESRIAPI EsriDeleteInstance(void* instance);
  ESRIEXPORT void ESRIAPI EsriSetHandlersForInstance(void* instance, void* userData, fnHandlerOnProvideWelcomeMessage, fnHandlerOnGetCurrentDirectory, fnHandlerOnProvideCommands, fnHandlerOnCommitCommand, fnHandlerOnExit, fnHandlerOnAbortCommand);
  ESRIEXPORT int ESRIAPI EsriStartInstance(void* instance, unsigned short port, int maxConnections);
  ESRIEXPORT void ESRIAPI EsriSendToTerminal(void* handler, char const * const message, int messageLength);
  ESRIEXPORT void ESRIAPI EsriPromptTerminal(void* handler);
}
