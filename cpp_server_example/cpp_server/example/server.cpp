/*** 
 * @Author: tylerytr
 * @Date: 2023-08-23 23:07:37
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-27 18:19:17
 * @FilePath: /cpp_server/example/server.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>//read write
#include <fcntl.h>//setnonblocking
#include <sys/epoll.h>
#include <errno.h>
#include "common.h"


#define MAX_EVENTS 1024
#define READ_BUFFER 1024
void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd==-1,"socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");
    
    int epfd = epoll_create1(0);//用flag代替了epoll_create里面的size
    errif(epfd == -1, "epoll create error");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    
    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;//ET模式 非阻塞
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);//往红黑树里面注册事件


    //读写缓冲区
    while(true){
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);//等待事件;非阻塞；
        errif(nfds == -1, "epoll wait error");

        for(int i=0;i<nfds;i++){
            //遍历已有事件

            if(events[i].data.fd==sockfd){
                //表示有新的链接
                struct sockaddr_in clnt_addr;
                bzero(&clnt_addr, sizeof(clnt_addr));
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                //socket处理 accept
                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                errif(clnt_sockfd == -1, "socket accept error");
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

                //注册到红黑树里面
                bzero(&ev, sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);//往红黑树里面注册事件
            }else if(events[i].events & EPOLLIN){      //可读事件
                char buf[READ_BUFFER];
                while(true){
                    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read>0){
                        printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    }else if(bytes_read<=0&&errno == EINTR){
                        printf("continue reading");
                        continue;
                    }else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    }else if(bytes_read == 0){//EOF 断开链接
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
            }else{//其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
    }
    close(sockfd);
    return 0;
}