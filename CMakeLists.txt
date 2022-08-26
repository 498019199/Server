cmake_minimum_required(VERSION 3.2)
project(demo_rpc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=utf-8")

# 平台宏
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

# 编译模式
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")
    add_definitions(-DMODE_DEBUG)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("Release mode:${CMAKE_C_FLAGS_RELEASE}")
else()
    message("else:${CMAKE_BUILD_TYPE}")
    message("else:${CMAKE_C_FLAGS_RELEASE}")
endif()

# 设置c++库版本
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
message(STATUS "-std=c++17 -pthread")

include_directories(
    ${PROJECT_SOURCE_DIR}/shared/
    ${PROJECT_SOURCE_DIR}/shared/other_lib/
    ${PROJECT_SOURCE_DIR}/shared/base/C++17/)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/shared/base/*.h
    ${PROJECT_SOURCE_DIR}/shared/net/*.h
  )

set(NET_SOURCES
        ${PROJECT_SOURCE_DIR}/shared/base/logger.cc
        ${PROJECT_SOURCE_DIR}/shared/base/condition_variable.cc

        ${PROJECT_SOURCE_DIR}/shared/net/acceptor.cc
        ${PROJECT_SOURCE_DIR}/shared/net/address.cc
        ${PROJECT_SOURCE_DIR}/shared/net/channel.cc
        ${PROJECT_SOURCE_DIR}/shared/net/connector.cc
        ${PROJECT_SOURCE_DIR}/shared/net/event_loop.cc
        ${PROJECT_SOURCE_DIR}/shared/net/echo_server.cc
        ${PROJECT_SOURCE_DIR}/shared/net/poller.cc
        ${PROJECT_SOURCE_DIR}/shared/net/socket.cc
        ${PROJECT_SOURCE_DIR}/shared/net/tcp/tcp_connection.cc
        ${PROJECT_SOURCE_DIR}/shared/net/tcp/tcp_server.cc
        ${PROJECT_SOURCE_DIR}/shared/net/timer.cc
        ${PROJECT_SOURCE_DIR}/shared/net/http/http_server.cc
        ${PROJECT_SOURCE_DIR}/shared/net/http/http_context.cc
        ${PROJECT_SOURCE_DIR}/shared/net/http/http_request.cc
        ${PROJECT_SOURCE_DIR}/shared/net/http/http_response.cc
        shared/string/Buffer.cc)

add_library(SHARED_LIB ${NET_SOURCES})

add_executable(${PROJECT_NAME}
    main.cc)

target_link_libraries(${PROJECT_NAME} SHARED_LIB)
target_link_libraries(${PROJECT_NAME} pthread stdc++fs)