#ifndef __NET_TCP_SERVER__H__
#define __NET_TCP_SERVER__H__
#include "net/socket.h"
#include <map>
#include <functional>

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

    tcp_server(event_loop* loop, 
            const faddress& listen_addr, 
            const char* name_args,
            Option opt = kNoReusePort);

    ~tcp_server();

    event_loop* get_loop() { return loop_;}

    void start();

    void set_connection_callback(const connnection_callback& callback) { connnection_callback_ = callback;};
    void set_message_callback(const message_callback& callback) { message_callback_ = callback;};

    std::string name() const{ return name_;}
    std::string ip() const{ return ip_;}
private:
    void new_connetction(int sockfd, const faddress& addr);

    void remove_connection(const tcp_connection_ptr& conn);
private:
    std::string name_;
    std::string ip_;
    int port;
    std::unique_ptr<acceptor> acceptor_;
    int next_conne_id_;

    event_loop* loop_;

    connnection_callback connnection_callback_;
    message_callback message_callback_;
    write_complete_callback write_complete_callback_;
    thread_init_callback thread_init_callback_;

    connection_map connection_map_;
};
#endif // __NET_TCP_SERVER__H__