#include "libESRI.h"
#include <iostream>

class EsriHandlers
{
public:
  static char const * const onWelcomeMessage(void* client)
  {
    return "Welcome to ESRI interface of my App";
  }

  static char const * const getCurrentDirectory(void* client)
  {
    return "main";
  }

  static char const * const provideCommands(void* client)
  {
    return "create;close;open;";
  }

  static void onCommand(void* client, void* terminal, const char * const command)
  {
    char const response[] = "response";
    EsriSendToTerminal(terminal, response, sizeof(response));
    EsriPromptTerminal(client, terminal);
  }

  static void onDisconnect(void* client)
  {

  }
};

int main()
{
  void* esriInstance = EsriCreateInstance();
  EsriSetHandlersForInstance(esriInstance,
                             &EsriHandlers::onWelcomeMessage,
                             &EsriHandlers::getCurrentDirectory,
                             &EsriHandlers::provideCommands,
                             &EsriHandlers::onCommand,
                             &EsriHandlers::onDisconnect);

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