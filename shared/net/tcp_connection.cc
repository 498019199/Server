#include "tcp_connection.h"
tcp_connection::tcp_connection(/* args */)
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
        close_callback_();
    }
    else
    {
        errno = save_error;
        //LOG_SYSERR << "TcpConnection::handleRead";
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
    // LOG_ERROR << "TcpConnection::handleError [" << name_
    //         << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}