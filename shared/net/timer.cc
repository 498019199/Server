#include "timer.h"
#include "base/logger.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_ERROR << "Failed in timerfd_create";
    }
    return timerfd;
}

int64_t get_now_time()
{
    timeval val;
    gettimeofday(&val, nullptr);
    int64_t re = val.tv_sec * 1000 + val.tv_usec / 1000;
    return re;
}




timer_event::timer_event(int64_t interval_time, bool is_repeated, event_callback callback)
        :interval_time_(interval_time), is_repeated_(is_repeated),event_(std::move(callback))
{
    arrival_time_ = get_now_time() + interval_time;
}



timer_queue::timer_queue(event_loop *loop)
    :timer_fd_(createTimerfd()),
     loop_(loop),
     timer_chan_(loop, timer_fd_)
{
    timer_chan_.set_read_callback(std::bind(&timer_queue::on_timer, this));
    timer_chan_.enable_reeading();
}

timer_queue::~timer_queue()
{
    close(timer_fd_);
}

void timer_queue::add_timer(const timer_event::ptr& event, bool need_reset)
{
    bool is_reset = false;
    if (pending_event_.empty())
    {
        is_reset = true;
    }
    else
    {
        auto it = pending_event_.begin();
        if (event->arrival_time_ < it->second->arrival_time_)
        {
            is_reset = true;
        }
    }

    pending_event_.emplace(event->arrival_time_, event);
    if (is_reset && need_reset)
    {
        LOG_DEBUG << "need reset timer";
        reset_active_timer();
    }
}

void timer_queue::remove_timer(timer_event::ptr event)
{
    event->is_cancelled_ = true;
}

void timer_queue::reset_active_timer()
{
    if (pending_event_.empty())
    {
        LOG_DEBUG << "no timerevent pending, size = 0";
        return;
    }
    int64_t now = get_now_time();
    auto it = pending_event_.begin();
    if (it->first < now)
    {
        LOG_DEBUG<< "all timer events has already expire";
        return;
    }

    int64_t interval = it->first - now;
    itimerspec new_value{};
    memset(&new_value, 0, sizeof(new_value));
    timespec ts{};
    memset(&ts, 0, sizeof(ts));
    ts.tv_sec = interval / 1000;
    ts.tv_nsec = (interval % 1000) * 1000000;
    new_value.it_value = ts;
    int rt = timerfd_settime(timer_fd_, 0, &new_value, nullptr);
    if (rt != 0)
    {
        LOG_ERROR << "tiemr_settime error, interval=" << interval;
    }
    else
    {
        LOG_DEBUG << "reset timer succ, next occur time=" << it->first;
    }
}

void timer_queue::on_timer()
{
    int64_t now = get_now_time();
    auto it = pending_event_.begin();
    std::vector<timer_event::ptr> tmps;
    std::vector<std::pair<int64_t, std::function<void()>>> tasks;
    for (it = pending_event_.begin(); it != pending_event_.end(); ++it)
    {
        if (it->first <= now && !it->second->is_cancelled_)
        {
            tmps.emplace_back(it->second);
            tasks.emplace_back(std::make_pair(it->second->arrival_time_, it->second->event_));
        }
        else
        {
            break;
        }
    }

    pending_event_.erase(pending_event_.begin(), it);
    for( auto iter : tmps)
    {
        if (iter->is_repeated_)
        {
            iter->restart(get_now_time());
            add_timer(iter, false);
        }
    }

    reset_active_timer();
    for (const auto& iter : tasks)
    {
        iter.second();
    }
}