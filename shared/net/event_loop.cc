#include "event_loop.h"

event_loop::event_loop()
    :poller_(new poller(this))
{}

void event_loop::update_channel(channel* chan)
{}

void event_loop::remove_channel(channel* chan)
{}