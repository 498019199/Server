#ifndef __NET_TCP_SERVER__H__
#define __NET_TCP_SERVER__H__
#include <memory>
#include <string>
#include <functional>
class faddress;
class tcp_connection;

typedef std::shared_ptr<tcp_connection> tcp_connection_ptr;
typedef std::funcation<void (const tcp_connection_ptr&)> connnection_callback;
typedef std::funcation<void (const tcp_connection_ptr&)> write_complete_callback;
typedef std::funcation<void (const tcp_connection_ptr&, std::string, int ts)> message_callback;

class tcp_server
{
    typedef std::function<void(event_loop*)> thread_init_callback;
public:
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

public:
    tcp_server(const faddress& listen_addr, std::string name_args, Option opt);

    ~tcp_server();

    void start();
private:
    void new_connetction(int sockfd, const faddress& addr);
private:
    std::string name_;
    std::string ip;
    int port;
    std::unique_ptr<faddress> acceptor_;
    int next_conne_id_;

    connnection_callback connnection_callback_;
    MessageCallback messageCallback_;
    write_complete_callback write_complete_callback_;
    thread_init_callback thread_init_callback_;
};
#endif // __NET_TCP_SERVER__H__