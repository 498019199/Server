#ifndef __NET_ACCEPTOR__H__
#define __NET_ACCEPTOR__H__

#include <functional>

#include "address.h"
#include "channel.h"
#include "event_loop.h"

class acceptor
{
public:
    typedef std::function<void(int sockfd, const faddress&)> new_connction_func;

    acceptor(event_loop* eve_loop, const faddress& addr, bool reuseport);

    ~acceptor();

    void set_func(const new_connction_func& func) { new_connction_func_ = func;}
    void set_listening();
    bool get_listening() { return listening_; }
private:
    int accept_fd_;
    bool listening_;
    channel acceptor_chan_;
    new_connction_func new_connction_func_;
};
#endif // __NET_ACCEPTOR__H__