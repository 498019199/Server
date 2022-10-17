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

#ÃÌº”Õ®”√ø‚
target_link_libraries(${PROJECT_NAME} pthread stdc++fs)
#target_link_libraries(${PROJECT_NAME} pthread stdc++fs protobuf protoc)

install(FILES LIB_HEADERS DESTINATION
        ${CODE_PATH}/include/net)
install(CODE "MESSAGE(\"Sample install message.\")")
