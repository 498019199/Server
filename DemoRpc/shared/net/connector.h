#ifndef __NET_CONNECT__H__
#define __NET_CONNECT__H__

#include "address.h"

// 连接器
class connector
{
public:
    connector(faddress addr);

    ~connector();
};


#endif//__NET_CONNECT__H__