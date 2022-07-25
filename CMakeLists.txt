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
    ${PROJECT_SOURCE_DIR}/shared/base/*.h,
    ${PROJECT_SOURCE_DIR}/shared/net/*.h,
)

# 共享库
set(NET_SOURCES
    ${PROJECT_SOURCE_DIR}/shared/net/socket.cc
    ${PROJECT_SOURCE_DIR}/shared/net/connect.cc)
add_library(SHARED_LIB ${NET_SOURCES})

message(STATUS "------------------------------")
message("${PROJECT_SOURCE_DIR}/shared/net/socket.cc")

add_executable(${PROJECT_NAME}
    main.cc 
)
target_link_libraries(${PROJECT_NAME} SHARED_LIB)