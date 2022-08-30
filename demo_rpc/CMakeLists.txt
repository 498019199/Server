#find_package(Protobuf REQUIRED)
#find_package(Threads REQUIRED)

#find_library(PROTOC_LIB NAMES protoc)
#if(NOT PROTOC_LIB)
#    message(FATAL_ERROR "Fail to find protoc lib")
#endif()

# 设置c++库版本
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
message(STATUS "-std=c++17 -pthread")

include_directories(
        ${PROTOBUF_INCLUDE_DIRS}/
        ${PROJECT_SOURCE_DIR}/)

#file(GLOB LIB_HEADERS
#        ${PROJECT_SOURCE_DIR}/base/*.h
#        ${PROJECT_SOURCE_DIR}/net/*.h)
set(LIB_HEADERS
        net/address.h)
set(LIB_SOURCES
        ${PROJECT_SOURCE_DIR}/base/logger.cc
        ${PROJECT_SOURCE_DIR}/base/condition_variable.cc
        ${PROJECT_SOURCE_DIR}/net/acceptor.cc
        ${PROJECT_SOURCE_DIR}/net/address.cc
        ${PROJECT_SOURCE_DIR}/net/channel.cc
        ${PROJECT_SOURCE_DIR}/net/event_loop.cc
        ${PROJECT_SOURCE_DIR}/net/echo_server.cc
        ${PROJECT_SOURCE_DIR}/net/poller.cc
        ${PROJECT_SOURCE_DIR}/net/socket.cc
        ${PROJECT_SOURCE_DIR}/net/tcp/tcp_connection.cc
        ${PROJECT_SOURCE_DIR}/net/tcp/tcp_server.cc
        ${PROJECT_SOURCE_DIR}/net/timer.cc
        ${PROJECT_SOURCE_DIR}/net/http/http_server.cc
        ${PROJECT_SOURCE_DIR}/net/http/http_context.cc
        ${PROJECT_SOURCE_DIR}/net/http/http_request.cc
        ${PROJECT_SOURCE_DIR}/net/http/http_response.cc
        ${PROJECT_SOURCE_DIR}/string/Buffer.cc)

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})

#添加通用库
target_link_libraries(${PROJECT_NAME} pthread stdc++fs)
#target_link_libraries(${PROJECT_NAME} pthread stdc++fs protobuf protoc)

install(FILES LIB_HEADERS DESTINATION
        ${CODE_PATH}/include/net)
install(CODE "MESSAGE(\"Sample install message.\")")
