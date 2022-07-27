#ifndef __NET_LOOP_EVENET__H__
#define __NET_LOOP_EVENET__H__

#include "poller.h"

#include <memory>

class event_loop
{
    event_loop();

    void update_channel(channel* chan);
    
    void remove_channel(channel* chan);
private:
    std::unique_ptr<poller> poller_;
};
#endif//__NET_LOOP_EVENET__H__