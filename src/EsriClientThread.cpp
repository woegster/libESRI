#include "stdafx.h"
#include "EsriClientThread.h"
#include "EsriHandler.h"
#include "strOps.h"
#include <algorithm>

namespace libESRI
{
  bool IsTextOrNumber(const char c)
  {
    return c >= 32 && c < 127;
  }

  EsriClientThread::EsriClientThread(toni::TcpClient* tcpClient, EsriHandler* handler)
    : m_tcpClient(tcpClient)
    , m_handler(handler)
  {

  }

  bool EsriClientThread::SendWelcomeMessage()
  {
    char const * const welcomeMessage = m_handler->OnProvideWelcomeMessage();
    if (welcomeMessage)
    {
      size_t welcomeMessageLength = strlen(welcomeMessage);
      return m_tcpClient->Send(welcomeMessage, (int)welcomeMessageLength) > 0;
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

  void EsriClientThread::AutoComplete(const std::string& input, std::string& commonStartOfAllCandidates, std::vector<std::string>& candidates)
  {
    auto* allCommandsAsString = m_handler->OnProvideCommands();
    if (!allCommandsAsString)
    {
      commonStartOfAllCandidates = input;
      return;
    }

    auto allCommands = toni::Tokenize<std::string>(allCommandsAsString, ";");
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

  bool EsriClientThread::HandleAutoComplete(const std::string& input)
  {
    std::string autoCompletedInput;
    std::vector<std::string> otherCandidates;
    AutoComplete(input, autoCompletedInput, otherCandidates);

    if (otherCandidates.size() > 1)
    {
      std::string allCandidates = "\n";
      for (auto& candidate : otherCandidates)
      {
        allCandidates += '\t' + candidate + '\n';
      }

      allCandidates += m_handler->OnGetCurrentDirectory();
      allCandidates += ">";
      allCandidates += autoCompletedInput;
      return m_tcpClient->Send(allCandidates.c_str(), (int)allCandidates.length()) > 0;
    }
    else
    {
      autoCompletedInput = "\r" + autoCompletedInput;
      return m_tcpClient->Send(autoCompletedInput.c_str(), (int)autoCompletedInput.length()) > 0;
    }
  }
  
  void EsriClientThread::EntryPoint()
  {
    if (!SendWelcomeMessage())
      return;

    if (!SendCurrentDirectory())
      return;


    int recvVal = 0;
    std::vector<char> receiveBuffer(1024);
    while ((recvVal = m_tcpClient->Recv(&receiveBuffer[0], (int)receiveBuffer.size())) > 0)
    {
      std::string inputBuffer;
      bool sentResponse = false;
      for (size_t i = 0; i < recvVal && !sentResponse; i++)
      {
        const char curChar = receiveBuffer[i];
        switch (curChar)
        {
        case '\t':
          if (!HandleAutoComplete(inputBuffer))
            return;
          sentResponse = true;
          break;
        case '\n':
          m_handler->OnCommitCommand(inputBuffer.c_str());
          sentResponse = true;
          break;
        default:
          if (IsTextOrNumber(curChar))
            inputBuffer += curChar;
          break;
        }
      }
    }
  }
}