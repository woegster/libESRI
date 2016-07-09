#pragma once
#include "IEsriHandler.h"
#include "IEsriTerminal.h"

namespace libESRI
{
  class IEsriHandlerFactory
  {
  public:
    virtual IEsriHandler* CreateNewHandler(IEsriTerminal* terminal) = 0;
    virtual void DeleteHandler(IEsriHandler* handler) = 0;
    virtual ~IEsriHandlerFactory() {}
  };
}