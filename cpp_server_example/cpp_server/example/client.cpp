/*** 
 * @Author: tylerytr
 * @Date: 2023-08-23 23:07:43
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-25 16:21:09
 * @FilePath: /cpp_server/example/client.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    //bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 客户端不进行bind操作

    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");


    while(true){
        char buff[1024];
        bzero(&buff, sizeof(buff));
        scanf("%s", buff);
        ssize_t write_bytes = write(sockfd, buff, sizeof(buff));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }

        //读信息
        bzero(&buff, sizeof(buff));
        ssize_t read_bytes = read(sockfd, buff, sizeof(buff));
                if(read_bytes>0){
            printf("message from client fd %d: %s\n", sockfd, buff);
            write(sockfd,buff,sizeof(buff));//往client发信息

        }else if(read_bytes==0){
            printf("client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }else if(read_bytes==-1){
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}