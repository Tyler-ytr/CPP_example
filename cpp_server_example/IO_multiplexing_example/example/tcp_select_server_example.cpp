/*** 
 * @Author: tylerytr
 * @Date: 2023-08-21 16:04:41
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-23 16:07:59
 * @FilePath: /CPP_example/cpp_server_example/IO_multiplexing_example/example/tcp_select_server_example.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>
#include <sys/socket.h>//创建socket
#include <netinet/in.h>//socket addr
#include <unistd.h>//close函数
#include <sys/select.h>

#define SERVER_PORT  8088  //服务器端口
#define SERVER_IP   "192.168.100.10" //服务端IP
#define MAXLEN  4096 //缓冲区最大大小

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

    //fd_set，每个socket对应一个bit位，bit位为1，代表该socket发生了事件。
    fd_set rfds, rset, wfds, wset;//rfds:是需要监控读的集合(从用户copy到内核)；rset:是发生了读事件的socket的集合（从内核copy到用户）

    FD_ZERO(&rfds);//清空bit位
	FD_SET(socketFd, &rfds);//设置监听的socket的bit位
	FD_ZERO(&wfds);


    //比如socketFd为100，那么select就需要从第一位bit监控到101位的bit
	int max_fd = socketFd;//select第一个参数为监听socket的最大值+1
    int connfd;
    int n;
    while(1){
        rset = rfds;
		wset = wfds;
        //调用select系统函数进入内核检查哪个连接的数据到达
        int nready = select(max_fd+1, &rset, &wset, NULL, NULL);//如果最后一个参数为NULL，那么是阻塞的，一直到有事件发生（nready会大于0）
        if (FD_ISSET(socketFd, &rset)) { //socketFd监听到了客户端连接
            struct sockaddr_in client;
		    socklen_t len = sizeof(client);
		    if ((connfd = accept(socketFd, (struct sockaddr *)&client, &len)) == -1) {
		        cout<<"accept error: "<<errno<<endl;
		        return 0;
		    }
            FD_SET(connfd, &rfds);//加入到读的集合中，代表要监听某客户端的读（recv）事件
            //如果客户端没有回收，connfd比max_fd大，但是如果客户端回收了fd，那么数字会变小，因此需要判断当前的connfd是否大于max_fd
            if (connfd > max_fd) max_fd = connfd;//重新设置最大的fd
            if (--nready == 0) continue;//只有一个客户端的连接
        
        
        }
        
        int i=0;
        //注意0-标志输入，1-标志输出，2-错误，然后是socketFd，
        //然后每次客户端连接后，connfd会依次增大。回收后connfd会变小，但是无论如何都比socketFd大
        //因此这里检测的是所有客户端是否有客户端与服务器进行了通信
        for(i=socketFd+1;i<=max_fd;i++){
            if (FD_ISSET(i, &rset)){
                //检测某个客户端是否已经准备好了数据或者关闭（正常关闭也会设置读数据）
                n = recv(i, buff, MAXLEN, 0);
                if(n>0){
                    buff[n] = '\0';
                    cout<<"recv msg from client: "<<buff<<endl;
                    //设置可以往该客户端socket发送数据
                    FD_SET(i, &wfds);
                    send(i, buff, n, 0);//直接将接收到的数据发送给客户端
                }else if(n==0){
                    //收到的字节为0，代表该客户端正常关闭了
                    FD_CLR(i, &rfds);//清空该客户端的读事件
                    close(i);
                }
                if (--nready == 0) break;//没有客户端的读事件了

            }
        }
    }
   
    


    close(socketFd);
    return 0;
}