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
    LOG_TRACE << "http_server[" << server_.name()
             << "] starts listening on " << server_.ip();
    server_.start();
}

void http_server::on_connection(const tcp_connection_ptr &conn)
{
    if (conn->is_connected())
    {
        auto context = std::make_shared<http_context>();
        conn->set_context(context);
    }
}

void http_server::on_message(const tcp_connection_ptr &conn, Buffer* buf, int64_t receive_time)
{
    std::shared_ptr<http_context>  context_ptr = conn->get_context().lock();
    if (!context_ptr->parse_request(buf, receive_time))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if (http_context::HttpRequestParseState::kGotAll == context_ptr->get_state())
    {
        on_request(conn, context_ptr->get_http_request());
        context_ptr->reset();
    }
}

void http_server::on_request(const tcp_connection_ptr &conn, const http_request &req)
{
    const std::string& connection = req.get_header("Connection");
    bool close = connection == "close" ||
                 (req.get_version() == http_request::kHttp10 && connection != "Keep-Alive");

    http_response response(close);
    http_callback_(req, &response);
    Buffer buf;
    response.append_buf(&buf);

    conn->send(&buf);
    if (response.get_close_connection())
    {
        conn->shutdown();
    }
}
