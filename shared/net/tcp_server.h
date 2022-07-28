#ifndef __NET_TCP_SERVER__H__
#define __NET_TCP_SERVER__H__
#include "socket.h"
#include <map>

class tcp_server
{
    typedef std::function<void(event_loop*)> thread_init_callback;
    typedef std::map<std::string, tcp_connection_ptr> connection_map;
public:
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

    tcp_server(event_loop* loop, const faddress& listen_addr, std::string name_args, Option opt);

    ~tcp_server();

    void start();
private:
    void new_connetction(int sockfd, const faddress& addr);

    void remove_connection(const tcp_connection_ptr& conn);
private:
    std::string name_;
    std::string ip;
    int port;
    std::unique_ptr<faddress> acceptor_;
    int next_conne_id_;

    event_loop* loop_;

    connnection_callback connnection_callback_;
    message_callback message_callback_;
    write_complete_callback write_complete_callback_;
    thread_init_callback thread_init_callback_;

    connection_map connection_map_;
};
#endif // __NET_TCP_SERVER__H__