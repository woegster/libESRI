#include "libESRI.h"
#include <iostream>
#include <cstring>

class EsriHandlers
{
public:
  static char const * const onWelcomeMessage(void* client, void* userData)
  {
    return "Welcome to ESRI interface of my App";
  }

  static char const * const getCurrentDirectory(void* client, void* userData)
  {
    return "esri@BusinessApp:/core# ";
  }

  static char const * const provideCommands(void* client, void* userData)
  {
    return "async;sync";
  }

  static void onCommand(void* client, void* terminal, const char * const command, void* userData)
  {
    if (strcmp(command, "async") == 0)
    {
      char const response[] = "doing async work\r\ncancel with CTRL-C";
      EsriSendToTerminal(terminal, response, sizeof(response));
    }

    if (strcmp(command, "sync") == 0)
    {
      char const response[] = "doing syncronus work - returning directly";
      EsriSendToTerminal(terminal, response, sizeof(response));
      EsriPromptTerminal(client);
    }
    
  }

  static void onAbortCommand(void* client, void* terminal, void* userData)
  {
    char const abort[] = "\r\naborted";
    EsriSendToTerminal(terminal, abort, sizeof(abort));
    EsriPromptTerminal(client);
  }

  static void onDisconnect(void* client, void* userData)
  {

  }
};

int main()
{
  void* esriInstance = EsriCreateInstance();
  EsriSetHandlersForInstance(esriInstance,
                             nullptr,
                             &EsriHandlers::onWelcomeMessage,
                             &EsriHandlers::getCurrentDirectory,
                             &EsriHandlers::provideCommands,
                             &EsriHandlers::onCommand,
                             &EsriHandlers::onDisconnect,
                             &EsriHandlers::onAbortCommand);

  const unsigned short port = 55104;
  EsriStartInstance(esriInstance, port, 5);


  std::cout << "This is the sample application.\n";
  std::cout << "It will block and do its other things\n";
  std::cout << " - but esri gives each thread a client\n";
  std::cout << "Connect to " << port <<" with telnet an use the application\n\n";

  std::cout << "Press enter here to quit this sample";
  std::cin.get();
  
  EsriDeleteInstance(esriInstance);
}