#include "channel.h"
#include "event_loop.h"

channel::channel(event_loop* loop, int fd)
    :loop_(loop),
    fd_(fd)
{}

void channel::handle_event(int receive_time)
{
    
}