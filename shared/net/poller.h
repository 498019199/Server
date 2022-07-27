#ifndef __NET_POLLER__H__
#define __NET_POLLER__H__
#include "event_loop.h"
#include "channel.h"
#include <map>

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

class poller
{
    static const int kInitEventListSize = 16;
public:
    poller(event_loop* loop);

    ~poller();

    void update_channel(channel* chan);

    void remove_channel(channel* chan);

private:
    void update(int operation, channel* chan);

    static const char* operation_to_string(int op);
private:
    typedef std::map<int, channel*> channel_map;
    channel_map chans_;
    int epoll_fd_;
};
#endif//__NET_POLLER__H__