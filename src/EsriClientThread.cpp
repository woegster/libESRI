#include "stdafx.h"
#include "EsriClientThread.h"
#include "EsriHandler.h"
#include "EsriHandlerFactory.h"
#include "EsriInternalCommands.h"
#include "strOps.h"
#include <algorithm>
#include <string.h>
#include <ntshell.h>

namespace libESRI
{
  EsriClientThread::EsriClientThread(toni::TcpClient& tcpClient, EsriHandlerFactory& handlerFactory)
    : m_Telnet(tcpClient) //holy fuck
    , m_Terminal(m_Telnet)//holy fuck
  {
    m_handler = handlerFactory.CreateNewHandler(m_Terminal);
    m_InternalHandler.reset(new EsriInternalCommands(*m_handler));
  }

  bool EsriClientThread::isControlCodeToDisconnect(const char controlCode)
  {
    switch (controlCode)
    {
      case '\x3': //CTRL-C
      case '\x4': //CTRL-D
        return true;
    }

    return false;
  }


  bool EsriClientThread::SendWelcomeMessage()
  {
    char const * const welcomeMessage = m_handler->OnProvideWelcomeMessage();
    if (welcomeMessage)
    {
      m_Telnet.WriteText(welcomeMessage, strlen(welcomeMessage));
    }

    return true;    
  }

  bool EsriClientThread::SendCurrentDirectory()
  {
    return true;
  }

  void EsriClientThread::AutoComplete(std::string& input, std::string& commonStartOfAllCandidates, std::vector<std::string>& candidates)
  {
    auto* allCommandsAsString = m_handler->OnProvideCommands();
    if (!allCommandsAsString)
    {
      commonStartOfAllCandidates = input;
      return;
    }

    std::string allCommandsStringBuffer = allCommandsAsString;
    allCommandsStringBuffer += m_InternalHandler->ProvideInternalCommands();
    auto allCommands = toni::Tokenize<std::string>(allCommandsStringBuffer, ";");
    for (auto& candidate : allCommands)
    {
      if (!candidate.empty() && candidate.find(input) == 0)
        candidates.push_back(candidate);
    }

    if (candidates.empty())
    {
      commonStartOfAllCandidates = input;
      return;
    }

    if (candidates.size() == 1)
    {
      commonStartOfAllCandidates = candidates[0];
      return;
    }

    //candidates.size() now >= 2
    std::sort(candidates.begin(), candidates.end());

    std::string& first = candidates[0];
    std::string& last = candidates[candidates.size() - 1];

    size_t shortestCandidate = std::min(first.size(), last.size());
    commonStartOfAllCandidates = "";
    for (size_t i = 0; i < shortestCandidate; i++)
    {
      if (first[i] == last[i])
        commonStartOfAllCandidates += first[i];
      else
        break;
    }
  }

  bool EsriClientThread::HandleAutoComplete(std::string& input)
  {
    std::string autoCompletedInput;
    std::vector<std::string> otherCandidates;
    AutoComplete(input, autoCompletedInput, otherCandidates);

    if (otherCandidates.size() > 1)
    {
      std::string allCandidates = "\r\n";
      for (auto& candidate : otherCandidates)
      {
        allCandidates += '\t' + candidate + "\r\n";
      }

      input = autoCompletedInput;

      allCandidates += m_handler->OnGetCurrentDirectory();
      allCandidates += ">";
      allCandidates += input;
      //return m_tcpClient.Send(allCandidates.c_str(), (int)allCandidates.length()) > 0;
    }
    else
    {
      input = autoCompletedInput;

      std::string autoCompletedInputWithPrompt = "\r";
      autoCompletedInputWithPrompt += m_handler->OnGetCurrentDirectory();
      autoCompletedInputWithPrompt += ">";
      autoCompletedInputWithPrompt += input;
      //return m_tcpClient.Send(autoCompletedInputWithPrompt.c_str(), (int)autoCompletedInputWithPrompt.length()) > 0;
    }

    return false;
  }

  int EsriClientThread::OnShellRequiresRead(char* targetBuffer, int bytesToRead)
  {
    if (m_Telnet.hasNetworkError())
      return 0;

    //mostly the case: terminal request one char
    if (bytesToRead == 1)
    {
      targetBuffer[0] = m_Telnet.ReadChar();
      return isControlCodeToDisconnect(targetBuffer[0]) ? 0 : bytesToRead;
    }
    
    //allocate buffer
    std::vector<char> textBuffer;
    textBuffer.resize(bytesToRead);

    //read all chars from network
    for (int i = 0; i < bytesToRead; i++)
    {
      textBuffer[i] = m_Telnet.ReadChar();
      if (isControlCodeToDisconnect(textBuffer[i]))
      {
        return 0;
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
    m_handler->OnCommitCommand(textFromTerminal);
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

    SendWelcomeMessage();    
    ntshell_execute(&terminalEmulation);
  }
}
