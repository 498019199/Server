#include "channel.h"

channel::channel(event_loop* loop, int fd)
    :loop_(loop),
    fd_(fd)
{}