#ifndef __NET_ADDRESS__H__
#define __NET_ADDRESS__H__

#include "socket.h"



class tcp_connection
{
private:
    enum Connection_State 
    { 
        kDisconnected, 
        kConnecting, 
        kConnected, 
        kDisconnecting,
    };
public:
    tcp_connection(/* args */);
    ~tcp_connection();

    void set_state(Connection_State state);

    void set_close_callback(const close_callback_& cb) { connnection_callback_ = cb;}
    void set_connection_callback(const connnection_callback& cb) { connnection_callback_ = cb;}
    void set_message_callback(const message_callback& cb) { message_callback_ = cb;}
    void set_write_complete_callback(const write_complete_callback& cb) { write_complete_callback_ = cb;}

    void handle_read(int ts);
    void handle_write();
    void handle_close();
    void handle_error();
private:
    event_loop* loop;
    std::string name_;
    bool reading_;
    Connection_State state_;
    
    int sock_fd_;
    std::unique_ptr<channel> chan_;
    std::string input_buff_;
    std::string output_buff_;

    close_callback close_callback_;
    connnection_callback connnection_callback_;
    message_callback message_callback_;
    write_complete_callback write_complete_callback_;
};



#endif//__NET_ADDRESS__H__