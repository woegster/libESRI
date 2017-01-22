#include "../bindings/esri.hpp"
#include <iostream>
#include <thread>
#include <cstring>

class MyEsriClient : public IEsriClient
{
public:
  MyEsriClient(void* clientFromLibrary)
    : esriClientForSendingText(clientFromLibrary)
  {

  }
  virtual const char* providePromptText()
  {
    return "core:~#";
  }

  virtual const char* provideCommands()
  {
    return "sync;async";
  }

  virtual void onCommand(const char* command)
  {
    if (strcmp(command, "async") == 0)
    {
      char start[] = "Doing Asynchronus work - use CTRL-C to abort";
      EsriSendToTerminal(esriClientForSendingText, start, sizeof(start));
      return;
    }

    if (strcmp(command, "sync") == 0)
    {
      char start[] = "Doing Synchronus work";
      EsriSendToTerminal(esriClientForSendingText, start, sizeof(start));

      std::this_thread::sleep_for(std::chrono::seconds(2));

      char stop[] = "finished";
      EsriSendToTerminal(esriClientForSendingText, stop, sizeof(stop));
      EsriPromptTerminal(esriClientForSendingText);
      return;
    }

    char unknwnCmd[] = "Unkown command";
    EsriSendToTerminal(esriClientForSendingText, unknwnCmd, sizeof(unknwnCmd));
    EsriPromptTerminal(esriClientForSendingText);
  }

  virtual void onAbortCommand()
  {
    char stop[] = "aborted";
    EsriSendToTerminal(esriClientForSendingText, stop, sizeof(stop));
    EsriPromptTerminal(esriClientForSendingText);
  }
private:
  void* esriClientForSendingText;
};

int main()
{
  EsriInstance myInstance("Welcome to Telnet Interface", [](void* esriClientForSendingText)
  {
    return std::unique_ptr<IEsriClient>(new MyEsriClient(esriClientForSendingText));
  });
  myInstance.Start(55104, 10);

  std::cout << "Press enter here to quit this sample" << std::endl;
  std::cin.get();
}