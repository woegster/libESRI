#pragma once

namespace libESRI
{
  //Must by implemented by the application
  //Represents a connected client
  class IEsriHandler
  {
  public:
    virtual char const * const OnProvideWelcomeMessage() = 0;
    virtual char const * const OnGetCurrentDirectory() = 0;
    virtual char const * const OnProvideCommands() = 0;
    virtual void OnCommitCommand(const char * const command) = 0;
    virtual ~IEsriHandler() {}
  };
}