#include <iostream>
#include "base/logger.h"
#include "shared/net/address.h"
#include "shared/net/event_loop.h"
#include "shared/net/echo_server.h"

logger::ptr g_logger; 
int main()
{
    g_logger = make_shared<logger>();
    g_logger->init();
    
    LOG_TRACE << "begin init server" << std::endl;
    event_loop loop;
    faddress server_addr("127.0.0.1", 6379);
    echo_server server(&loop, server_addr);

    LOG_TRACE << "begin start server" << std::endl;
    server.start();
    LOG_TRACE << "begin loop server" << std::endl;
    loop.loop();
    return 0;
}