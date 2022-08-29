#ifndef DEMO_RPC_HTTP_RESPONSE_H
#define DEMO_RPC_HTTP_RESPONSE_H


#include <map>
#include <utility>
#include "string/Buffer.h"

class http_request
{
public:
    enum Method
    {
        kInvalid,
        kGet,
        kPost,
        kHead,
        kPut,
        kDelete,
    };

    enum Version
    {
        kUnknown,
        kHttp10,
        kHttp11
    };

    http_request();

    void set_version(Version version) { version_ = version; }
    Version get_version() const { return version_; }

    bool set_method(const char* start, const char* end);
    Method get_method() const { return method_; }
    const char* method_to_string() const;

    void set_path(const char* start, const char* end) { path_.assign(start, end); }
    const std::string& get_path() const { return path_;}

    void set_query(const char* start, const char* end) { query_.assign(start, end);}
    const std::string& get_query() const { return query_;}

    void set_receive_time(int64_t rt) { received_time = rt;}
    int64_t get_recei_time() const { return received_time;}

    void add_header(const char* start, const char* colon, const char* end);
    std::string get_header(const char* key) const;
    const std::map<std::string, std::string>& get_hearder_map() const { return http_header_;}
private:
    Method method_;
    Version version_;
    std::string path_;
    std::string query_;
    int64_t received_time = 0;
    std::map<std::string, std::string> http_header_;
};


#endif //DEMO_RPC_HTTP_RESPONSE_H
