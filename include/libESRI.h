#pragma once
#include "EsriExport.h"
#include "IEsriInstance.h"

/*
  libESRI - A telnet framework library for applications
*/

namespace libESRI
{
  extern "C"
  {
    ESRIEXPORT IEsriInstance* CreateLibEsriInstance();
    ESRIEXPORT void DeleteLibEsriInstance(IEsriInstance* instance);
  }
}