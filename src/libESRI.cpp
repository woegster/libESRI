#include "stdafx.h"
#include "../include/libESRI.h"
#include "EsriInstance.h"

namespace libESRI
{
  IEsriInstance* CreateLibEsriInstance()
  {
    return new EsriInstance();
  }

  void DeleteLibEsriInstance(IEsriInstance* instance)
  {
    delete instance;
  }
}