#pragma once
#include "IEsriHandlerFactory.h"

namespace libESRI
{
  class IEsriInstance
  {
  public:
    virtual bool Start(unsigned short port, int maxConnections) = 0;
    virtual void SetHandlerFactory(IEsriHandlerFactory* newHandlerFactory) = 0;
    virtual ~IEsriInstance() {}
  };
}
