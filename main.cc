#include <iostream>
#include <filesystem>
#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

//
//#include "shared/net/address.h"
//#include "shared/net/event_loop.h"
//#include "shared/net/echo_server.h"
#include "shared/base/logger.h"

logger::ptr g_logger;
int main()
{
    auto path = fs::current_path();
    g_logger = std::make_shared<logger>();
    g_logger->init("111.log", path.c_str(), 1024, 1);

    LOG_TRACE << "begin init server" << std::endl;
//    event_loop loop;
//    faddress server_addr("127.0.0.1", 6379);
//    echo_server server(&loop, server_addr);
//
    LOG_TRACE << "begin start server" << std::endl;
//    server.start();
    LOG_TRACE << "begin loop server" << std::endl;
//    loop.loop();

    return 0;
}

