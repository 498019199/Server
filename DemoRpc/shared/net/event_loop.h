#ifndef __NET_LOOP_EVENET__H__
#define __NET_LOOP_EVENET__H__

#include <functional>
#include <memory>
#include <vector>
#include "timer.h"

class channel;
class poller;

class event_loop
{
    typedef std::vector<channel*> channel_list;
    typedef std::function<void()> functor;
public:
    event_loop();
    ~event_loop();
    
    void update_channel(channel* chan);
    void remove_channel(channel* chan);

    void run_int_loop(functor func);
    void queue_in_loop(functor func);

    void loop();
    void quit();

    timer_queue* get_timer();
private:
    bool looping_;
    bool quit_;
    bool event_handling_;

    std::unique_ptr<poller> poller_;
    channel_list active_chans_;
    channel* current_chan_ptr_ = nullptr;
    timer_queue* timer_queue_ = nullptr;
};
#endif//__NET_LOOP_EVENET__H__