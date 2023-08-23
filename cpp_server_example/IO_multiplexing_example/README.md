<!--
 * @Author: tylerytr
 * @Date: 2023-08-20 22:41:46
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-23 17:15:32
 * @FilePath: /CPP_example/cpp_server_example/IO_multiplexing_example/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->
# IO多路复用

## epoll使用
1. epoll就是一种典型的I/O多路复用技术: epoll技术的最大特点是支持高并发。
   1. 传统多路复用技术select，poll，在并发量达到1000-2000，性能就会明显下降
### epoll
[epoll底层源码解析](https://www.cnblogs.com/mysky007/p/12284842.html)
1. [eventpoll结构体](https://www.cnblogs.com/mysky007/p/12284842.html)中包含了:
   1. 自旋锁 lock: 用于保护对于这个结构体的访问
   2. mutex用来保证epoll使用文件的时候文件不会被删除
   3. wait_queue_head_t wq; 这个队列里存放的是执行epoll_wait从而等待的进程队列；有数据到达的时候，epoll_wait会通过回调函数default_wake_function唤醒
   4. wait_queue_head_t poll_wait; 这个队列里存放的是该eventloop作为poll对象的一个实例，加入到等待的队列
   5. struct list_head **rdllist**; 就绪的描述符的链表。当有的连接数据就绪的时候，内核会把就绪的连接放到 rdllist 链表里。这样应用进程只需要判断链表就能找出就绪进程，而不用去遍历整棵树。
   6. struct rb_root_cached **rbr**; 红黑树的根节点，这颗树中存储着所有添加到epoll中的需要监控的事件；红黑树的值为epitem中的sockfd
   7. struct epitem *ovflist; 
   8. struct wakeup_source *ws; wakeup_source used when ep_scan_ready_list is running
   9. struct user_struct *user; The user that created the eventpoll descriptor
   4.  struct file *file 这是eventloop对应的匿名文件
   5.  int visited; used to optimize loop detection check 
   6.  struct list_head visited_list_link;
   7.  unsigned int napi_id;（#ifdef CONFIG_NET_RX_BUSY_POLL） /* used to track busy poll napi_id */
2. [epitem结构体] 这是红黑树的节点，也是双向链表的节点;
    1. **rbn** 红黑树节点
    2. **rdllink**;next 双向链表头以及指针
    3. **ffd**;nwait; 监听的fd;文件被监听的次数
    4. pwqlist; List containing poll wait queues
    5. ep; 当前epollitem所属的eventpoll
    6. fllink;  List header used to link this item to the "struct file" items list
    7. ws; wakeup_source used when EPOLLWAKEUP is set
    8. event; The structure that describe the interested events and the source fd
3. 一般有如下情况，会使操作系统把节点插入到双向链表中
   1. 客户端完成三次握手服务器要accept();
   2. 当客户端关闭连接，服务器也要调用close()关闭
   3. 客户端发送数据来的服务器要调用read(),recv()函数来收数据
   4. 当可以发送数据时服务武器可以调用send(),write()
   5. 其他情况.


#### epoll三个系统调用
1. `int epoll_create(int __size)` 创建一个epoll，_size 参数在linux2.6内核之后就没有什么作用了, 但是要>0，一般直接填1就好了。函数返回创建的epoll的文件描述符，如果创建失败，会返回 -1。
   1. 原理:`struct eventpoll *ep = (struct eventpoll*)calloc(1, sizeof(struct eventpoll)); ` 当进程调用epoll_create方法时，Linux内核会创建一个eventpoll结构体；从[linux数据结构](http://www.embeddedlinux.org.cn/linuxkernel/ds/ds.html)视角来看，eventpoll处于:`task_struct`->`files_struct`->`file`->`private_data`中; 
    2. 可以参考[该博客](https://blog.csdn.net/baidu_41388533/article/details/110134366)的图示;可以发现主要该结构体通过一个红黑树`rdllist`和一个双向链表`rdllist`来存放事件数据;
2. `int epoll_ctl(int __epfd, int __op, int __fd,struct epoll_event *__event)` : 把一个socket以及这个socket相关的**事件**添加到这个epoll对象描述符中去，目的就是通过这个epoll对象来监视这个socket【客户端的TCP连接】上数据的来往情况；当有数据来往时，系统会通知我们;
   1. efpd：epoll_create()返回的epoll对象描述符
   2. op 类型：
      1. EPOLL_CTL_ADD添加事件：内核就会为我们创建出一个epitem实例,然后往红黑树上添加一个节点;每个客户端连入服务器后，服务器都会产生一个对应的socket，每个连接这个socket值都不重复所以，这个socket就是红黑树中的key，把这个节点添加到红黑树上去;
      2. EPOLL_CTL_MOD：修改事件你 用了EPOLL_CTL_ADD把节点添加到红黑树上之后，才存在修改
      3. EPOLL_CTL_DEL：是从红黑树上把这个节点干掉这会导致这个socket【这个tcp链接】上无法收到任何系统通知事件
   3. 需要监听的fd
   4. event:告诉内核需要监听什么事;结构体如下:
      ```c++
      typedef union epoll_data {
          void *ptr;
          int fd;
          __uint32_t u32;
          __uint64_t u64;
      } epoll_data_t;

      struct epoll_event {
          __uint32_t events; /* Epoll events */
          epoll_data_t data; /* User data variable */
      };
      ```
      events可以是以下几个宏的集合：
        - EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
        - EPOLLOUT：表示对应的文件描述符可以写；
        - EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
        - EPOLLERR：表示对应的文件描述符发生错误；
        - EPOLLHUP：表示对应的文件描述符被挂断；
        - EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
        - EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
    5. **总结**：该函数相当于是根据`op`对于红黑树节点进行增删改操作;
3. `int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout)` 阻塞一小段时间并且等待事件发生，返回事件集合;相当于[IO模型中的第三个模型](https://segmentfault.com/a/1190000039898780#item-3);相当于是遍历双向链表`rdllist`，把双向链表中的节点数据拷贝出去，拷贝完毕的会移除;
   1. epfd: epoll_create()返回的epoll对象描述符。
   2. events: 长度是maxevents;  表示本次`epoll_wait`调用准备好的读写事件;
   3. 参数timeout：阻塞等待的时长

4. epoll_event_callback() 上述三个接口是用户的系统调用；本接口由操作系统进行操作；会在如下五种情况下触发;大概的含义是根据TCP给定的socketid从红黑树找到节点然后根据情况(epitem的rdy)来决定是否加入到双向链表以及是否修改rdy;
   1. 客户端connect()连入，服务器处于SYN_RCVD状态时
   2. 三次握手完成，服务器处于ESTABLISHED状态时
   3. 客户端close()断开连接，服务器处于FIN_WAIT_1和FIN_WAIT_2状态时
   4. 客户端send/write()数据，服务器可读时
   5. 服务器可以发送数据时
5. epoll底层实现中有两个关键的数据结构，一个是eventpoll另一个是epitem，其中eventpoll中有两个成员变量分别是rbr和rdlist,前者指向一颗红黑树的根，后者指向双向链表的头。而epitem则是红黑树节点和双向链表节点的综合体，也就是说epitem即可作为树的节点，又可以作为链表的节点，并且epitem中包含着用户注册的事件。使用epoll的主要流程如下:
   1. 当用户调用epoll_create()时，会创建eventpoll对象（包含一个红黑树和一个双链表）；
   2. 而用户调用epoll_ctl(ADD)时，会在红黑树上增加节点（epitem对象）；
   3. 接下来，操作系统会默默地在通过epoll_event_callback()来管理eventpoll对象。当有事件被触发时，操作系统则会调用epoll_event_callback函数，将含有该事件的epitem添加到双向链表中。
   4. 当用户需要管理连接时，只需通过epoll_wait()从eventpoll对象中的双链表下"摘取"epitem并取出其包含的事件即可。

## CPP tcp server 简化版本
### tcp client 
1. example 中实现了tcp_client_example
2. tcp_client中：
   1. 创建套接字对象，通过connect请求连接到socketfd;然后在死循环中通过send发`"this is from client message!"`
### tcp socket server
1. tcp_socket_server_example.cpp中实现了基本的socket 收发功能；只能够接受一个客户端的信息
2. tcp_socket_server_multi_example.cpp中通过多线程能够接受多个客户端的信息，优点是结构简单，缺点是不适用高并发情况(一个线程8M,4G内存，最多512个线程（仅仅是线程就占用了4个G)


### tcp select server
1. 使用select完成IO多路复用,案例位于tcp_socket_server_example.cpp
2. select接口如下:
      ```
         #include <sys/select.h>
         //readfds、writefds、errorfds 是三个文件描述符集合。select 会遍历每个集合的前 nfds 个描述符，分别找到可以读取、可以写入、发生错误的描述符，统称为“就绪”的描述符。然后用找到的子集替换这三个引用参数中的对应集合，返回所有就绪描述符的数量。
         int select(
          int nfds,                     // 监控的文件描述符集里最大文件描述符加1
          fd_set *readfds,              // 监控有读数据到达文件描述符集合，引用类型的参数
          fd_set *writefds,             // 监控写数据到达文件描述符集合，引用类型的参数
          fd_set *exceptfds,            // 监控异常发生达文件描述符集合，引用类型的参数
          struct timeval *timeout);     // 定时阻塞监控时间

         int FD_ZERO(int fd, fd_set *fdset);  // 将 fd_set 所有位置 0
         int FD_CLR(int fd, fd_set *fdset);   // 将 fd_set 某一位置 0
         int FD_SET(int fd, fd_set *fd_set);  // 将 fd_set 某一位置 1
         int FD_ISSET(int fd, fd_set *fdset); // 检测 fd_set 某一位是否为 1
      
      ```
3. select实现有以下缺点:
   1. 性能开销大
      1. 调用 select 时会陷入内核，这时需要将参数中的 fd_set 从用户空间拷贝到内核空间，select 执行完后，还需要将 fd_set 从内核空间拷贝回用户空间，高并发场景下这样的拷贝会消耗极大资源；（epoll 优化为不拷贝）
      2. 进程被唤醒后，不知道哪些连接已就绪即收到了数据，需要遍历传递进来的所有 fd_set 的每一位，不管它们是否就绪；（epoll 优化为异步事件通知）
      3. select 只返回就绪文件的个数，具体哪个文件可读还需要遍历；（epoll 优化为只返回就绪的文件描述符，无需做无效的遍历）
   2. 同时能够监听的文件描述符数量太少。受限于 sizeof(fd_set) 的大小，在编译内核时就确定了且无法更改。一般是 32 位操作系统是 1024，64 位是 2048。（poll、epoll 优化为适应链表方式）

### tcp poll server
1. 使用select完成IO多路复用,案例位于tcp_poll_server_example.cpp
2. 和 select 类似，只是描述 fd 集合的方式不同，poll 使用 pollfd 结构而非 select 的 fd_set 结构。管理多个描述符也是进行轮询，根据描述符的状态进行处理，但 poll 无最大文件描述符数量的限制，因其基于链表存储。
      ```
         struct pollfd {
          int fd;           // 要监听的文件描述符
          short events;     // 要监听的事件
          short revents;    // 文件描述符fd上实际发生的事件
         };
      ```
3. select 和 poll 在内部机制方面并没有太大的差异。相比于 select 机制，poll 只是取消了最大监控文件描述符数限制，并没有从根本上解决 select 存在的问题。
### tcp epoll server
1. example中实现了tcp_epoll_server_example;
2. epoll 的特点是：
   1. 使用红黑树存储一份文件描述符集合，每个文件描述符只需在添加时传入一次，无需用户每次都重新传入；—— 解决了 select 中 fd_set 重复拷贝到内核的问题
   2. 通过异步 IO 事件找到就绪的文件描述符，而不是通过轮询的方式；
   3. 使用队列存储就绪的文件描述符，且会按需返回就绪的文件描述符，无须再次遍历；
3. tcp server中: 
   1. 首先是创建套接字，设置地址端口并绑定;监听;这部分流程可以参考[我之前的总结](https://tyler-ytr.github.io/2022/10/19/socket-learning/#%E6%9C%8D%E5%8A%A1%E7%AB%AF)
   2. 然后创建一个epoll;把socket包装成一个epoll_event对象，通过epoll_ctl添加到epoll中;创建回调事件数组;
   3. 在死循环中通过epoll_wait获得响应事件;
      1. 如果该事件的fd为socketfd;说明这是一个新连接;通过accept函数得到接受的fd;如果成功的话设置对应的响应事件然后通过epoll_ctl添加到epoll中;另外通过fcntl把fd设置成非阻塞I/O模式;
      2. 如果不是，说明要么是断开或者连接出错；要么是已有连接；对于断开/出错，通过epoll_ctl删除;对于可读事件，通过read读取然后往客户端写数据;



# 参考
1. [并发测试](https://github.com/wangbojing/c1000k_test/blob/master/client.c)
2. [linux下 C++ 使用 epoll 多路复用 实现高性能的tcpserver](https://juejin.cn/post/6936836371352911902)
3. [微型网络库](https://zhuanlan.zhihu.com/p/597876632)
4. [30天服务器](https://github.com/yuesong-feng/30dayMakeCppServer)
5. [openonload](https://github.com/Xilinx-CNS/onload)
6. [Linux内核之epoll模型](https://github.com/0voice/linux_kernel_wiki/blob/main/%E6%96%87%E7%AB%A0/%E7%BD%91%E7%BB%9C%E5%8D%8F%E8%AE%AE%E6%A0%88/Linux%E5%86%85%E6%A0%B8%E4%B9%8Bepoll%E6%A8%A1%E5%9E%8B.md)
7. [socket客户端服务端](https://blog.csdn.net/abcd552191868/article/details/122398762)
8. [UNIX网络编程——fcntl函数](https://blog.51cto.com/u_15127629/4105853?articleABtest=1)
9. [深入学习IO多路复用 select/poll/epoll 实现原理](https://www.cnblogs.com/88223100/p/Deeply-learn-the-implementation-principle-of-IO-multiplexing-select_poll_epoll.html)
