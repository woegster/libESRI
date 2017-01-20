cmake_minimum_required (VERSION 3.1.3)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
cmake_policy(SET CMP0011 OLD)
include(cotire)
project (libESRI)

SET(CMAKE_CXX_STANDARD 11)

#3rd party telnet
add_library(Telnet 3rdparty/libtelnet/libtelnet.c)
set_property(TARGET Telnet PROPERTY POSITION_INDEPENDENT_CODE ON)

#3rd party nthshell
SET(SOURCES_NTSHELL
  3rdparty/ntshell/ntlibc.c
  3rdparty/ntshell/ntshell.c
  3rdparty/ntshell/text_editor.c
  3rdparty/ntshell/text_history.c
  3rdparty/ntshell/vtrecv.c
  3rdparty/ntshell/vtsend.c
)

add_library(nthshell ${SOURCES_NTSHELL})
set_property(TARGET nthshell PROPERTY POSITION_INDEPENDENT_CODE ON)

#main library
SET(TARGET ESRI)
if ( WIN32 )
  SET(TARGET libESRI) #windows dlls are not prefixed with dll, add it here for unified output names
endif()

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
include_directories(
  3rdparty/libtelnet
  3rdparty/ntshell)
add_library(${TARGET} SHARED ${SOURCES})
target_link_libraries(${TARGET} Telnet nthshell)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET} Threads::Threads)

if ( WIN32 )
  target_link_libraries(${TARGET} psapi.lib ws2_32.lib)
endif()

set_target_properties(${TARGET} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/stdafx.h")
set_target_properties(${TARGET} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
cotire(${TARGET})

#sample app
include_directories(include)
add_executable(sample sample/SampleApp.cpp)
target_link_libraries(sample ${TARGET})