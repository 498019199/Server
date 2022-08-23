#include "timer.h"
#include "base/logger.h"

#include <sys/timerfd.h>
#include <unistd.h>

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_ERROR << "Failed in timerfd_create";
    }
    return timerfd;
}

timer_event::timer_event(int64_t interval_time, bool is_repeated, event_callback callback)
    :interval_time_(interval_time), is_repeated_(is_repeated),event_(callback)
{

}

void timer_event::run() const
{
    event_();
}

void timer_event::restart(int64_t current_time)
{
    arrival_time_ = current_time + interval_time_;
}





timer_queue::timer_queue(event_loop *loop)
    :loop_(loop),
     timer_fd_(createTimerfd())
{

}

void timer_queue::add_timer(timer_event::ptr event, bool need_reset)
{

}

void timer_queue::remove_timer(timer_event::ptr event)
{

}
