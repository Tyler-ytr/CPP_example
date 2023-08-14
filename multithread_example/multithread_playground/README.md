<!--
 * @Author: tylerytr
 * @Date: 2023-08-07 14:15:47
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 14:26:33
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
7. 07_lock_guard: 
   1. lock_guard的工作原理:在构造函数里边执lock()函数；析构函数里边执行unlock()函数。
   2. 可以通过花括号{}来控制lock_guard的作用范围。
8. 08_unique_lock:
   1. class A中inMsgRecvQueue会把收到的消息存到队列中，outMsgLULProc会在消息队列不为空的时候弹出第一个元素，outMsgRecvQueue会调用outMsgLULProc把数据从消息队列里面弹出。测试程序中有两个线程分别收消息和弹消息。
   2. uniqie_lock 的缺省用法实际上与 lock_quard 一样，可以直接替换。
      ```
      void inMsgRecvQueue() {
      for (int i = 0; i < 100000; ++i) {
         cout << "inMsgRecvQueue exec, push an elem " << i << endl;
         std::unique_lock<std::mutex> m_guard1(m_mutex1);
         msgRecvQueue.push_back(i); /* 假设数字 i 就是收到的玩家命令 */
         }
      }
      ```
   3. unique_lock可以有第二个参数(和lock_guard一样)。
   4. adopt_lock用来标记这个互斥锁已经被 lock() 了，uniqie_lock 不会再重复上锁。
         ```
         /* 把收到的消息（玩家命令）存到队列中 */
        void inMsgRecvQueue() {
          for (int i = 0; i < 100000; ++i) {
            cout << "inMsgRecvQueue exec, push an elem " << i << endl;
            m_mutex1.lock(); /* 先lock()，才能在 unique_lock 中用 adopt_lock 标准 */
            std::unique_lock<std::mutex> m_guard1(m_mutex1, std::adopt_lock);
            msgRecvQueue.push_back(i); /* 假设数字 i 就是收到的玩家命令 */
          }
        }
         ```
   5. try_to_lock代表代码尝试上锁，如果不成功那么会立即返回，不会阻塞;案例中outMsgLULProc会在获得锁之后睡眠20sec，这同时会阻塞另一个线程，这里使用try_to_lock让他提前返回。(虽然造成的效果是push失败了)
         ```
      /* 消息队列不为空时，返回并弹出第一个元素 */
        bool outMsgLULProc(int& command) {
          std::unique_lock<std::mutex> m_guard1(m_mutex1);

          std::chrono::milliseconds dura(20000); /* 20秒 */
          std::this_thread::sleep_for(dura);     /* sleep 20秒 */

          if (!msgRecvQueue.empty()) {
            command = msgRecvQueue.front(); /* 返回第一个元素 */
            msgRecvQueue.pop_front();       /* 移除第一个元素 */
            return true;
          }
          return false;
        }
      /* 把收到的消息（玩家命令）存到队列中 */
        void inMsgRecvQueue() {
          for (int i = 0; i < 100000; ++i) {
            cout << "inMsgRecvQueue exec, push an elem " << i << endl;
            /* 尝试上锁 */
            std::unique_lock<std::mutex> m_guard1(m_mutex1, std::try_to_lock);
            if (m_guard1.owns_lock()) {  /* 如果拿到了锁 */
              msgRecvQueue.push_back(i); /* 假设数字 i 就是收到的玩家命令 */
            } else {
              cout << "try_to_lock fail, do something else!!!" << endl;
            }
          }
        }   
         ```
   6. defer_lock表示并没有给 mutex 加锁，即初始化了一个没有加锁的 mutex。使用该标记初始化的 uniqie_lock 对象可以灵活的调用 uniqie_lock 的成员函数
   7. unique_lock的成员函数包括 lock()，unlock(),try_lock(),release()
      1. unique_lock使用lock()的时候会在该对象析构的时候自动解锁；也可以手动unlock()来降低锁的粒度
      2. try_lock()与 try_to_lock相似,尝试上锁，如果拿到锁了，则返回 true，否则返回 false。
         ```
         /* 把收到的消息（玩家命令）存到队列中 */
           void inMsgRecvQueue() {
             for (int i = 0; i < 100000; ++i) {
               cout << "inMsgRecvQueue exec, push an elem " << i << endl;
               /* 初始化了没有加锁的m_mutex1 */
               std::unique_lock<std::mutex> m_guard1(m_mutex1, std::defer_lock);
               if (m_guard1.try_lock()) {   /* 如果拿到锁了 */
                 msgRecvQueue.push_back(i); /* 假设数字 i 就是收到的玩家命令 */
               } else {
                 cout << "try_to_lock fail, do something else!!!" << endl;
               }
             }
           }
         ```
      3. release()作用是返回它所管理的 mutex 对象指针，并释放所有权；也就是说，这个 unique_lock 和 mutex 不再有关系。 调用 release() 函数解绑后，我们必须保存返回的 mutex 指针，并在接下来的代码中自行管理。
            ```
               /* 把收到的消息（玩家命令）存到队列中 */
                 void inMsgRecvQueue() {
                   for (int i = 0; i < 100000; ++i) {
                     cout << "inMsgRecvQueue exec, push an elem " << i << endl;
                     /* 初始化了没有加锁的m_mutex1 */
                     std::unique_lock<std::mutex> m_guard1(m_mutex1);
                     std::mutex* pmutex = m_guard1.release(); /* 解绑后返回之前绑定的m_mutex1，接下来我们要自行管理m_mutex1 */
                     msgRecvQueue.push_back(i); /* 假设数字 i 就是收到的玩家命令 */
                     pmutex->unlock();          /* 解绑后需要自己解锁 */
                   }
                 }
            ```
    8. unique_lock()可以转移所有权但是不能复制所有权。
      ```
      //通过移动语义转移所有权
      std::unique_lock<std::mutex> auto_mutex_1(my_mutex1_);
      // std::unique_lock<std::mutex> auto_mutex_1(auto_mutex_1); // 直接复制，编译报错
      std::unique_lock<std::mutex> auto_mutex_2(std::move(auto_mutex_1)); // 左值转右值，调用移动构造函数
      //通过成员函数返回临时对象
      std::unique_lock<std::mutex> rtn_unique_lock()
         {
         	std::unique_lock<std::mutex> tmpguard(my_mutex1_);
         	return tmpguard;
         }
         // 从一个临时对象来构造新的Foo的话，编译器会优先调用搬移构造函数，来把临时对象开膛破肚，取出自己需要的东西。
         // 这里本质也是用了移动语义
         std::unique_lock<std::mutex> auto_mutex_2 = rtn_unique_lock()
    
      ```
