#ifndef DEMO_RPC_TIMER_H
#define DEMO_RPC_TIMER_H

#include <set>
#include <map>
#include "socket.h"
struct timer_event
{
    typedef std::shared_ptr<timer_event> ptr;

    timer_event(int64_t interval_time, bool is_repeated, event_callback callback)
            :interval_time_(interval_time), is_repeated_(is_repeated),event_(std::move(callback))
    {
    }

    void run() const
    {
        event_();
    }

    void restart(int64_t current_time)
    {
        arrival_time_ = current_time + interval_time_;
    }

    int64_t arrival_time_ = 0;
    int64_t interval_time_ = 0;
    bool is_repeated_ = false;
    bool is_cancelled_ = false;
    event_callback event_;
};

class timer_queue
{
public:
    explicit timer_queue(event_loop* loop);
    ~timer_queue();

    void add_timer(const timer_event::ptr& event, bool need_reset);
    void remove_timer(timer_event::ptr event);

    void reset_active_timer();

    void on_timer();
private:
    event_loop* loop_;
    const int timer_fd_;

    std::multimap<int64_t, timer_event::ptr> pending_event_;
};
#endif //DEMO_RPC_TIMER_H
