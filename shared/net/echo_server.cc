#include "echo_server.h"
#include "event_loop.h"
#include "tcp_server.h"

echo_server::echo_server(event_loop* loop, const faddress& addr)
    :server_(loop, addr, "echo_server")
{
    
}

echo_server::~echo_server()
{
    
}

void echo_server::start()
{
    server_.start();
}
