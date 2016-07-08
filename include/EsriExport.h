#pragma once

#ifdef BUILDING_ESRI
#define ESRIEXPORT __declspec(dllexport)
#else
#define ESRIEXPORT __declspec(dllimport)
#endif
