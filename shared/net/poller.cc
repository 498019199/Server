#include "poller.h"

#include <assert.h>

#include <poll.h>
#include <sys/epoll.h>

// On Linux, the constants of poll(2) and epoll(4)
// are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

poller::poller(event_loop* loop)
    :
{
}

poller::~poller()
{
    ::close(epoll_fd_);
}

void poller::update_channel(channel* chan)
{
    const int index = chan->index();
    // LOG_TRACE << "fd = " << chan->fd()
    // << " events = " << chan->events() << " index = " << index;
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
    assert(chans_[fd] == channel);
    assert(chan->is_no_event());
    int index = chan->index();
    assert(index == kAdded || index == kDeleted);
    size_t n = chans_.erase(fd);
    (void)n;
    assert(n == 1);

    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    chan->set_index(kNew);
}

void poller::update(int operation, channel* chan)
{
    struct epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = chan->events();
    event.data.ptr = chan;
    int fd = chan->fd();
    //LOG_TRACE << "epoll_ctl op = " << operationToString(operation)
        //<< " fd = " << fd << " event = { " << chan->eventsToString() << " }";
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            //LOG_SYSERR << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
        else
        {
            //LOG_SYSFATAL << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
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