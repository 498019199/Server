#include "tcp_server.h"
#include "net/socket.h"
#include "net/acceptor.h"
#include "net/tcp/tcp_connection.h"
#include "net/address.h"
#include "base/logger.h"
#include <functional>
#include <sys/syslog.h>


tcp_server::tcp_server(event_loop* loop, 
                    const faddress& listen_addr,
                    const char* name_args,
                    Option opt/* = kNoReusePort*/)
     :name_(name_args),
     ip_(listen_addr.ip()),
     port_(listen_addr.port()),
     acceptor_(new acceptor(loop, listen_addr, opt == kReusePort)),
     next_conne_id_(1),
     loop_(loop)
{
     acceptor_->set_new_connnect_callback(
         std::bind(&tcp_server::new_connetction, this, _1, _2));
}

void tcp_server::start()
{
    loop_->run_int_loop(
        [acptor = acceptor_.get()]
        {
            acptor->set_listening();
        });
}

void tcp_server::new_connetction(int sockfd, const faddress& addr)
{
    char buf[64];
    snprintf(buf, sizeof buf, "-%s:%d#%d", ip_.c_str(), port_, next_conne_id_);
    ++next_conne_id_;
    std::string conn_name = name_ + buf;

    LOG_TRACE << "tcp_server::new_connetction [" << name_
         << "] - new connection [" << conn_name
         << "] from " << addr.ip() << ":" << addr.port();
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
    conn->connect_established();
}

void tcp_server::remove_connection(const tcp_connection_ptr& conn)
{
    LOG_TRACE << "tcp_server::remove_connection [" << name_
             << "] - connection " << conn->name();
    size_t n = connection_map_.erase(conn->name());
    (void)n;
    assert(n == 1);
    conn->connect_destroyed();
}

tcp_server::~tcp_server()
{
    LOG_TRACE << "tcp_server::~tcp_server [" << name_ << "] destructing";
    for (auto& item : connection_map_)
    {
        auto conn(item.second);
        item.second.reset();
        conn->connect_destroyed();
    }
}

