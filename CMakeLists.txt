cmake_minimum_required(VERSION 3.2)
project(demo_rpc)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "current platform: Linux")
    add_definitions(-DPLATFORM_LINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "current platform: Windows")
    add_definitions(-DPLATFORM_WINDOWS)  
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    message(STATUS "current platform: Mac-OS")
    add_definitions(-DPLATFORM_MACOS) 
else()
    message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

include_directories(
    ${PROJECT_SOURCE_DIR}/shared/)

# 共享库
set(HEADERS
    ${PROJECT_SOURCE_DIR}/shared/base/*.h
    ${PROJECT_SOURCE_DIR}/shared/net/*.h
  )

set(NET_SOURCES
    ${PROJECT_SOURCE_DIR}/shared/base/logger.cc
    ${PROJECT_SOURCE_DIR}/shared/net/acceptor.cc
    ${PROJECT_SOURCE_DIR}/shared/net/address.cc
    ${PROJECT_SOURCE_DIR}/shared/net/channel.cc
    ${PROJECT_SOURCE_DIR}/shared/net/connector.cc
    ${PROJECT_SOURCE_DIR}/shared/net/event_loop.cc 
    ${PROJECT_SOURCE_DIR}/shared/net/echo_server.cc
    ${PROJECT_SOURCE_DIR}/shared/net/poller.cc
    ${PROJECT_SOURCE_DIR}/shared/net/socket.cc
    ${PROJECT_SOURCE_DIR}/shared/net/tcp_connection.cc
    ${PROJECT_SOURCE_DIR}/shared/net/tcp_server.cc)
add_library(SHARED_LIB ${NET_SOURCES})


add_executable(${PROJECT_NAME}
    main.cc 
)
target_link_libraries(${PROJECT_NAME} SHARED_LIB)