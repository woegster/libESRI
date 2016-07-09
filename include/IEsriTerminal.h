#pragma once

namespace libESRI
{
  class IEsriTerminal
  {
  public:
    virtual bool SendToTerminal(char const * const text, size_t textLen) = 0;
    virtual ~IEsriTerminal() {}
  };
}