#include <algorithm>
#include "http_context.h"

void http_context::reset()
{
    state_ = kExpectRequestLine;
    http_request dummy;
    http_request_ = dummy;
}

bool http_context::parse_request(Buffer* buf, int64_t received_time)
{
    bool ok = true;
    bool hasMore = true;
    while (hasMore)
    {
        if (state_ == kExpectRequestLine)
        {
            const char* crlf = buf->findCRLF();
            if (crlf)
            {
                ok = process_request_line(buf->peek(), crlf);
                if (ok)
                {
                    http_request_.set_receive_time(received_time);
                    buf->retrieveUntil(crlf + 2);
                    state_ = kExpectHeaders;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state_ == kExpectHeaders)
        {
            const char* crlf = buf->findCRLF();
            if (crlf)
            {
                const char* colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf)
                {
                    http_request_.add_header(buf->peek(), colon, crlf);
                }
                else
                {
                    // empty line, end of header
                    // FIXME:
                    state_ = kGotAll;
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state_ == kExpectBody)
        {
            // FIXME:
        }
    }
    return ok;
}

bool http_context::process_request_line(const char *begin, const char *end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    if (space != end && http_request_.set_method(start, space))
    {
        start = space+1;
        space = std::find(start, end, ' ');
        if (space != end)
        {
            const char* question = std::find(start, space, '?');
            if (question != space)
            {
                http_request_.set_path(start, question);
                http_request_.set_query(question, space);
            }
            else
            {
                http_request_.set_path(start, space);
            }
            start = space+1;
            succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
            if (succeed)
            {
                if (*(end-1) == '1')
                {
                    http_request_.set_version(http_request::kHttp11);
                }
                else if (*(end-1) == '0')
                {
                    http_request_.set_version(http_request::kHttp10);
                }
                else
                {
                    succeed = false;
                }
            }
        }
    }

    return succeed;
}

