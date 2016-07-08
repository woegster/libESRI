#pragma once
#include "IEsriHandler.h"

namespace libESRI
{
  class IEsriHandlerFactory
  {
  public:
    virtual IEsriHandler* CreateNewHandler() = 0;
    virtual void DeleteHandler(IEsriHandler* handler) = 0;
    virtual ~IEsriHandlerFactory() {}
  };
}