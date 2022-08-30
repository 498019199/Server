#ifndef DEMO_RPC_HTTP_CONTEXT_H
#define DEMO_RPC_HTTP_CONTEXT_H

#include "http_request.h"
#include "../../string/Buffer.h"

class http_context
{
public:
    enum HttpRequestParseState
    {
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll,
    };

    http_context()
        :state_(kExpectRequestLine)
    {}

    void reset();

    bool parse_request(Buffer* buf, int64_t received_time);

    void set_state(HttpRequestParseState state) { state_ = state; }
    HttpRequestParseState get_state() { return state_; }

    http_request get_http_request() const { return http_request_;}
    http_request get_http_request()  { return http_request_;}
private:
    bool process_request_line(const char* start, const char* end);

    HttpRequestParseState state_;
    http_request http_request_;
};


#endif //DEMO_RPC_HTTP_CONTEXT_H
