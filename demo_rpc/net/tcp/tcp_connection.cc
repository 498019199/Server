#include "tcp_connection.h"
#include "../channel.h"
#include "../../base/logger.h"

#include<memory>
#include <cstring>

tcp_connection::tcp_connection(event_loop* loop,
                    const char* name,
                    int sockfd,
                    const faddress& local_addr,
                    const faddress& peer_addr)
                    :loop_(loop),
                    name_(name),
                    reading_(true),
                    state_(kConnecting),
                    sock_fd_(sockfd),
                    local_addr_(local_addr),
                    peer_addr_(peer_addr),
                    chan_(new channel(loop, sockfd))
{
    chan_->set_read_callback(
            std::bind(&tcp_connection::handle_read, this, _1));
    chan_->set_write_callback(
            std::bind(&tcp_connection::handle_write, this));
    chan_->set_close_callback(
            std::bind(&tcp_connection::handle_close, this));
    chan_->set_error_callback(
            std::bind(&tcp_connection::handle_error, this));
    LOG_DEBUG << "tcp_connection::ctor[" <<  name_ << "] at " << this
              << " fd=" << sockfd;
    sockets::set_keep_alive(sockfd, true);
}


void tcp_connection::set_state(Connection_State state)
{
    state_ = state;
}

tcp_connection::~tcp_connection()
{
    LOG_DEBUG << "TcpConnection::dtor[" <<  name_ << "] at " << this
              << " fd=" << chan_->fd()
              << " state=" << state_to_string();
    assert(state_ == kDisconnected);
}

void tcp_connection::handle_read(int ts)
{
    int save_error = 0;
    ssize_t n = input_buff_.readFd(chan_->fd(), &save_error);
    if (n > 0)
    {
        message_callback_(shared_from_this(), &input_buff_, ts);
    }
    else if (0 == n)
    {
        handle_close();
    }
    else
    {
        errno = save_error;
        LOG_ERROR << "TcpConnection::handleRead";
        handle_error();
    }
}

void tcp_connection::handle_write()
{
    if (chan_->is_writing())
    {
        ssize_t n = sockets::write(chan_->fd(),
                                   output_buff_.peek(),
                                   output_buff_.readableBytes());
        if (n > 0)
        {
            output_buff_.retrieve(n);
            if (output_buff_.readableBytes() == 0)
            {
                chan_->disable_writing();
                if (write_complete_callback_)
                {
                    write_complete_callback_(shared_from_this());
                }
                if (state_ == kDisconnecting)
                {
                    sockets::shutdown_write(sock_fd_);
                }
            }
        }
        else
        {
            LOG_TRACE << "tcp_connection::handle_write";
        }
    }
    else
    {
        LOG_TRACE << "Connection fd = " << chan_->fd()
                  << " is down, no more writing";
    }
}

void tcp_connection::handle_close()
{
    LOG_TRACE << "fd = " << chan_->fd() << " state = " << state_to_string();
    assert(state_ == kConnected || state_ == kDisconnecting);
    // we don't close fd, leave it to dtor, so we can find leaks easily.
    set_state(kDisconnected);
    chan_->disable_all();

    tcp_connection_ptr guardThis(shared_from_this());
    connnection_callback_(guardThis);
    // must be the last line
    close_callback_(guardThis);
}

void tcp_connection::handle_error()
{
    int err = sockets::get_socket_error(chan_->fd());
    LOG_ERROR << "TcpConnection::handleError [" << name_
             << "] - SO_ERROR = " << err << " " << strerror(err);
}

void tcp_connection::send(const StringPiece &msg)
{
    if (state_ != kConnected)
    {
        return;
    }
    send(msg.data(), msg.size());
}

void tcp_connection::send(const char *msg, int len)
{
    if (state_ != kConnected)
    {
        return;
    }
    send_in_loop(msg, len);
}

void tcp_connection::send(Buffer *msg)
{
    if (state_ != kConnected)
    {
        return;
    }
    send(msg->peek(), msg->readableBytes());
    msg->retrieveAll();
}

void tcp_connection::shutdown()
{
    if (state_ == kConnected)
    {
        set_state(kDisconnecting);
    }
}

void tcp_connection::send_in_loop(const char *msg, int len)
{
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if (state_ == kDisconnected)
    {
        LOG_WARN << "disconnected, give up writing";
        return;
    }

    // if no thing in output queue, try writing directly
    if (!chan_->is_writing() && output_buff_.readableBytes() == 0)
    {
        nwrote = sockets::write(chan_->fd(), msg, len);
        if (nwrote >= 0)
        {
            remaining = len - nwrote;
            if (remaining == 0 && write_complete_callback_)
            {
                loop_->queue_in_loop(std::bind(write_complete_callback_, shared_from_this()));
            }
        }
        else // nwrote < 0
        {
            nwrote = 0;
            if (errno != EWOULDBLOCK)
            {
                LOG_TRACE << "TcpConnection::sendInLoop";
                if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
                {
                    faultError = true;
                }
            }
        }
    }

//    assert(remaining <= len);
//    if (!faultError && remaining > 0)
//    {
//        size_t oldLen = output_buff_.readableBytes();
//        if (oldLen + remaining >= highWaterMark_
//            && oldLen < highWaterMark_
//            && highWaterMarkCallback_)
//        {
//            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
//        }
//        output_buff_.append(static_cast<const char*>(msg)+nwrote, remaining);
//        if (!chan_->is_writing())
//        {
//            chan_->enable_writing();
//        }
//    }
}

const char *tcp_connection::state_to_string() const
{
    switch (state_)
    {
        case kDisconnected:
            return "kDisconnected";
        case kConnecting:
            return "kConnecting";
        case kConnected:
            return "kConnected";
        case kDisconnecting:
            return "kDisconnecting";
        default:
            return "unknown state";
    }

    return "";
}

void tcp_connection::connect_established()
{
    assert(state_ == kConnecting);
    set_state(kConnected);
    chan_->tie(shared_from_this());
    chan_->enable_reeading();
    connnection_callback_(shared_from_this());
}

void tcp_connection::connect_destroyed()
{
    if (state_ == kConnected)
    {
        set_state(kDisconnected);
        chan_->disable_all();
        connnection_callback_(shared_from_this());
    }
    chan_->remove();
}
