#pragma once
#include <string>
#include <vector>

namespace toni
{
  namespace details
  {
    bool isWhitespace(const char ch)
    {
      switch (ch)
      {
        case ' ':
        case '\t':
          return true;
      }

      return false;
    }
  }

  template<class stringClass>
  void ReplaceAll(stringClass& source, const stringClass& what, const stringClass& with)
  {
    if (what.empty())
      return;

    auto foundPos = source.find(what);
    while (foundPos != stringClass::npos)
    {
      source.replace(foundPos, what.length(), with);
      foundPos = source.find(what, foundPos);
    }
  }

  template<class stringClass>
  std::vector<stringClass> Tokenize(const stringClass& source, const stringClass& tokens)
  {
    std::vector<stringClass> result;

    size_t prevTok = 0;
    while (prevTok != stringClass::npos)
    {
      auto nextTok = source.find_first_of(tokens, prevTok);

      result.push_back(source.substr(prevTok, nextTok - prevTok));

      if (nextTok == stringClass::npos)
        break;

      prevTok = nextTok + 1;
    }

    return result;
  }

  template<class stringClass>
  stringClass TrimRight(const stringClass& source)
  {
    stringClass trimmed;
    for (size_t i = source.length()-1; i >= 0 && i != stringClass::npos; --i)
    {
      const auto curChar = source[i];
      if (!details::isWhitespace(curChar))
      {
        trimmed = curChar + trimmed;
      }
    }

    return trimmed;
  }
}
