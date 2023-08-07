<!--
 * @Author: tylerytr
 * @Date: 2023-08-07 14:15:47
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-07 17:05:55
 * @FilePath: /CPP_example/multithread_example/multithread_playground/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->
# C++ 多线程样例
实验环境的硬件可以支持32线程。
1. 01_hello_thread: 构造thread对象，输入函数/lambda表达式; thread可以传参;joinable()可以查看一个thread对象是否关联某个线程;可见默认的不关联；thread传参之后，join之前，关联；thread传参之后，join之后，不关联(因为执行完销毁了)
2. 02_thread_manage: 使用sleep_for,join.detach的案例；sleep_thread会睡3秒，loop_thread会循环输出10000次。
   1. PS:在C++中，当主线程退出时，子线程会怎样处理取决于子线程的创建方式和程序的实现。
      1. 子线程被设置为detached（分离）状态： 这意味着子线程在运行时与主线程分离，主线程不再跟踪该线程的状态。当主线程退出时，所有处于分离状态的子线程都将被强制终止，而不管它们是否已经完成(有C++运行时库操作)。
      2. 子线程未被设置为分离状态，但主线程在结束前等待子线程完成：这意味着主线程将保持活动状态，直到子线程完成。这可以通过使用pthread_join（）或std :: thread :: join（）等函数来实现。当主线程退出时，它将等待子线程完成。如果子线程未完成，程序将被挂起。
      3. 子线程未被设置为分离状态，但主线程未等待子线程完成就退出：这种情况下，子线程仍然在运行，但没有与主线程相关联。在这种情况下，可能会发生以下情况之一：子线程继续运行直到完成。子线程被操作系统强制终止。子线程继续运行直到主线程退出，但程序可能不稳定，因为子线程访问的资源可能已经被销毁。
3. 03_call_once: 保证线程只执行一次
4. 04_naive_muptithread: 
   1. 目标：计算某个范围内所有自然数的平方根之和，例如[1, 10e8]。
   2. serial_task：通过worker计算平方根和并且计时(7653 ms)。
   3. concurrent_task：计时(8475 ms)，获得硬件的最大支持线程数；按照该数量划分计算范围然后并行执行。
   4. 会发现concurrent_task的计算结果是错误的，原因是因为多个线程对于同一个数据sum的修改不是原子的。
5. 05_mutex_lock:
   1. 目标：计算某个范围内所有自然数的平方根之和，例如[1, 10e8]。
   2. concurrent_task对于循环中的sum进行加锁，因此执行的速度会比串行慢(241114 ms)，因为同一个时间片只有一个sum执行加和，并且还有线程切换的开销；
   3. improved_task降低了锁的粒度，让sum在每一段计算完自己部分的和之后再修改，因此大大提高了速度(331 ms)。
   4. 可以看到实际上竞争只发生在“汇总”的部分，而划分给自己的数据处理部分实际上是独立的。对于竞争部分加锁可以大大提高性能，也就是要尽可能降低锁的力度
6. 06_dead_lock: 创建了Account类和Bank类；Account类包含了mutex锁mMoneyLock以及名字和钱数；Bank类包含了Account集合，对外暴露transferMoney函数，可以把两个账户的钱进行转账。测试是对两个账户进行随机的转账。
   1. 运行一会之后程序会发生卡死，也就是死锁。另外输出会乱序。
   2. 原因可能是因为一个线程拿到了A锁想拿B锁，另一个线程拿到了B锁想拿A锁，这个时候就会死锁；
   3. 乱序输出问题通过在输出的位置加锁解决；
   4. 这里死锁的问题根源是之前的版本一个线程可能没法保证同时获得两把锁，因此这里通过`lock(*accountA->getLock(), *accountB->getLock());`来通过标准库的实现同时获得两把锁，进而解决了死锁问题。lock_guard中的adopt_lock表示这个mutex已经被lock了，不需要再次lock。

# 编译运行方法
```
mkdir build
cmake ..
make
./01_hello_thread
```

# 参考
1. [C++并发编程](https://paul.pub/cpp-concurrency/)