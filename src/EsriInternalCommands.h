#pragma once
#include <map>
#include <string>
#include <vector>
#include <functional>

namespace libESRI
{
  class EsriInternalCommands
  {
  public:
    EsriInternalCommands(std::function<void()>&& prompt, std::function<void(const char*, int)>&& sendToTerminal);
    bool ExecuteInternalCommand(const std::string& command);
    const std::string& ProvideInternalCommands();
  private:
    void BuildCommandTable();
  private:
    std::string memstat();
  private:
    std::map<std::string, std::function<std::string()>> m_internalFunctionMap;
    std::function<void()> m_promptTerminal;
    std::function<void(const char*, int)> m_sendToTerminal;
    std::string m_InternalCommandsAsString;
  };
}
