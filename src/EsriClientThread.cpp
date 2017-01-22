#include "stdafx.h"
#include "EsriClientThread.h"
#include "EsriHandler.h"
#include "EsriHandlerFactory.h"
#include "EsriInternalCommands.h"
#include "strOps.h"
#include <algorithm>
#include <string.h>


namespace libESRI
{
  EsriClientThread::EsriClientThread(toni::TcpClient& tcpClient, EsriHandlerFactory& handlerFactory)
    : m_Telnet(tcpClient) //holy fuck
    , m_Terminal(m_Telnet)//holy fuck
  {
    m_handler = handlerFactory.CreateNewHandler(m_Terminal);
  }

  bool EsriClientThread::isDisconnectChar(const char controlCode)
  {
    return controlCode == '\x4'; //CTRL-D
  }

  bool EsriClientThread::isAbortChar(const char controlCode)
  {
    return controlCode == '\x3'; //CTRL-C
  }


  void EsriClientThread::SendWelcomeMessage()
  {
    char const * const welcomeMessage = m_handler->OnProvideWelcomeMessage();
    if (welcomeMessage)
    {
      m_Telnet.WriteText(welcomeMessage, strlen(welcomeMessage));
    }
  }
  
  void EsriClientThread::SetAutocompleteToNtshell(ntshell_t& shell)
  {
    auto allCommandsAsString = m_InternalHandler.ProvideInternalCommands();
    auto* userCommands = m_handler->OnProvideCommands();
    if (userCommands)
    {
      allCommandsAsString += userCommands;
    }

    ntshell_completition_reset(&shell);
    auto allCommands = toni::Tokenize<std::string>(allCommandsAsString, ";");
    for (size_t i = 0; i < allCommands.size(); i++)
    {
      ntshell_completition_setAt(&shell, i, allCommands[i].c_str());
    }
  }

  int EsriClientThread::OnShellRequiresRead(char* targetBuffer, int bytesToRead)
  {
    if (m_Telnet.hasNetworkError())
      return 0;

    //allocate buffer
    std::vector<char> textBuffer;
    textBuffer.resize(bytesToRead);

    //read all chars from network
    for (int i = 0; i < bytesToRead; i++)
    {
      textBuffer[i] = m_Telnet.ReadChar();
      if (isDisconnectChar(textBuffer[i]))
      {
        return 0;
      }

      if (isAbortChar(textBuffer[i]))
      {
        m_handler->OnAbortCommand();
      }
    }

    //copy buffer into terminal
    memcpy(targetBuffer, textBuffer.data(), bytesToRead);
    return bytesToRead;
  }

  int EsriClientThread::OnShellWantsToWrite(const char* sourceData, int sourceDataSize)
  {
    if (m_Telnet.hasNetworkError())
      return -1;

    m_Telnet.WriteText(sourceData, sourceDataSize);
    return sourceDataSize;
  }

  int EsriClientThread::OnShellCallback(const char* textFromTerminal)
  {
    const auto trimmed = toni::TrimRight<std::string>(textFromTerminal);
    if (!m_InternalHandler.ExecuteInternalCommand(trimmed, m_Telnet))
    {
      m_handler->OnCommitCommand(trimmed.c_str());
    }    
    return 0;
  }
  
  void EsriClientThread::EntryPoint()
  {
    auto shell_read_proxy = [](char* buffer, int count, void* userData) -> int
    {
      return static_cast<EsriClientThread*>(userData)->OnShellRequiresRead(buffer, count);
    };

    auto shell_write_proxy = [](const char* buffer, int count, void* userData) -> int
    {
      return static_cast<EsriClientThread*>(userData)->OnShellWantsToWrite(buffer, count);
    };

    auto shell_callback_proxy = [](const char* buffer, void* userData) -> int
    {
      return static_cast<EsriClientThread*>(userData)->OnShellCallback(buffer);
    };

    SendWelcomeMessage();

    ntshell_t terminalEmulation;
    ntshell_init(&terminalEmulation, shell_read_proxy, shell_write_proxy, shell_callback_proxy, this);
    char const * const directory = m_handler->OnGetCurrentDirectory();
    if (directory)
    {
      ntshell_set_prompt(&terminalEmulation, directory);
    }
    else
    {
      ntshell_set_prompt(&terminalEmulation, "ESRI>");
    }
    
    
    SetAutocompleteToNtshell(terminalEmulation);
    ntshell_execute(&terminalEmulation);
  }
}
