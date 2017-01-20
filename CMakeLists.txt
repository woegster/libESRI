cmake_minimum_required (VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
cmake_policy(SET CMP0011 OLD)
include(cotire)
project (libESRI)

SET(CMAKE_CXX_STANDARD 11)

SET(SOURCES 
  src/EsriClientThread.cpp
  src/EsriHandler.cpp
  src/EsriHandlerFactory.cpp
  src/EsriInstance.cpp
  src/EsriInternalCommands.cpp
  src/EsriTerminal.cpp
  src/libESRI.cpp
  src/SetThreadName.cpp
  src/SocketEndpoint.cpp
  src/SocketEndpointConverter.cpp
  src/TcpClient.cpp
  src/TcpServer.cpp
)

if ( WIN32 )
  SET(SOURCES ${SOURCES} src/OsTools_Windows.cpp)
elseif ( UNIX )
  SET(SOURCES ${SOURCES} src/OsTools_Linux.cpp)
endif()

add_definitions(-DBUILDING_ESRI)
add_library(libESRI SHARED ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(libESRI Threads::Threads)

if ( WIN32 )
  target_link_libraries(libESRI psapi.lib ws2_32.lib)
endif()

set_target_properties(libESRI PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/stdafx.h")
set_target_properties(libESRI PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
cotire(libESRI)