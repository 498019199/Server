#include "acceptor.h"

acceptor::acceptor(const faddress& addr, bool reuseport)
    :accept_fd_(sockets::create_nonblocking_die(addr.family())),
    listening_(false)
{
    sockets::bind(accept_fd_, addr.get_sockaddr());
}

acceptor::~acceptor()
{}

void acceptor::set_listening()
{
    listening_ = true;
}