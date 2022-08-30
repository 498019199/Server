#ifndef DEMO_RPC_HTTP_REQUEST_H
#define DEMO_RPC_HTTP_REQUEST_H


#include <map>
#include "../../string/Buffer.h"

class http_response
{
public:
    enum HttpStatusCode
    {
        kUnknown,
        k200Ok = 200,
        k301MovedPermanently = 301,
        k400BadRequest = 400,
        k404NotFound = 404,
    };

    explicit http_response(bool is_closed);

    void set_content_type(const char* content_type) { add_header("Content-Type", content_type);}
    void set_status_code(HttpStatusCode status) { http_state_ = status; }
    void set_status_message(std::string message) { status_message_ = std::move(message); }
    void set_close_connection(bool on) { close_connection_ = on; }
    void set_body(const std::string& body) { http_body_ = body; }

    bool get_close_connection() const { return close_connection_;}
    void add_header(const std::string& key, const std::string& value) { http_header_[key] = value;}
    void append_buf(Buffer* buf);
private:
    std::map<std::string, std::string> http_header_;
    HttpStatusCode http_state_;

    std::string status_message_;
    bool close_connection_ = false;
    std::string http_body_;
};


#endif //DEMO_RPC_HTTP_REQUEST_H
