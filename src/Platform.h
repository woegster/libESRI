#pragma once
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#include <Windows.h>
#include <WS2tcpip.h>
using sockaddrLenType = int;
#else
#define closesocket close
using sockaddrLenType = unsigned int;
#define SOCKET int
#define SD_BOTH SHUT_RDWR
#define INVALID_SOCKET -1
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif
