#pragma once
#include <map>
#include <string>
#include <vector>
#include <functional>

namespace libESRI
{
  class TelnetConnection;

  class EsriInternalCommands
  {
  public:
    EsriInternalCommands();
    bool ExecuteInternalCommand(const std::string& command, TelnetConnection& outputTo);
    const std::string& ProvideInternalCommands();
  private:
    void Prompt();
    void BuildCommandTable();
  private:
    std::string memstat();
  private:
    std::map<std::string, std::function<std::string()>> m_internalFunctionMap;
    std::string m_InternalCommandsAsString;
  };
}
