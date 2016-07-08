#pragma once
#include "EsriExport.h"
#include "IEsriInstance.h"

namespace libESRI
{
  extern "C"
  {
    ESRIEXPORT IEsriInstance* CreateLibEsriInstance();
    ESRIEXPORT void DeleteLibEsriInstance(IEsriInstance* instance);
  }
}