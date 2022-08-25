#ifndef __NET_ADDRESS__H__
#define __NET_ADDRESS__H__

#include "base/copyable.h"
#include <netinet/in.h>
#include <bits/sockaddr.h>

#include <cstdint>
#include <string>

class faddress : copyable
{
public:
    explicit faddress(const struct sockaddr_in& addr)
        :addr_(addr)
    {
    }

    explicit faddress(const struct sockaddr_in6& addr)
        :addr6_(addr)
    {
    }

    // 使用ip和port 创建地址
    // ip 是"1.2.3.4""
    faddress(const char* ip, uint16_t port, bool ipv6 = false);

    //使用给定的端口号构造端点。
    //主要用于TcpServer侦听。
    faddress(uint16_t port = 0, bool loop_back_only = false, bool ipv6 = false);

    void set_sockaddr_inet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }
    const struct sockaddr* get_sockaddr() const { return &sa_; }
    sa_family_t family() const { return addr_.sin_family; }
    std::string ip() const;
    uint16_t port() const;
private:
    union 
    {
        struct sockaddr sa_{};
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };
    
};

#endif//__NET_ADDRESS__H__