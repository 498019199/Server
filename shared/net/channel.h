#ifndef __NET_CHANNEL__H__
#define __NET_CHANNEL__H__
#include "loop_event.h"

class channel
{
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
public:
    channel(event_loop* loop, int fd);

    int fd() const { return fd_; }
    int events() const { return events_; }
    bool is_no_event() const { return events_ == kNoneEvent;}
    // for Poller
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

private:
    event_loop* loop_;
    const int  fd_;
    int        events_;
    int        revents_; // it's the received event types of epoll or poll
    int        index_; // used by Poller.
};
#endif//__NET_CHANNEL__H__