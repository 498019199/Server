#include "event_loop.h"
#include "poller.h"

event_loop::event_loop()
    :poller_(new poller(this))
{}

void event_loop::update_channel(channel* chan)
{

}

void event_loop::remove_channel(channel* chan)
{}

void event_loop::loop()
{
    looping_ = true;
    quit_ = false;  // FIXME: what if someone calls quit() before loop() ?

    while (!quit_)
    {
        active_chans_.clear();
        poller_->
    }
    
}