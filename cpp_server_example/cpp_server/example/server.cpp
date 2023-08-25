/*** 
 * @Author: tylerytr
 * @Date: 2023-08-23 23:07:37
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-25 16:13:09
 * @FilePath: /cpp_server/example/server.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>//read write
#include "common.h"

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
    
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");

    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    //读写缓冲区
    while(true){
        char buff[1024];
        bzero(&buff,sizeof(buff));//https://www.cnblogs.com/qingpeng/p/14591179.html 清零
        ssize_t read_bytes=read(clnt_sockfd,buff,sizeof(buff));
        
        if(read_bytes>0){
            printf("message from client fd %d: %s\n", clnt_sockfd, buff);
            write(clnt_sockfd,buff,sizeof(buff));//往client发信息

        }else if(read_bytes==0){
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }else if(read_bytes==-1){
            close(clnt_sockfd);
            errif(true, "socket read error");
        }

    }
    close(sockfd);
    return 0;
}