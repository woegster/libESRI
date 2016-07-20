#include "stdafx.h"
#include "EsriInternalCommands.h"
#include "EsriHandler.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "OsTools.h"

namespace libESRI
{
  EsriInternalCommands::EsriInternalCommands(EsriHandler* const realHandler)
    : m_realHandler(realHandler)
  {
    BuildCommandTable();

    m_InternalCommandsAsString = "";
    for (auto& command : m_internalFunctionMap)
    {
      m_InternalCommandsAsString += ";";
      m_InternalCommandsAsString += command.first;
    }
  }

  bool EsriInternalCommands::ExecuteInternalCommand(const std::string & command)
  {
    auto functionToExecute = m_internalFunctionMap.find(command);
    if (functionToExecute != m_internalFunctionMap.end())
    {
      std::string functionResult = functionToExecute->second();
      m_realHandler->SendToTerminal(functionResult.c_str());
      Prompt();
      return true;
    }

    return false;
  }
  const std::string & EsriInternalCommands::ProvideInternalCommands()
  {
    return m_InternalCommandsAsString;
  }
  void EsriInternalCommands::Prompt()
  {
    std::string curDir = m_realHandler->OnGetCurrentDirectory();
    curDir += ">";
    m_realHandler->SendToTerminal(curDir.c_str());
  }

  void EsriInternalCommands::BuildCommandTable()
  {
    m_internalFunctionMap["memstat"] = std::bind(&EsriInternalCommands::memstat, this);
    m_internalFunctionMap["uptime"] = std::bind(&EsriInternalCommands::memstat, this);
  }

  std::string EsriInternalCommands::memstat()
  {
    double memSizeMiB = (double)OsTools::GetMemUsageOfCurrentProcessBytes();
    memSizeMiB /= 1024.0 * 1024.0;
    std::stringstream formatter;
    formatter << "\r\nHost App Memory Usage: " << std::setw(12) << std::setfill(' ') << std::setprecision(7) << memSizeMiB << " MiB\r\n";
    return formatter.str();
  }
}
