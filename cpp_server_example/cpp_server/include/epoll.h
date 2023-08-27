#pragma once
#include <sys/epoll.h>
#include <vector>

#include <unistd.h>
#include <string.h>

class Epoll{
private:
    int epfd;//epoll_create返回的fd
    struct epoll_event *events;//epoll_wait返回的事件数组
public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
};