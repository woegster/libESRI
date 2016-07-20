#pragma once
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#include <Windows.h>
#include <WS2tcpip.h>
#else
#define SOCKET int
#define INVALID_SOCKET 0
#include <sys/socket.h>
#include <netinet/in.h>
#endif
