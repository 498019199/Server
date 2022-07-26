#include "tcp_server.h"
#include "acceptor.h"
#include <functional>


tcp_server::tcp_server(const faddress& listen_addr, std::string name_args, Option opt)
    :acceptor_(new acceptor(listen_addr, opt == kReusePort)),
    name_(name_args),
    next_conne_id_(1)
{
    acceptor_->set_func(
        std::bind(&tcp_server::new_connetction, this, _1, _2));
}

tcp_server::~tcp_server()
{}

void tcp_server::new_connetction(int sockfd, const faddress& addr)
{
    char buf[64];
    snprintf(buf, sizeof buf, "-%s:%d#%d", ip.c_str(), port, next_conne_id_);
    ++next_conne_id_;
    std::string connet_name = name_ + buf;

    // LOG_INFO << "TcpServer::newConnection [" << name_
    //     << "] - new connection [" << connet_name
    //     << "] from " << addr.ip() << ":" << addr.port();
}