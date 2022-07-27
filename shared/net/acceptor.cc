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
    acceptor_chan_
}

acceptor::~acceptor()
{}

void acceptor::set_listening()
{
    listening_ = true;
}