#ifndef __NET_POLLER__H__
#define __NET_POLLER__H__

#include <map>
#include <vector>

class channel;
class event_loop;

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

class poller
{
    static const int kInitEventListSize = 16;
    typedef std::map<int, channel*> channel_map;
    typedef std::vector<struct epoll_event> event_list;
public:
    poller(event_loop* loop);

    ~poller();

    void update_channel(channel* chan);

    void remove_channel(channel* chan);

    int poll(int timeout, std::vector<channel*>* active_chans);
private:
    void update(int operation, channel* chan);

    void fill_active_channels(int num_event, std::vector<channel*>* active_chans) const;

    static const char* operation_to_string(int op);
private:
    channel_map chans_;
    event_loop* loop_;
    event_list events_;

    int epoll_fd_;
};
#endif//__NET_POLLER__H__