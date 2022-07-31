#include "event_loop.h"
#include "channel.h"
#include "poller.h"
#include "base/logger.h"

const int kPollTimeMs = 10000;

event_loop::event_loop()
    :poller_(new poller(this))
{}

event_loop::~event_loop()
{
    
}

void event_loop::update_channel(channel* chan)
{
    poller_->update_channel(chan);
}

void event_loop::remove_channel(channel* chan)
{
    poller_->remove_channel(chan);
}

void event_loop::run_int_loop(functor func)
{
    func();
}

void event_loop::queue_in_loop(functor func)
{
    
}

void event_loop::loop()
{
    looping_ = true;
    quit_ = false;  // FIXME: what if someone calls quit() before loop() ?

    while (!quit_)
    {
        LOG_TRACE << "loop !" << std::endl;

        active_chans_.clear();
        int poll_ret_time = poller_->poll(kPollTimeMs, &active_chans_);

        // TODO sort channel by priority
        event_handling_ = true;
        for (channel* chan : active_chans_)
        {
            current_chan_ptr_ = chan;
            current_chan_ptr_->handle_event(poll_ret_time);
        }
        current_chan_ptr_ = nullptr;
        event_handling_ = false;
    }
    
}