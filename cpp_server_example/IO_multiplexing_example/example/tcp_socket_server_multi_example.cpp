/*** 
 * @Author: tylerytr
 * @Date: 2023-08-21 16:04:41
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-23 15:09:01
 * @FilePath: /CPP_example/cpp_server_example/IO_multiplexing_example/example/tcp_socket_server_multi_example.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>
#include <sys/socket.h>//创建socket
#include <netinet/in.h>//socket addr
#include <unistd.h>//close函数


#define SERVER_PORT  8088  //服务器端口
#define SERVER_IP   "192.168.100.10" //服务端IP
#define MAXLEN  4096 //缓冲区最大大小

#include <pthread.h>
//一个线程8M,4G内存，最多512个线程（仅仅是线程就占用了4个G）
//C10k

void *client_routine(void *arg) { //

	int connfd = *(int *)arg;

	char buff[MAXLEN];

	while (1) {

		int n = recv(connfd, buff, MAXLEN, 0);
        if (n > 0) {
            buff[n] = '\0';
            printf("recv msg from client: %s\n", buff);

	    	send(connfd, buff, n, 0);
        } else if (n == 0) {
            close(connfd);
			break;
        }

	}

	return NULL;
}


using namespace std;
int main(){
    //读socket的数组
    char buff[MAXLEN];
    //创建一个tcp socket
    int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketFd==-1){
        cout<<"create socket error"<<endl;
        return -1;
    }
    
    //设置socket监听的地址和端口
    sockaddr_in sockAddr{};
    sockAddr.sin_port = htons(SERVER_PORT);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);

    //绑定socket
    if (bind(socketFd, (sockaddr *) &sockAddr, sizeof(sockAddr)) == -1) {
        cout << "bind error" << endl;
        return -1;
    }
    //listen接口之后会一直监听客户端的连接，每次客户端连接，都会和其创建连接（三次连接时内核完成的，不是由应用程序去控制的）
    //三次握手不发生在任何API中，协议栈本身被动完成的。
    if (listen(socketFd, 10) == -1) {
        cout << "listen error" << endl;
        return -1;
    }

    // //这个版本只能用于多个客户端连接，而只有第一个客户端可以发送数据
    // struct sockaddr_in client;
    // socklen_t len = sizeof(client);
    // int connFd;
    // if ((connFd = accept(socketFd, (struct sockaddr *)&client, &len)) == -1) {
    //     //printf("accept socket error: %s(errno: %d)\n", strerror(errno), errno);
    //     cout<<"accept error: "<<errno<<endl;
    //     return -1;
    // }

    // cout<<"waiting for client's request"<<endl;
    // while(1){
    //     //阻塞等待第一个客户端的数据的到来
    //     int n = recv(connFd, buff, MAXLEN, 0);
    //     //cout<<n<<endl;
    //     if(n==0){
    //         close(connFd);
    //     }else if(n>0){
    //         buff[n] = '\0';
    //         printf("recv msg from client: %s\n", buff);

    //     }

    // }
    while(1){
        struct sockaddr_in client;
	    socklen_t len = sizeof(client);
        int connFd;
	    if ((connFd = accept(socketFd, (struct sockaddr *)&client, &len)) == -1) {
	        cout<<"accept error: "<<errno<<endl;
	        return 0;
	    }
        //1个客户端1个线程去处理，早期的网络模型就是这种模式
		pthread_t threadid;
		pthread_create(&threadid, NULL, client_routine, (void*)&connFd);


    }


    close(socketFd);
    return 0;
}