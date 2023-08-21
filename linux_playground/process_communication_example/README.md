<!--
 * @Author: tylerytr
 * @Date: 2023-08-02 09:48:35
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-21 14:26:13
 * @FilePath: /CPP_example/linux_playground/process_communication_example/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->

# 进程通信
1. 管道pipe
2. 命名管道FIFO
3. 消息队列MessageQueue
4. 共享存储SharedMemory
5. 信号量Semaphore
6. 套接字Socket
7. 信号 Signal

## 管道
1. 管道又称为匿名管道，是一种基本的IPC机制，通过pipe创建:
   ```
    #include <unistd.h>
    int _pipe[2] = {0, 0};
    int ret = pipe(_pipe);  // 创建无名管道,参数返回写和读文件操作符;ret返回值：成功返回0，失败返回-1；
   ```
2. 调用pipe函数时在内核中开辟一块缓冲区用于通信,它有一个读端，一个写端：_pipe[0]指向管道的读端，_pipe[1]指向管道的写端。所以管道在用户程序看起来就像一个打开的文件,通过read(_pipe[0])或者write(_pipe[1])向这个文件读写数据，其实是在读写内核缓冲区。
3. 管道是通过环形队列实现的，数据从写端流入从读端流出。
4. 使用管道的通信过程
   1. 当使用`pipe(_pipe);`开辟管道之后；父进程的_pipe里的两个文件描述符就会分别指向管道的读端写端；
   2. 父进程调用`fork`创建子进程，子进程的_pipe里面的两个文件描述符也会指向管道的读端和写端
   3. 父进程关闭写，子进程关闭读；子进程可以通过`write`往管道里面写，父进程可以通过`read`读取管道的数据
5. [使用管道的特殊情况](https://blog.csdn.net/ljy_home/article/details/51702036):
   1. 所有指向写端的文件描述符都关闭了，而仍有进程从管道的读端读数据，那么管道中剩余的数据都被读取后，再次read会返回0，就像读到文件末尾一样
   2. 如果有指向管道写端的文件描述符没关闭，而持有管道写端的进程也没有向管道中写数据，这时有进程从管道读端读数据，那么管道中剩余的数据都被读取后，再次read会阻塞，直到管道中有数据可读了才读取数据并返回。
   3. 如果所有指向管道读端的文件描述符都关闭了，这时有进程向管道的写端write，那么该进程会收到信号SIGPIPE，通常会导致进程异常终止。
   4. 如果有指向管道读端的文件描述符没关闭，而持有管道读端的进程也没有从管道中读数据，这时有进程向管道写端写数据，那么管道被写满时，再次write会阻塞，直到管道中有空位置了才写入数据并返回。


# 参考
1. https://blog.yanjingang.com/?p=4503