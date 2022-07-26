#ifndef DEMO_RPC_HTTP_SERVER_H
#define DEMO_RPC_HTTP_SERVER_H

#include <utility>

#include "../../../network/include/socket.h"
#include "../tcp/tcp_server.h"
#include "../../string/Buffer.h"

class http_request;
class http_response;

class http_server
{
public:
    typedef std::function<void(const http_request&, http_response*)> http_callback;

    http_server(event_loop* loop, const faddress& addr, const char* name, tcp_server::Option op = tcp_server::kNoReusePort);

    event_loop* get_loop() { return server_.get_loop();}

    void set_http_callback(http_callback callback) { http_callback_ = std::move(callback);}

    void start();

private:
    void on_connection(const tcp_connection_ptr& conn);

    void on_message(const tcp_connection_ptr& com, Buffer* buf, int64_t receive_time);

    void on_request(const tcp_connection_ptr& conn, const http_request& req);

private:
    tcp_server server_;
    http_callback http_callback_;
};


#endif //DEMO_RPC_HTTP_SERVER_H
