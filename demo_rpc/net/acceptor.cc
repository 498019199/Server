#include "acceptor.h"
#include "../base/type.h"
#include "socket.h"
#include "address.h"
#include "../base/logger.h"

acceptor::acceptor(event_loop* loop, const faddress& addr, bool reuseport)
    :accept_fd_(sockets::create_nonblocking_die(addr.family())),
    listening_(false),
    acceptor_chan_(loop, accept_fd_)
{
    sockets::set_reuse_addr(accept_fd_, true);
    sockets::set_reuse_port(accept_fd_, reuseport);
    sockets::bind(accept_fd_, addr.get_sockaddr());
    acceptor_chan_.set_read_callback(std::bind(&acceptor::handle_read, this));
}

void acceptor::set_listening()
{
    listening_ = true;
    sockets::listen(accept_fd_);
    acceptor_chan_.enable_reeading();
}

void acceptor::handle_read()
{
    faddress local_addr;
    struct sockaddr_in6 addr {};
    memZero(&addr, sizeof addr);
    int conn_fd =sockets::accept(accept_fd_, &addr);
    if (conn_fd >= 0)
    {
        local_addr.set_sockaddr_inet6(addr);
        if (new_connction_func_)
        {
            new_connction_func_(conn_fd, local_addr);
        }
        else
        {
            sockets::close(accept_fd_);
        }
    }
    else
    {
        LOG_ERROR << "in acceptor::handle_read";
    }
}