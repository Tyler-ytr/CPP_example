#include "inetaddress.h"
#include <string.h>

InetAddress::InetAddress()  
{
    bzero(&addr, sizeof(addr));
    addr_len = sizeof(addr);
}
InetAddress::InetAddress(const char*ip, uint16_t port){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}

InetAddress::~InetAddress()
{
}