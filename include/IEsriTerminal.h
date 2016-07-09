#pragma once

namespace libESRI
{
  //The terminal is provided by the library for the application
  class IEsriTerminal
  {
  public:
    virtual bool EsriSendToTerminal(char const * const text, size_t textLen) = 0;
    virtual ~IEsriTerminal() {}
  };
}