#include "address.h"
#include "shared/base/type.h"

#include <string.h>
#include <endian.h>
#include<arpa/inet.h>

//faddress---------------------------------------------------------------------
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;
#pragma GCC diagnostic error "-Wold-style-cast"

faddress::faddress(const char* ip, uint16_t port, bool ipv6 /*= false*/)
{
    if (ipv6)
    {
        memset(&addr6_, 0, sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        addr6_.sin6_port = htobe16(port);
        if (::inet_pton(AF_INET6, ip, &addr6_.sin6_addr) <= 0)
        {
            //LOG_SYSERR << "sockets::fromIpPort";
        }
    }
    else
    {
        memset(&addr_, 0, sizeof addr_);
        addr_.sin_family = AF_INET;
        addr_.sin_port = htobe16(port);
        if (::inet_pton(AF_INET, ip, &addr_.sin_addr) <= 0)
        {
            //LOG_SYSERR << "sockets::fromIpPort";
        }
    }
}

faddress::faddress(uint16_t port, bool loop_back_only, bool ipv6 /*= false*/)
{
    if (ipv6)
    {
        memset(&addr6_, 0, sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loop_back_only ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = htobe16(port);
    }
    else
    {
        memset(&addr_, 0, sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip = loop_back_only ? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = htobe32(ip);
        addr_.sin_port = htobe16(port);
    }
}

std::string faddress::ip() const
{
    char buf[64] = "";
    int size = sizeof buf;
    if (sa_.sa_family == AF_INET)
    {
        assert(size >= INET_ADDRSTRLEN);
        ::inet_ntop(AF_INET, &addr_.sin_addr, buf, static_cast<socklen_t>(size));
    }
    else if (sa_.sa_family == AF_INET6)
    {
        assert(size >= INET6_ADDRSTRLEN);
        ::inet_ntop(AF_INET6, &addr6_.sin6_addr, buf, static_cast<socklen_t>(size));
    }

  return buf;
}

uint16_t faddress::port() const
{
    return be16toh(addr_.sin_port);
}