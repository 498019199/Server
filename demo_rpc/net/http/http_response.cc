#include "http_response.h"

http_response::http_response(bool is_closed)
        :http_state_(kUnknown),close_connection_(is_closed)
{

}

void http_response::append_buf(Buffer *output)
{
    char buf[32];
    snprintf(buf, sizeof buf, "HTTP/1.1 %d ", http_state_);
    output->append(buf);
    output->append(status_message_);
    output->append("\r\n");

    if (close_connection_)
    {
        output->append("Connection: close\r\n");
    }
    else
    {
        snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", http_body_.size());
        output->append(buf);
        output->append("Connection: Keep-Alive\r\n");
    }

    for (const auto& header : http_header_)
    {
        output->append(header.first);
        output->append(": ");
        output->append(header.second);
        output->append("\r\n");
    }

    output->append("\r\n");
    output->append(http_body_);
}


