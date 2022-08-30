cmake_minimum_required(VERSION 3.2)
project(game_rpc)

# 设置c++库版本
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
message(STATUS "-std=c++17 -pthread")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=utf-8")
set(CODE_PATH ${PROJECT_SOURCE_DIR})
set(RES_PATH ${PROJECT_SOURCE_DIR}/res)

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

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")
    add_definitions(-DMODE_DEBUG)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("Release mode:${CMAKE_C_FLAGS_RELEASE}")
else()
    message("else:${CMAKE_BUILD_TYPE}")
    message("else:${CMAKE_C_FLAGS_RELEASE}")
endif()


#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${RES_PATH})
if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(LIBRARY_OUTPUT_PATH ${RES_PATH}/release64/lib)
    set(LIBRARY_OUTPUT_PATH ${RES_PATH}/release64)
else()
    set(LIBRARY_OUTPUT_PATH ${RES_PATH}/debug64/lib)
    set(EXECUTABLE_OUTPUT_PATH ${RES_PATH}/debug64)
endif()
message(STATUS ${LIBRARY_OUTPUT_PATH})
message(STATUS ${EXECUTABLE_OUTPUT_PATH})

# 代码子目录
add_subdirectory(http)
#add_subdirectory(net)
#add_subdirectory(server_rpc)
