#include "tcp_connection.h"
#include "channel.h"
#include "base/logger.h"

#include<memory>
#include <string.h>

#include <error.h>

tcp_connection::tcp_connection(event_loop* loop,
                    const std::string name,
                    int sockfd,
                    const faddress& local_addr,
                    const faddress& peer_addr)
                    :loop_(loop),
                    name_(name),
                    state_(kConnected),
                    reading_(true),
                    sock_fd_(sockfd),
                    chan_(new channel(loop, sockfd)),
                    local_addr_(local_addr),
                    peer_addr_(peer_addr)
{
    
}


void tcp_connection::set_state(Connection_State state)
{
    state_ = state;
}

tcp_connection::~tcp_connection()
{
}

void tcp_connection::handle_read(int ts)
{
    int save_error = 0;
    char extrabuf[65536];
    int n = sockets::read(chan_->fd(), extrabuf, 65536);
    if (n > 0)
    {
        message_callback_(shared_from_this(), extrabuf, ts);
    }
    else if (0 == n)
    {
        handle_close();
    }
    else
    {
        errno = save_error;
        LOG_SYSERR << "TcpConnection::handleRead";
        handle_error();
    }
}

void tcp_connection::handle_write()
{}

void tcp_connection::handle_close()
{}

void tcp_connection::handle_error()
{
    int err = sockets::get_socket_error(chan_->fd());
    LOG_ERROR << "TcpConnection::handleError [" << name_
             << "] - SO_ERROR = " << err << " " << strerror(err);
}
