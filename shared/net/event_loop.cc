#include "event_loop.h"
#include "channel.h"
#include "poller.h"

const int kPollTimeMs = 10000;

event_loop::event_loop()
    :poller_(new poller(this))
{}

void event_loop::update_channel(channel* chan)
{
    poller_->update_channel(chan);
}

void event_loop::remove_channel(channel* chan)
{
    poller_->remove_channel(chan);
}

void event_loop::loop()
{
    looping_ = true;
    quit_ = false;  // FIXME: what if someone calls quit() before loop() ?

    while (!quit_)
    {
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