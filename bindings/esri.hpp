#pragma once
#include <libESRI.h>
#include <map>
#include <memory>
#include <functional>
#include <string>

class IEsriClient
{
public:
  virtual ~IEsriClient() = default;
  virtual const char* providePromptText() = 0;
  virtual const char* provideCommands() = 0;
  virtual void onCommand(const char* command) = 0;
  virtual void onAbortCommand() = 0;
};

class EsriInstance
{
public:
  EsriInstance(const std::string& welcomeMessage, std::function<std::unique_ptr<IEsriClient>(void*)> fnCreateClient)
    : welcomeMessage(welcomeMessage)
    , createClient(fnCreateClient)
    , esriInstance(nullptr)
  {
    esriInstance = EsriCreateInstance();
  }

  void Start(unsigned short port, int maxClients)
  {
    EsriSetHandlersForInstance(esriInstance,
                               this,
                               &EsriInstance::onWelcomeMessage_proxy,
                               &EsriInstance::getCurrentDirectory_proxy,
                               &EsriInstance::provideCommands_proxy,
                               &EsriInstance::onCommand_proxy,
                               &EsriInstance::onDisconnect_proxy,
                               &EsriInstance::onAbortCommand_proxy);
    EsriStartInstance(esriInstance, port, maxClients);
  }

  ~EsriInstance()
  {
    EsriDeleteInstance(esriInstance);
  }
private:
  const char* provideWelcomeMessage(void* client)
  {
    clientMap.emplace(client, createClient(client));

    //welcome message could be provided on a by-client level
    //Make it on instance level for simplicity reasons    
    return welcomeMessage.c_str();
  }

  const char* provideCurrentDictionary(void* client)
  {
    auto iteratorToClient = clientMap.find(client);
    if (iteratorToClient != clientMap.end())
    {
      return iteratorToClient->second->providePromptText();
    }
    return nullptr;
  }

  const char* provideCommands(void* client)
  {
    auto iteratorToClient = clientMap.find(client);
    if (iteratorToClient != clientMap.end())
    {
      return iteratorToClient->second->provideCommands();
    }
    return nullptr;
  }

  void onCommand(void* client, const char* command)
  {
    auto iteratorToClient = clientMap.find(client);
    if (iteratorToClient != clientMap.end())
    {
      iteratorToClient->second->onCommand(command);
    }
  }

  void onAbort(void* client)
  {
    auto iteratorToClient = clientMap.find(client);
    if (iteratorToClient != clientMap.end())
    {
      iteratorToClient->second->onAbortCommand();
    }
  }

  void onDisconnect(void* client)
  {
    clientMap.erase(client);
  }
private:
  static char const * const onWelcomeMessage_proxy(void* client, void* userData)
  {
    return static_cast<EsriInstance*>(userData)->provideWelcomeMessage(client);
  }

  static char const * const getCurrentDirectory_proxy(void* client, void* userData)
  {
    return static_cast<EsriInstance*>(userData)->provideCurrentDictionary(client);
  }

  static char const * const provideCommands_proxy(void* client, void* userData)
  {
    return static_cast<EsriInstance*>(userData)->provideCommands(client);
  }

  static void onCommand_proxy(void* client, const char * const command, void* userData)
  {
    static_cast<EsriInstance*>(userData)->onCommand(client, command);
  }

  static void onAbortCommand_proxy(void* client, void* userData)
  {
    static_cast<EsriInstance*>(userData)->onAbort(client);
  }

  static void onDisconnect_proxy(void* client, void* userData)
  {
    static_cast<EsriInstance*>(userData)->onDisconnect(client);
  }
private:
  void* esriInstance;
  const std::string welcomeMessage;
  /// Maps void* ersi clients to IEsriClient
  std::map<void*, std::unique_ptr<IEsriClient>> clientMap;
  /// function to create the application provided client
  std::function<std::unique_ptr<IEsriClient>(void*)> createClient;
};
