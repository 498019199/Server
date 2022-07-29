#ifndef __NET_CHANNEL__H__
#define __NET_CHANNEL__H__
#include "socket.h"

class channel :public noncopyable
{
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
public:
    channel(event_loop* loop, int fd);
    ~channel();

    void handle_event(int receive_time);
    void set_read_callback(read_event_callback cb){ read_callback_ = std::move(cb); }
    void set_write_callback(event_callback cb){ write_callback_ = std::move(cb); }
    void set_close_callback(event_callback cb){ close_callback_ = std::move(cb); }
    void set_error_callback(event_callback cb){ error_callback_ = std::move(cb); }

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; } // used by pollers
    bool is_no_event() const { return events_ == kNoneEvent;}
    // for Poller
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    void enable_reeading() { events_ |= kReadEvent; update(); }
    void disable_reading() { events_ &= ~kReadEvent; update(); }
    void enable_writing() { events_ |= kWriteEvent; update(); }
    void disable_writing() { events_ &= ~kWriteEvent; update(); }
    void disable_all() { events_ = kNoneEvent; update(); }
    bool is_writing() const { return events_ & kWriteEvent; }
    bool is_reading() const { return events_ & kReadEvent; }

    void remove();
private:

    void update();
private:

    event_loop* loop_;
    const int  fd_;
    int        events_;
    int        revents_; // it's the received event types of epoll or poll
    int        index_; // used by Poller.
    bool event_handling_;

    read_event_callback read_callback_;
    event_callback write_callback_;
    event_callback close_callback_;
    event_callback error_callback_;
};
#endif//__NET_CHANNEL__H__