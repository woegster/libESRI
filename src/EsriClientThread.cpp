#include "stdafx.h"
#include "EsriClientThread.h"
#include "EsriHandler.h"
#include "EsriInternalCommands.h"
#include "strOps.h"
#include <algorithm>
#include <string.h>

namespace libESRI
{
  EsriClientThread::EsriClientThread(toni::TcpClient* tcpClient, EsriHandler* handler)
    : m_tcpClient(tcpClient)
    , m_handler(handler)
    , m_TelnetLib(nullptr)
  {
    m_InternalHandler = new EsriInternalCommands(handler);
  }

  EsriClientThread::~EsriClientThread()
  {
    if (m_TelnetLib)
    {
      telnet_free(m_TelnetLib);
      m_TelnetLib = nullptr;
    }

    delete m_InternalHandler;
  }

  bool EsriClientThread::SendWelcomeMessage()
  {
    char const * const welcomeMessage = m_handler->OnProvideWelcomeMessage();
    if (welcomeMessage)
    {
      size_t welcomeMessageLength = strlen(welcomeMessage);
      telnet_send(m_TelnetLib, welcomeMessage, welcomeMessageLength);
    }

    return true;    
  }

  bool EsriClientThread::SendCurrentDirectory()
  {
    char const * const curDir = m_handler->OnGetCurrentDirectory();
    if (curDir)
    {
      std::string currentDirectory = curDir;
      currentDirectory += ">";
      return m_tcpClient->Send(currentDirectory.c_str(), (int)currentDirectory.length()) > 0;
    }

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
      return m_tcpClient->Send(allCandidates.c_str(), (int)allCandidates.length()) > 0;
    }
    else
    {
      input = autoCompletedInput;

      std::string autoCompletedInputWithPrompt = "\r";
      autoCompletedInputWithPrompt += m_handler->OnGetCurrentDirectory();
      autoCompletedInputWithPrompt += ">";
      autoCompletedInputWithPrompt += input;
      return m_tcpClient->Send(autoCompletedInputWithPrompt.c_str(), (int)autoCompletedInputWithPrompt.length()) > 0;
    }
  }

  void EsriClientThread::OnTelnetEvent(telnet_event_t *ev)
  {
    switch (ev->type)
    {
      case TELNET_EV_DATA:
        m_recvBufferAfterTelnet.resize(ev->data.size);
        memcpy(m_recvBufferAfterTelnet.data(), ev->data.buffer, ev->data.size);
        break;
      case TELNET_EV_SEND:
        m_tcpClient->Send(ev->data.buffer, ev->data.size);
        break;
    }
  }

  int EsriClientThread::OnShellRequiresRead(char* targetBuffer, int bytesToRead)
  {
    std::vector<char> rawNetworkData;
    rawNetworkData.resize(bytesToRead);
    m_tcpClient->Recv(rawNetworkData.data(), bytesToRead);

    telnet_recv(m_TelnetLib, rawNetworkData.data(), rawNetworkData.size());

    memcpy(targetBuffer, m_recvBufferAfterTelnet.data(), m_recvBufferAfterTelnet.size());
    m_recvBufferAfterTelnet.clear();

    return bytesToRead;
  }

  int EsriClientThread::OnShellWantsToWrite(const char* sourceData, int sourceDataSize)
  {
    //OutputDebugStringA(sourceData);
    telnet_send(m_TelnetLib, sourceData, sourceDataSize);
    return sourceDataSize;
  }

  int EsriClientThread::OnShellCallback(const char* textFromTerminal)
  {
    //OutputDebugStringA(textFromTerminal);
    return 0;
  }
  
  void EsriClientThread::EntryPoint()
  {
    auto TelnetEvent_proxy = [](telnet_t *telnet, telnet_event_t *ev, void *user_data)
    {
      EsriClientThread* clientThread = static_cast<EsriClientThread*>(user_data);
      clientThread->OnTelnetEvent(ev);
    };

    static const telnet_telopt_t telopts[] = {
      { TELNET_TELOPT_ECHO,	TELNET_WILL, TELNET_DONT },
      { TELNET_TELOPT_SGA,	TELNET_WILL, TELNET_DO },
      { -1, 0, 0 }
    };

    m_TelnetLib = telnet_init(telopts, TelnetEvent_proxy, 0, this);

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

    ntshell_init(&m_nthshell, shell_read_proxy, shell_write_proxy, shell_callback_proxy, this);
    ntshell_set_prompt(&m_nthshell, "ESRI>");

    ntshell_execute(&m_nthshell);

    if (!SendWelcomeMessage())
      return;
    //
    //if (!SendCurrentDirectory())
    //  return;


    int recvVal = 0;
    std::vector<char> receiveBuffer(1024);
    while ((recvVal = m_tcpClient->Recv(&receiveBuffer[0], (int)receiveBuffer.size())) > 0)
    {
      telnet_recv(m_TelnetLib, receiveBuffer.data(), receiveBuffer.size());
      memset(receiveBuffer.data(), 0, receiveBuffer.size());
    }
  }
}
