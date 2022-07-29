#include "channel.h"
#include "event_loop.h"
#include <poll.h>

const int channel::kNoneEvent = 0;
const int channel::kReadEvent = POLLIN | POLLPRI;
const int channel::kWriteEvent = POLLOUT;

channel::channel(event_loop* loop, int fd)
    :loop_(loop),
    fd_(fd)
{}

channel::~channel()
{
    
}

void channel::handle_event(int receive_time)
{
    event_handling_ = true;
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
    {
        // if (logHup_)
        // {
        //     LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
        // }
        if (close_callback_) close_callback_();
    }

    if (revents_ & POLLNVAL)
    {
        //LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
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

void channel::remove()
{   
    if (loop_)
    {
        loop_->remove_channel(this);
    }
}

void channel::update()
{
    if (loop_)
    {
        loop_->update_channel(this);
    }
}