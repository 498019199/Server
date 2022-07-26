#include "socket.h"
#include "../base/type.h"
#include "../base/logger.h"

#include <unistd.h>
#include <sys/uio.h>  // readv
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace sockets
{


int create_nonblocking_die(sa_family_t family)
{
    //SOCK_STREAM提供面向连接的稳定数据传输
    int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL << "sockets::createNonblockingOrDie";
    }
    return sockfd;
}

int connect(int sockfd, const struct sockaddr* addr)
{
    return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

void bind(int sockfd, const struct sockaddr* addr)
{
    int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if (ret < 0)
    {
        LOG_FATAL << "sockets::bindOrDie";
    }
}

void listen(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_FATAL << "sockets::listenOrDie";
    }
}

int accept(int sockfd, struct sockaddr_in6* addr)
{
    auto addrlen = static_cast<socklen_t>(sizeof *addr);
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
    return connfd;
}

ssize_t read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t write(int sockfd, const void *buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void close(int sockfd)
{
    if (::close(sockfd) < 0)
    {
        LOG_ERROR << "sockets::close";
    }
}

 void set_tcp_delay(int sockfd, bool on)
{
    // 减少小包的数量
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY,
                &optval, static_cast<socklen_t>(sizeof optval));
}
    
void set_reuse_addr(int sockfd, bool on)
{
    //打开或关闭地址复用功能
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                &optval, static_cast<socklen_t>(sizeof optval));
}

void set_reuse_port(int sockfd, bool on)
{
    //端口释放后立即就可以被再次使用
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
        LOG_ERROR << "SO_REUSEPORT failed.";
    }
}

void set_keep_alive(int sockfd, bool on)
{
    //套接字保活。
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
}

struct sockaddr_in6 get_addr(int sockfd)
{
    struct sockaddr_in6 addr{};
    memZero(&addr, sizeof addr);
    auto addrlen = static_cast<socklen_t>(sizeof addr);
    if (::getsockname(sockfd, sockaddr_cast(&addr), &addrlen) < 0)
    {
        LOG_ERROR << "sockets::getLocalAddr";
    }

    return addr;
}

int get_socket_error(int sockfd)
{
    int optval;
    auto optlen = static_cast<socklen_t>(sizeof optval);
    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    else
    {
        return optval;
    }
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}

void shutdown_write(int sockfd)
{
    ::shutdown(sockfd, SHUT_WR);
}


}