8. 09_unique_lock:
   1. 第一部分以异步的形式启动任务，存在f1这个future对象里面；在wait之后执行完成并输出。需要注意的是async是启动一个新的线程，还是以同步的方式（不启动新的线程）运行任务是标准没有指定的，与编译器相关，可以通过launch::async明确说明(async：运行新线程，以异步执行任务；deferred：调用方线程上第一次请求其结果时才执行任务，即惰性求值。)
   2. 第二部分使用lambda表达式替代了函数
   3. 第三部分使用了对象封装。
9. 10_packaged_task:
   1.  packaged_task对象是一个可调用对象，它可以被封装成一个std::fucntion，或者作为线程函数传递给std::thread，或者直接调用。
10. 11_future_promise:
    1.  future对象只有被一个线程获取值。并且在调用get()之后，就没有可以获取的值了。如果从多个线程调用get()会出现数据竞争，其结果是未定义的。
# 多线程使用注意点
1. thread_test_1中测试了detach使用中的线程传参;有两个测试；第一个测试是不安全的线程传参，因为指针指向的地址可能会提前释放；第二个测试是较安全的，建议使用显示类型转换；
2. thread_test_2中使用get_ID()观察线程创建，传参的过程；参数拷贝次数与编译器有关；目前的g++ 9.4.0版本表现如下;
  测试2传递临时对象可以看到会发生两次拷贝构造。
    ```
   主线程的ID是：140203331639104
   构造函数被执行0x7ffc1eb91a68创建此对象的ID是：140203331639104
   Copy构造函数被执行0x7ffc1eb91a20执行copy操作的线程ID是：140203331639104
   Copy构造函数被执行0x5621a30112c8执行copy操作的线程ID是：140203331639104
   析构函数被执行0x7ffc1eb91a20执行析构的线程ID是：140203331639104
   析构函数被执行0x7ffc1eb91a68执行析构的线程ID是：140203331639104
   子线程的ID是：140203331634944
   析构函数被执行0x5621a30112c8执行析构的线程ID是：140203331634944
    ```
  如果使用ref参数(测试3)那就不会copy出来一份了，也就是实现了引用的真实含义。
  测试4中是调用类中成员函数的方式，这里需要传递对象进去，可以看到是值传递，这是为了避免detach可能的问题。

# 编译运行方法
```
mkdir build
cmake ..
make
./01_hello_thread
```

# 参考
1. [C++并发编程](https://paul.pub/cpp-concurrency/)
2. [C++多线程专栏](https://gutsgwh1997.github.io/categories/C/C-%E5%A4%9A%E7%BA%BF%E7%A8%8B/)