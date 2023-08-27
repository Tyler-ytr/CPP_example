#pragma once
#include<unistd.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<sys/socket.h>
class InetAddress;
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int _fd);
    ~Socket();

    void bind(InetAddress*addr);
    void listen();
    void setnonblocking();

    int accept(InetAddress*addr);//返回clientfd

    int getFd();
};
