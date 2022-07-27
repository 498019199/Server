#ifndef __NET_LOOP_EVENET__H__
#define __NET_LOOP_EVENET__H__


#include <memory>
#include <vector>
class channel;
class poller;

class event_loop
{
    typedef std::vector<channel*> channel_list;
public:
    event_loop();

    void update_channel(channel* chan);
    
    void remove_channel(channel* chan);

    void loop();

    void quit();
private:
    bool looping_;
    bool quit_;

    std::unique_ptr<poller> poller_;
    channel_list active_chans_;
};
#endif//__NET_LOOP_EVENET__H__