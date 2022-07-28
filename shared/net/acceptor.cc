#include "acceptor.h"
#include "socket.h"
#include "address.h"

acceptor::acceptor(event_loop* eve_loop, const faddress& addr, bool reuseport)
    :accept_fd_(sockets::create_nonblocking_die(addr.family())),
    acceptor_chan_(eve_loop, accept_fd_),
    listening_(false)
{
    sockets::set_reuse_addr(accept_fd_, true);
    sockets::set_reuse_port(accept_fd_, reuseport);
    sockets::bind(accept_fd_, addr.get_sockaddr());
}

acceptor::~acceptor()
{}

void acceptor::set_listening()
{
    listening_ = true;
}

void acceptor::handle_read()
{
    faddress addr;
    int conn_fd =sockets::accept(accept_fd_, &addr);
    if (conn_fd >= 0)
    {
        if (new_connction_func_)
        {
            new_connction_func_();
        }
        else
        {
            sockets::close(accept_fd_);
        }
    }
    else
    {

        
    }
}