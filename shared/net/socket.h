#ifndef __NET_SOCKET__H__
#define __NET_SOCKET__H__
#include "base/noncopyable.h"

#include <arpa/inet.h>

#include <memory>
#include <string>
#include <functional>

class event_loop;
class channel;
class faddress;
class tcp_connection;


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

typedef std::shared_ptr<tcp_connection> tcp_connection_ptr;
typedef std::function<void (const tcp_connection_ptr&)> connnection_callback;
typedef std::function<void (const tcp_connection_ptr&)> close_callback;
typedef std::function<void (const tcp_connection_ptr&)> write_complete_callback;
typedef std::function<void (const tcp_connection_ptr&, std::string, int ts)> message_callback;

typedef std::function<void()> event_callback;
typedef std::function<void(int)> read_event_callback;
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

    struct sockaddr_in6 get_addr(int sockfd);
    int get_socket_error(int sockfd);


    const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
    struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
    const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

};


#endif//__NET_SOCKET__H__