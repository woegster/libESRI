#include "stdafx.h"
#include "../include/libESRI.h"
#include "EsriInstance.h"

namespace libESRI
{
  IEsriInstance* CreateLibEsriInstance()
  {
    WSADATA wDat = { 0 };
    WSAStartup(MAKEWORD(2, 2), &wDat);
    return new EsriInstance();
  }

  void DeleteLibEsriInstance(IEsriInstance* instance)
  {
    WSACleanup();
    delete instance;
  }
}