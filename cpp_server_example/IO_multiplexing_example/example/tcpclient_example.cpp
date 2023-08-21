/*** 
 * @Author: tylerytr
 * @Date: 2023-08-21 15:17:05
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-21 15:31:08
 * @FilePath: /CPP_example/cpp_server_example/example/tcpclient_example.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>//控制台输出
#include <sys/socket.h>//创建socket
#include <netinet/in.h>//socket addr
#include <unistd.h>//close函数
#include <fcntl.h>//设置非阻塞
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h> //strlen

#define SERVER_PORT  8088  //服务器端口
#define SERVER_IP   "192.168.100.10" //服务端IP
#define MAXLEN  4096 //缓冲区最大大小

int main(){
    //创建套接字对象
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in servaddr{};
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //请求连接
    int ret = connect(socketfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));
    if(ret != 0){
        printf("connect server failed:error number:%d\n", errno);
        exit(-1);
    }


    while(1){
    
        const char *message = "this is from client message!";
        ret = send(socketfd, message, strlen(message), 0);
        
        if(ret > 0){
            char recvbuf[MAXLEN] = {0}; 
            ret = recv(socketfd, recvbuf, MAXLEN, 0);
            if(ret > 0){
                printf("recv message from server:%s\n", recvbuf);
            }else if(ret == 0){
                printf("server has closed!\n");
                close(socketfd);
            } 
            else{
                printf("recv error :%d\n", errno);
                close(socketfd);
            }
        }
        sleep(1);
    }

    return 0;


}


