
#include <cassert>
#include "http_request.h"

http_request::http_request()
        :method_(kInvalid), version_(kUnknown)
{

}

bool http_request::set_method(const char *start, const char *end)
{
    assert(method_ == kInvalid);
    std::string m(start, end);
    if (m == "GET")
    {
        method_ = kGet;
    }
    else if (m == "POST")
    {
        method_ = kPost;
    }
    else if (m == "HEAD")
    {
        method_ = kHead;
    }
    else if (m == "PUT")
    {
        method_ = kPut;
    }
    else if (m == "DELETE")
    {
        method_ = kDelete;
    }
    else
    {
        method_ = kInvalid;
    }
    return (method_ != kInvalid);
}

const char *http_request::method_to_string() const
{
    const char* result = "UNKNOWN";
    switch(method_)
    {
        case kGet:
            result = "GET";
            break;
        case kPost:
            result = "POST";
            break;
        case kHead:
            result = "HEAD";
            break;
        case kPut:
            result = "PUT";
            break;
        case kDelete:
            result = "DELETE";
            break;
        default:
            break;
    }
    return result;
}

void http_request::add_header(const char *start, const char *colon, const char *end)
{
    std::string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
    {
        ++colon;
    }
    std::string value(colon, end);
    while (!value.empty() && isspace(value[value.size()-1]))
    {
        value.resize(value.size()-1);
    }
    http_header_[field] = value;
}

std::string http_request::get_header(const char *key) const
{
    std::string result;
    auto it = http_header_.find(key);
    if (it != http_header_.end())
    {
        result = it->second;
    }
    return result;
}



