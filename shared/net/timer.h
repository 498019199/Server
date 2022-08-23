#ifndef DEMO_RPC_TIMER_H
#define DEMO_RPC_TIMER_H

#include <set>
#include "socket.h"
class timer_event
{
public:
    typedef std::shared_ptr<timer_event> ptr;

    timer_event(int64_t interval_time, bool is_repeated, event_callback callback);

    void run() const;

    void restart(int64_t current_time);

    bool is_repeated() const { return is_repeated_; }
    bool is_cancel() const { return is_cancelled_; }
public:
    int64_t arrival_time_ = 0;
    int64_t interval_time_ = 0;
    bool is_repeated_ = false;
    bool is_cancelled_ = false;
    event_callback event_;
};

class timer_queue
{
    typedef std::pair<int64_t , timer_event*> timer_element;
    typedef std::pair<timer_event*, int64_t > active_element;
    typedef std::set<timer_element> timer_list;
    typedef std::set<active_element> active_list;
public:
    explicit timer_queue(event_loop* loop);

    void add_timer(timer_event::ptr event, bool need_reset);
    void remove_timer(timer_event::ptr event);
private:
    event_loop* loop_;
    const int timer_fd_;
};
#endif //DEMO_RPC_TIMER_H
