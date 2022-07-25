#ifndef __NET_ACCEPTOR__H__
#define __NET_ACCEPTOR__H__

#include <functional>
#include "socket.h"
#include "acceptor.h"

class acceptor
{
public:
    typedef std::function<void(int sockfd, const faddress&)> new_connction_func;

    acceptor(const faddress& addr, bool reuseport);

    ~acceptor();

    void set_func(const new_connction_func& func) { new_connction_func_ = func;}
    void set_listening();
    bool get_listening() { return listening_; }
private:
    int accept_fd_;
    new_connction_func new_connction_func_;
    bool listening_;
};
#endif // __NET_ACCEPTOR__H__