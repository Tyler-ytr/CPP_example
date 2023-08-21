
#include <iostream>//控制台输出
#include <sys/socket.h>//创建socket
#include <netinet/in.h>//socket addr
#include <sys/epoll.h>//epoll
#include <unistd.h>//close函数
#include <fcntl.h>//设置非阻塞

using namespace std;


int main(){
    const int EVENTS_SIZE = 20;
    //读socket的数组
    char buff[1024];
    //创建一个tcp socket
    int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //设置socket监听的地址和端口
    sockaddr_in sockAddr{};
    sockAddr.sin_port = htons(8088);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);
    //绑定socket
    if (bind(socketFd, (sockaddr *) &sockAddr, sizeof(sockAddr)) == -1) {
        cout << "bind error" << endl;
        return -1;
    }

    //开始监听socket,当调用listen之后,进程就可以调用accept来接受一个外来的请求;对应于博客 进入等待连接请求状态
    if (listen(socketFd, 10) == -1) {
        cout << "listen error" << endl;
        return -1;
    }


    //创建一个epoll,size已经不起作用了,一般填1就好了
    int eFd = epoll_create(1);
    //把socket包装成一个epoll_event对象并添加到epoll中
    epoll_event epev{};
    epev.events = EPOLLIN;//可以响应的事件,这里只响应可读就可以了
    epev.data.fd = socketFd;//socket的文件描述符
    epoll_ctl(eFd, EPOLL_CTL_ADD, socketFd, &epev);//添加到epoll中

    //回调事件的数组,当epoll中有响应事件时,通过这个数组返回
    epoll_event events[EVENTS_SIZE];

    //整个epoll_wait 处理都要在一个死循环中处理
    while(1){
        //这个函数会阻塞,直到超时或者有响应事件发生
        int eNum = epoll_wait(eFd, events, EVENTS_SIZE, -1);
        if (eNum == -1) {
            cout << "epoll_wait" << endl;
            return -1;
        }

        //遍历所有的事件
        for(int i =0;i<eNum;i++){
            //判断这次是不是socket可读(是不是有新的连接)
            if (events[i].data.fd == socketFd){
                
                if (events[i].events & EPOLLIN){ 
                    sockaddr_in cli_addr{};
                    socklen_t length = sizeof(cli_addr);
                    //接受来自socket连接;对应于博客 受理客户端的连接请求
                    int fd = accept(socketFd, (sockaddr *) &cli_addr, &length);
                    if (fd > 0){
                        //设置响应事件,设置可读和边缘(ET)模式

                        epev.events = EPOLLIN | EPOLLET;
                        epev.data.fd = fd;
                        //设置连接为非阻塞模式
                        int flags = fcntl(fd, F_GETFL, 0);
                        if (flags < 0) {
                            cout << "set no block error, fd:" << fd << endl;
                            continue;
                        }
                        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
                            cout << "set no block error, fd:" << fd << endl;
                            continue;
                        }

                        //将新的连接添加到epoll中
                        epoll_ctl(eFd, EPOLL_CTL_ADD, fd, &epev);
                        cout << "client on line fd:" << fd << endl;
                    }
                }
            }else{
                    //不是socket的响应事件
                    //判断是不是断开和连接出错
                    //因为连接断开和出错时,也会响应`EPOLLIN`事件
                    if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP){
                        //出错时,从epoll中删除对应的连接
                        //第一个是要操作的epoll的描述符
                        //因为是删除,所有event参数填写null就可以了
                        epoll_ctl(eFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        cout << "client out fd:" << events[i].data.fd << endl;
                        close(events[i].data.fd);
                    }else if(events[i].events & EPOLLIN){
                        //如果是可读事件
                        //如果在windows中,读socket中的数据要用recv()函数
                        int len = read(events[i].data.fd, buff, sizeof(buff));
                        //如果读取数据出错,关闭并从epoll中删除连接
                        if (len == -1) {
                            epoll_ctl(eFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                            cout << "client out fd:" << events[i].data.fd << endl;
                            close(events[i].data.fd);
                        }else{
                            //正常读取,打印读到的数据
                            cout << buff << endl;
                            //向客户端发数据
                            char a[] = "Server received!";
                            //如果在windows中,向socket中写数据要用send()函数
                            write(events[i].data.fd, a, sizeof(a));
                        }
                    }
                }
        }
    }


}