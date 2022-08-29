#ifndef __NET_ACCEPTOR__H__
#define __NET_ACCEPTOR__H__

#include <functional>

#include "base/copyable.h"
#include "address.h"
#include "channel.h"
#include "event_loop.h"

// 接收器
class acceptor:noncopyable
{
public:
    typedef std::function<void(int sockfd, const faddress&)> new_connction_func;

    acceptor(event_loop* loop, const faddress& addr, bool reuseport);

    ~acceptor() = default;

    void set_new_connnect_callback(const new_connction_func& func) { new_connction_func_ = func;}
    void set_listening();
    bool get_listening() const { return listening_; }

private:

    void handle_read();
private:
    int accept_fd_;
    bool listening_;
    channel acceptor_chan_;
    new_connction_func new_connction_func_;
};
#endif // __NET_ACCEPTOR__H__