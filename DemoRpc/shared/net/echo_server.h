#ifndef __NET_ECHO_SERVER__H__
#define __NET_ECHO_SERVER__H__

#include "net/tcp/tcp_server.h"

class event_loop;
class faddress;

class echo_server
{
public:
    echo_server(event_loop* loop, const faddress& addr);

    ~echo_server();

    void start();

private:
    tcp_server server_;
};


#endif//__NET_ECHO_SERVER__H__