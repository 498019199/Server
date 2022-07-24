#ifndef __NET_SOCKET__H__
#define __NET_SOCKET__H__
#include <netinet/in.h>
#include <stdint.h>

class faddress
{
public:
    explicit faddress(const struct sockaddr_in& addr)
        :addr_(addr)
    {
    }

    explicit faddress(const struct sockaddr_in6& addr)
        :addr6_(addr)
    {
    }

    // 使用ip和port 创建地址
    // ip 是"1.2.3.4""
    faddress(const char* ip, uint16_t port, bool ipv6 = false);

    //使用给定的端口号构造端点。
    //主要用于TcpServer侦听。
    faddress(uint16_t port, bool loop_back_only, bool ipv6 = false);

    const struct sockaddr* get_sockadd() const; 
    sa_family_t family() const { return addr_.sin_family; }
    std::string ip() const;
    uint16_t port() const;
private:
    union 
    {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };
    
};

class acceptor
{
public:
    typedef std::function<void(int sockfd, const faddress&)> new_connction_func;

    acceptor(const faddress& addr, bool reuseport);
    ~acceptor();

    void set_func(const new_connction_func& func) { new_connction_func_ = func;}
    void set_listening();
    bool get_listening() { return listening_};
private:
    new_connction_func new_connction_func_;
    bool listening_;
};

class i_socket
{};

class tpc_socket : public i_socket
{};

class upd_socket : public i_socket
{};
#endif//__NET_SOCKET__H__