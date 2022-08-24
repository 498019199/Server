#include "poller.h"
#include "channel.h"
#include "base/logger.h"

#include <assert.h>
#include <cstring>

#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>

// On Linux, the constants of poll(2) and epoll(4)
// are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

poller::poller(event_loop* loop)
    :loop_(loop),
    events_(kInitEventListSize),
    epoll_fd_(::epoll_create1(EPOLL_CLOEXEC))
{
    if (epoll_fd_ < 0)
    {
        LOG_FATAL << "EPollPoller::EPollPoller";
    }
}

poller::~poller()
{
    ::close(epoll_fd_);
}

void poller::update_channel(channel* chan)
{
    const int index = chan->index();
    LOG_TRACE << "fd = " << chan->fd()
    << " events = " << chan->events() << " index = " << index;
    if (index == kNew || index == kDeleted)
    {
        // a new one, add with EPOLL_CTL_ADD
        int fd = chan->fd();
        if (index == kNew)
        {
            assert(chans_.find(fd) == chans_.end());
            chans_[fd] = chan;
        }
        else // index == kDeleted
        {
            assert(chans_.find(fd) != chans_.end());
            assert(chans_[fd] == chan);
        }

        chan->set_index(kAdded);
        update(EPOLL_CTL_ADD, chan);
    }
    else
    {
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = chan->fd();
        (void)fd;
        assert(chans_.find(fd) != chans_.end());
        assert(chans_[fd] == chan);
        assert(index == kAdded);
        if (chan->is_no_event())
        {
            update(EPOLL_CTL_DEL, chan);
            chan->set_index(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, chan);
        }
    }
}

void poller::remove_channel(channel* chan)
{
    int fd = chan->fd();
    //LOG_TRACE << "fd = " << fd;
    assert(chans_.find(fd) != chans_.end());
    assert(chans_[fd] == chan);
    assert(chan->is_no_event());
    int index = chan->index();
    assert(index == kAdded || index == kDeleted);
    size_t n = chans_.erase(fd);
    (void)n;
    assert(n == 1);

    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, chan);
    }
    chan->set_index(kNew);
}

int poller::poll(int timeout, std::vector<channel*>* active_chans)
{
    LOG_TRACE << "fd total count " << chans_.size();
    int event_num = ::epoll_wait(epoll_fd_,
                                &*events_.begin(),
                                static_cast<int>(events_.size()),
                                timeout);
    int saved_errno = errno;
    if (event_num > 0)
    {
        LOG_TRACE << event_num << " events happened";
        fill_active_channels(event_num, active_chans);
        if (static_cast<size_t>(event_num) == events_.size())
        {
            events_.resize(events_.size()*2);
        }
    }
    else if (event_num == 0)
    {
        LOG_TRACE << "nothing happened";
    }
    else
    {
        // error happens, log uncommon ones
        if (saved_errno != EINTR)
        {
            errno = saved_errno;
            LOG_ERROR << "poller::poll()";
        }
    }

    return 0;
}

void poller::update(int operation, channel* chan)
{
    struct epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = chan->events();
    event.data.ptr = chan;
    int fd = chan->fd();
    LOG_TRACE << "epoll_ctl op = " << operation_to_string(operation)
        << " fd = " << fd << " event = { " << chan->events_to_string() << " }";
    if (::epoll_ctl(epoll_fd_, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR << "epoll_ctl op =" << operation_to_string(operation) << " fd =" << fd;
        }
        else
        {
            LOG_FATAL << "epoll_ctl op =" << operation_to_string(operation) << " fd =" << fd;
        }
    }
}

void poller::fill_active_channels(int num_event, std::vector<channel*>* active_chans) const
{
    assert(static_cast<size_t>(num_event) <= events_.size());
    for (int i = 0; i < num_event; ++i)
    {
        channel* chan = static_cast<channel*>(events_[i].data.ptr);
        int fd = chan->fd();
        auto it = chans_.find(fd);
        assert(it != chans_.end());
        assert(it->second == chan);

        chan->set_revents(events_[i].events);
        active_chans->push_back(chan);
    }
}

const char* poller::operation_to_string(int op)
{
    switch (op)
    {
    case EPOLL_CTL_ADD:
        return "ADD";
    case EPOLL_CTL_DEL:
        return "DEL";
    case EPOLL_CTL_MOD:
        return "MOD";
    default:
        assert(false && "ERROR op");
        return "Unknown Operation";
    }
}