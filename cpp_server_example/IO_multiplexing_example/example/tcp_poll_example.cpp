/*** 
 * @Author: tylerytr
 * @Date: 2023-08-23 16:24:39
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-23 16:55:18
 * @FilePath: /CPP_example/cpp_server_example/IO_multiplexing_example/example/tcp_poll_example.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/poll.h>

#define SERVER_PORT  8088  //服务器端口
#define SERVER_IP   "192.168.100.10" //服务端IP
#define MAXLEN  4096 //缓冲区最大大小
#define POLL_SIZE 1024

int main(int argc, char **argv) 
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    char buff[MAXLEN];
 
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);
 
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    //listen接口之后会一直监听客户端的连接，每次客户端连接，都会和其创建连接（三次连接时内核完成的，不是由应用程序去控制的）
	//三次握手不发生在任何API中，协议栈本身被动完成的。
    if (listen(listenfd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    
    //poll 原理和select类似
	struct pollfd fds[POLL_SIZE] = {0};//管理所有的fd
	fds[0].fd = listenfd;
	fds[0].events = POLLIN;//监测listenfd的连接（读）事件

	int max_fd = listenfd;
	int i = 0;
	for (i = 1;i < POLL_SIZE;i ++) {//初始化其他socket
		fds[i].fd = -1;
	}
	
	while (1) {

		int nready = poll(fds, max_fd+1, -1);

		if (fds[0].revents & POLLIN) { //发生了连接

			struct sockaddr_in client;
		    socklen_t len = sizeof(client);
		    if ((connfd = accept(listenfd, (struct sockaddr *)&client, &len)) == -1) {
		        printf("accept socket error: %s(errno: %d)\n", strerror(errno), errno);
		        return 0;
		    }

			fds[connfd].fd = connfd;
			fds[connfd].events = POLLIN;

			if (connfd > max_fd) max_fd = connfd;

			if (--nready == 0) continue;
		}

		//int i = 0;
		for (i = listenfd+1;i <= max_fd;i ++)  {

			if (fds[i].revents & POLLIN) {
				
				n = recv(i, buff, MAXLEN, 0);
		        if (n > 0) {
		            buff[n] = '\0';
		            printf("recv msg from client: %s\n", buff);

					send(i, buff, n, 0);
		        } else if (n == 0) { //断开连接

					fds[i].fd = -1;

		            close(i);
					
		        }
				if (--nready == 0) break;
			}
		}
	}
    close(listenfd);
    return 0;
}
