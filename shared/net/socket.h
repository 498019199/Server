#ifndef __NET_SOCKET__H__
#define __NET_SOCKET__H__
#include <arpa/inet.h>

namespace sockets
{
    // 创建非阻塞套接字文件描述符
    int create_nonblocking_die(sa_family_t family);
    int  connect(int sockfd, const struct sockaddr* addr);
    void bind(int sockfd, const struct sockaddr* addr);
    void listen(int sockfd);
    int  accept(int sockfd, struct sockaddr_in6* addr);
    ssize_t read(int sockfd, void *buf, size_t count);
    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
    ssize_t write(int sockfd, const void *buf, size_t count);
    void close(int sockfd);

    void set_tcp_delay(int sockfd, bool on);
    void set_reuse_addr(int sockfd, bool on);
    void set_reuse_port(int sockfd, bool on);
    void set_keep_alive(int sockfd, bool on);
};


#endif//__NET_SOCKET__H__