#include "tcp_server.h"
#include "net/socket.h"
#include "net/acceptor.h"
#include "net/tcp/tcp_connection.h"
#include "net/address.h"
#include <functional>


tcp_server::tcp_server(event_loop* loop, 
                    const faddress& listen_addr,
                    const char* name_args,
                    Option opt/* = kNoReusePort*/)
     :name_(name_args),
     acceptor_(new acceptor(loop, listen_addr, opt == kReusePort)),
     next_conne_id_(1),
     loop_(loop)
{
     acceptor_->set_new_connnect_callback(
         std::bind(&tcp_server::new_connetction, this, _1, _2));
}

tcp_server::~tcp_server()
{}

void tcp_server::start()
{
    loop_->run_int_loop(
        std::bind(&acceptor::set_listening, acceptor_.get()));
}

void tcp_server::new_connetction(int sockfd, const faddress& addr)
{
    char buf[64];
    snprintf(buf, sizeof buf, "-%s:%d#%d", ip.c_str(), port, next_conne_id_);
    ++next_conne_id_;
    std::string conn_name = name_ + buf;

    // LOG_INFO << "TcpServer::newConnection [" << name_
    //     << "] - new connection [" << conn_name
    //     << "] from " << addr.ip() << ":" << addr.port();
    faddress local_addr(sockets::get_addr(sockfd));
    tcp_connection_ptr conn(new tcp_connection(loop_, 
                                            conn_name.c_str(),
                                            sockfd,
                                            local_addr,
                                            addr));
    
    connection_map_[conn_name] = conn;
    conn->set_connection_callback(connnection_callback_);
    conn->set_write_complete_callback(write_complete_callback_);
    conn->set_message_callback(message_callback_);
    conn->set_close_callback(
        std::bind(&tcp_server::remove_connection, this, _1));

}

void tcp_server::remove_connection(const tcp_connection_ptr& conn)
{

}

