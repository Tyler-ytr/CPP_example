# CPP 服务器
用于输出socket以及epoll的积累
## epoll使用
1. epoll就是一种典型的I/O多路复用技术: epoll技术的最大特点是支持高并发。
   1. 传统多路复用技术select，poll，在并发量达到1000-2000，性能就会明显下降
### epoll
[epoll底层源码解析](https://www.cnblogs.com/mysky007/p/12284842.html)
1. [eventpoll结构体](https://www.cnblogs.com/mysky007/p/12284842.html)中包含了:
   1. 自旋锁 lock: 用于保护对于这个结构体的访问
   2. mutex用来保证epoll使用文件的时候文件不会被删除
   3. wait_queue_head_t wq; 这个队列里存放的是执行epoll_wait从而等待的进程队列
   4. wait_queue_head_t poll_wait; 这个队列里存放的是该eventloop作为poll对象的一个实例，加入到等待的队列
   5. struct list_head **rdllist**; 双链表中则存放着将要通过epoll_wait返回给用户的满足条件的事件
   6. struct rb_root_cached **rbr**; 红黑树的根节点，这颗树中存储着所有添加到epoll中的需要监控的事件
   7. struct epitem *ovflist; 
   8. struct wakeup_source *ws; wakeup_source used when ep_scan_ready_list is running
   9. struct user_struct *user; The user that created the eventpoll descriptor
   10. struct file *file 这是eventloop对应的匿名文件
   11. int visited; used to optimize loop detection check 
   12. struct list_head visited_list_link;
   13. unsigned int napi_id;（#ifdef CONFIG_NET_RX_BUSY_POLL） /* used to track busy poll napi_id */
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


## CPP tcp server 简化版本
### 


# 参考
1. [并发测试](https://github.com/wangbojing/c1000k_test/blob/master/client.c)
2. [linux下 C++ 使用 epoll 多路复用 实现高性能的tcpserver](https://juejin.cn/post/6936836371352911902)
3. [微型网络库](https://zhuanlan.zhihu.com/p/597876632)
4. [30天服务器](https://github.com/yuesong-feng/30dayMakeCppServer)
5. [openonload](https://github.com/Xilinx-CNS/onload)
6. [Linux内核之epoll模型](https://github.com/0voice/linux_kernel_wiki/blob/main/%E6%96%87%E7%AB%A0/%E7%BD%91%E7%BB%9C%E5%8D%8F%E8%AE%AE%E6%A0%88/Linux%E5%86%85%E6%A0%B8%E4%B9%8Bepoll%E6%A8%A1%E5%9E%8B.md)
