#pragma once
#include "IEsriHandlerFactory.h"

namespace libESRI
{
  //Instance of the library
  //Application must set its implementation IEsriHandlerFactory
  //Then start to bind it to a Tcpv4 Port
  class IEsriInstance
  {
  public:
    virtual bool Start(unsigned short port, int maxConnections) = 0;
    virtual void SetAndTakeOwnershipOfHandlerFactory(IEsriHandlerFactory* newHandlerFactory) = 0;
    virtual ~IEsriInstance() {}
  };
}
