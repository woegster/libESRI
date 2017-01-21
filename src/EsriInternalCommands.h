#pragma once
#include <map>
#include <string>
#include <vector>
#include <functional>

namespace libESRI
{
  class EsriHandler;

  class EsriInternalCommands
  {
  public:
    EsriInternalCommands(EsriHandler& realHandler);
    bool ExecuteInternalCommand(const std::string& command);
    const std::string& ProvideInternalCommands();
  private:
    void Prompt();
    void BuildCommandTable();
  private:
    std::string memstat();
  private:
    std::map<std::string, std::function<std::string()>> m_internalFunctionMap;
    std::string m_InternalCommandsAsString;
    EsriHandler& m_realHandler;
  };
}
