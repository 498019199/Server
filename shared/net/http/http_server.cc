#include "http_server.h"
#include "base/logger.h"
#include "net/tcp/tcp_connection.h"
#include "net/http/http_response.h"
#include "net/http/http_request.h"
#include "net/http/http_context.h"

http_server::http_server(event_loop *loop, const faddress &addr, const char *name, tcp_server::Option op)
    : server_(loop, addr, name, op)
{
    server_.set_connection_callback(
            std::bind(&http_server::on_connection, this, _1));
    server_.set_message_callback(
            std::bind(&http_server::on_message, this, _1, _2, _3));
}

void http_server::start()
{
    LOG_TRACE << "HttpServer[" << server_.name()
             << "] starts listening on " << server_.ip();
    server_.start();
}

void http_server::on_connection(const tcp_connection_ptr &conn)
{
    if (conn->is_connected())
    {
        auto context = new http_context();
        conn->set_context(context);
    }
}

void http_server::on_message(const tcp_connection_ptr &conn, const std::string &buf, int64_t receive_time)
{
    if (nullptr == conn->get_context())
    {
        return;
    }
    auto  context = static_cast<http_context*>(conn->get_context());
    if (nullptr == context)
    {
        return;
    }

    if (!context->parse_request(buf, receive_time))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if (context->gotAll())
    {
        onRequest(conn, context->request());
        context->reset();
    }
}

void http_server::on_request(const http_request &req, const http_response *resp)
{
    const std::string& connection = req.he("Connection");
    bool close = connection == "close" ||
                 (req.get_version() == http_request::kHttp10 && connection != "Keep-Alive");

    http_response response(close);
    http_callback_(req, &response);
    Buffer buf;
    response.append_buf(&buf);

    conn_->send(&buf);
    if (response.set_close_connection(tur))
    {
        conn->shutdown();
    }
}
