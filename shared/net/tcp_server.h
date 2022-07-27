#ifndef __NET_TCP_SERVER__H__
#define __NET_TCP_SERVER__H__
#include <memory>
#include <string>
class faddress;

class tcp_server
{
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

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;
};
#endif // __NET_TCP_SERVER__H__