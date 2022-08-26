#include "channel.h"
#include "event_loop.h"
#include "base/logger.h"
#include "poller.h"

#include <sstream>

#include <poll.h>

const int channel::kNoneEvent = 0;
const int channel::kReadEvent = POLLIN | POLLPRI;
const int channel::kWriteEvent = POLLOUT;

channel::channel(event_loop* loop, int fd)
    :loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(kNew)
{}

void channel::handle_event(int receive_time)
{
    event_handling_ = true;
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
    {
        // if (logHup_)
        // {
             LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
        // }
        if (close_callback_) close_callback_();
    }

    if (revents_ & POLLNVAL)
    {
        LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
    }
    if (revents_ & (POLLERR | POLLNVAL))
    {
        if (error_callback_) error_callback_();
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (read_callback_) read_callback_(receive_time);
    }
    if (revents_ & POLLOUT)
    {
        if (write_callback_) write_callback_();
    }
    event_handling_ = false;
}

std::string channel::revents_to_string() const
{
    return events_to_string(fd_, revents_);
}

std::string channel::events_to_string() const
{
    return events_to_string(fd_, events_);
}

std::string channel::events_to_string(int fd, int ev)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLRDHUP)
        oss << "RDHUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";

    return oss.str(); 
}

void channel::update()
{
    add_to_loop_ = true;
    loop_->update_channel(this);
}

void channel::remove()
{
    add_to_loop_ = false;
    loop_->remove_channel(this);
}

void channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}
