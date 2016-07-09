#pragma once
#include "IEsriHandler.h"
#include "IEsriTerminal.h"

namespace libESRI
{
  //Used by the Library to create a new handler
  //Each connected clients gets its own handler
  class IEsriHandlerFactory
  {
  public:
    virtual IEsriHandler* CreateNewHandler(IEsriTerminal* terminal) = 0;
    virtual void DeleteHandler(IEsriHandler* handler) = 0;
    virtual ~IEsriHandlerFactory() {}
  };
}