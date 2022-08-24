#include <iostream>

#include "filessystem.h"
#include "shared/net/address.h"
#include "shared/net/event_loop.h"
#include "shared/net/echo_server.h"
#include "shared/base/logger.h"

logger::ptr g_logger;
int main()
{
    auto path = fs::current_path();
    g_logger = std::make_shared<logger>();
    g_logger->init("111.log", path.c_str(), 1024, 1);

    event_loop loop;
    g_logger->start(&loop, 1);
    LOG_TRACE << "begin init server";

    faddress server_addr("127.0.0.1", 6379);
    echo_server server(&loop, server_addr);

    LOG_TRACE << "begin start server";
    server.start();
    LOG_TRACE << "begin loop server";
    loop.loop();
    return 0;
}

