#include "stdafx.h"
#include "EsriInternalCommands.h"
#include "EsriHandler.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "OsTools.h"
#include "TelnetConnection.h"

namespace libESRI
{
  EsriInternalCommands::EsriInternalCommands(std::function<void()>&& prompt,
                                             std::function<void(const char*, int)>&& sendToTerminal)
    : m_promptTerminal(std::move(prompt))
    , m_sendToTerminal(std::move(sendToTerminal))
  {
    BuildCommandTable();

    m_InternalCommandsAsString = "";
    for (auto& command : m_internalFunctionMap)
    {
      m_InternalCommandsAsString += command.first;
      m_InternalCommandsAsString += ";";
    }
  }

  bool EsriInternalCommands::ExecuteInternalCommand(const std::string& command)
  {
    auto functionToExecute = m_internalFunctionMap.find(command);
    if (functionToExecute != m_internalFunctionMap.end())
    {
      std::string functionResult = functionToExecute->second();
      m_sendToTerminal(functionResult.c_str(), functionResult.length());
      m_promptTerminal();
      return true;
    }

    return false;
  }
  const std::string & EsriInternalCommands::ProvideInternalCommands()
  {
    return m_InternalCommandsAsString;
  }

  void EsriInternalCommands::BuildCommandTable()
  {
    m_internalFunctionMap["memstat"] = std::bind(&EsriInternalCommands::memstat, this);
  }

  std::string EsriInternalCommands::memstat()
  {
    double memSizeMiB = (double)OsTools::GetMemUsageOfCurrentProcessBytes();
    memSizeMiB /= 1024.0 * 1024.0;
    std::stringstream formatter;
    formatter << "Host App Memory Usage: " << std::setw(12) << std::setfill(' ') << std::setprecision(7) << memSizeMiB << " MiB";
    return formatter.str();
  }
}
