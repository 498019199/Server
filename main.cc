#include <iostream>
#include "shared/net/address.h"
#include "shared/net/event_loop.h"
#include "shared/net/echo_server.h"

int main()
{
    event_loop loop;
    faddress server_addr("127.0.0.1", 6379);
    echo_server server(&loop, server_addr);

    server.start();
    loop.loop();
    return 0;
}