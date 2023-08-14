<!--
 * @Author: tylerytr
 * @Date: 2023-04-07 15:57:31
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 17:36:08
 * @FilePath: /CPP_example/multithread_example/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->
# C++多线程
# C++内存模型
## 关系术语
### sequenced-before
sequenced-before用于表示单线程之间，两个操作上的先后顺序，这个顺序是非对称、可以进行传递的关系。

它不仅仅表示两个操作之间的先后顺序，还表示了操作结果之间的可见性关系。两个操作A和操作B，如果有A sequenced-before B，除了表示操作A的顺序在B之前，还表示了操作A的结果操作B可见。

### happens-before
与sequenced-before不同的是，happens-before关系表示的不同线程之间的操作先后顺序，同样的也是非对称、可传递的关系。

如果A happens-before B，则A的内存状态将在B操作执行之前就可见。在上一篇文章中，某些情况下一个写操作只是简单的写入内存就返回了，其他核心上的操作不一定能马上见到操作的结果，这样的关系是不满足happens-before的。

### synchronizes-with
synchronizes-with关系强调的是变量被修改之后的传播关系（propagate），即如果一个线程修改某变量的之后的结果能被其它线程可见，那么就是满足synchronizes-with关系的。

显然，满足synchronizes-with关系的操作一定满足happens-before关系。

## C++ 11 内存模型
1. 支持以下内存模型:
```
   enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};
```
2. 上述一致性模型可以分为四类，从一致性约束强到一致性约束弱的顺序为:
    1. 顺序一致性:memory_order_seq_cst
    2. Acquire-Release:memory_order_acquire, memory_order_release, memory_order_acq_rel
    3. Release-Consume:memory_order_consume
    4. Relaxed:memory_order_relaxed
3. 顺序一致性:
    - 每个处理器的执行顺序和代码中的顺序（program order）一样。
    - 所有处理器都只能看到一个单一的操作执行顺序。
4. Acquire-Release: 假设store()使用memory_order_release，而load()使用memory_order_acquire。这种模型有两种效果：
    - 第一种是可以限制 CPU 指令的重排：在store()之前的所有读写操作，不允许被移动到这个store()的后面；在load()之后的所有读写操作，不允许被移动到这个load()的前面。
    - 假设 Thread-1 store()的那个值，成功被 Thread-2 load()到了，那么 Thread-1 在store()之前对内存的所有写入操作，此时对 Thread-2 来说，都是可见的。
5. Release-Consume:
    - carry-a-dependency: 
        ````
        b = *a;
        c = *b;
        ````
        其中第二行代码的执行结果依赖于第一行代码的执行结果，此时称这两行代码之间的关系为“carry-a-dependency ”。
    - C++中引入的memory_order_consume内存模型就针对这类代码间有明确的依赖关系的语句限制其先后顺序。相当于对于有依赖关系的部分是有先后顺序的(如同acquire)，但是对于没有依赖关系的部分是不保证先后顺序的。
6. Relaxed:允许乱序，只保证操作原子性；



# C++多线程使用
## C++多线程相关头文件
1. C++11 新标准中引入了四个头文件来支持多线程编程，他们分别是`<atomic> ,<thread>,<mutex>,<condition_variable>,<future>`
   1. `<atomic>`：该头文主要声明了两个类, std::atomic 和 std::atomic_flag，另外还声明了一套 C 风格的原子类型和与 C 兼容的原子操作的函数。
   2. `<thread>`：该头文件主要声明了 std::thread 类，另外 std::this_thread 命名空间也在该头文件中。
   3. `<mutex>`：该头文件主要声明了与互斥量(mutex)相关的类，包括 std::mutex 系列类，std::lock_guard, std::unique_lock, 以及其他的类型和函数。C++14/17更新了shared_timed_mutex/shared_mutex;
   4. `<condition_variable>`：该头文件主要声明了与条件变量相关的类，包括 std::condition_variable 和 std::condition_variable_any。
   5. `<future>`：该头文件主要声明了 std::promise, std::package_task 两个 Provider 类，以及 std::future 和 std::shared_future 两个 Future 类，另外还有一些与之相关的类型和函数，std::async() 函数就声明在此头文件中。


## Thread库
下文所述，如无另外标记，默认为C++11特性
1. thread: 创建线程，可以传入(带参)函数，lambda表达式
### join与detach
1. thread类调用join()函数后，原始线程会等待新线程执行完毕之后再去销毁线程对象。
   thread::join()还会在进程结束后清理子线程相关的内存空间，此后该thread对象不再与该子线程相关，即thread::joinable() = false ,故join对一个线程而言只能调用一次
2. thread::detach将线程从thread对象分离出来，运行线程独立执行。当主线程结束时，由运行时库负责清理与子线程相关的资源。通常称分离线程为守护线程(daemon threads),UNIX中守护线程是指，没有任何显式的用户接口，并在后台运行的线程。(经过测试，当main()结束的时候，派生的子线程也不在运行)
   由于detach不像join一样会等待子线程与主线程汇聚，故要注意以下几点:
   1. 如果传递int这种简单类型，推荐使用值传递，不要用引用
   2. 如果传递类对象，避免使用隐式类型转换，全部都在创建线程这一行就创建出临时对象，然后在函数参数里，用引用来接，否则还会创建出一个对象。
### 线程管理
1. yield：让出处理器，重新调度各执行线程;通常用在自己的主要任务已经完成的时候，此时希望让出处理器给其他任务使用。
2. get_id: 返回当前线程的线程 id;可以以此来标识不同的线程。
3. sleep_for: 是让当前线程停止一段时间。
4. sleep_until: 使当前线程的执行停止直到指定的时间点

### 一次调用
1. call_once：即便在多线程环境下，也能保证只调用某个函数一次
2. once_flag：与call_once配合使用

### 竞争条件和临界区
当多个进程或者线程同时访问共享数据时，只要有一个任务会修改数据，那么就可能会发生问题。此时结果依赖于这些任务执行的相对时间，这种场景称为**竞争条件**（race condition）。

访问共享数据的代码片段称之为**临界区**（critical section）。比如04_native_multithread中的sum变量。要避免竞争条件，就需要对临界区进行数据保护。

## mutex库
### mutex
mutex是mutual exclusion（互斥）的简写。

mutex有如下类型：
1. mutex：提供基本互斥设施
2. timed_mutex：提供互斥设施，带有超时功能
3. recursive_mutex：提供能被同一线程递归锁定的互斥设施
4. recursive_timed_mutex：提供能被同一线程递归锁定的互斥设施，带有超时功能
5. shared_timed_mutex(C++14)：提供共享互斥设施并带有超时功能
6. shared_mutex(C++17)：提供共享互斥设施

这些类型都提供如下方法：
1. lock：锁定互斥体，如果不可用，则阻塞
2. try_lock：尝试锁定互斥体，如果不可用，直接返回
3. unlock：解锁互斥体
   
在这些基础功能上，其他的类进行了如下三方面扩展：
1. **超时**：`timed_mutex，recursive_timed_mutex，shared_timed_mutex`的名称都带有timed，这意味着它们都支持超时功能。它们都提供了`try_lock_for`和`try_lock_until`方法，这两个方法分别可以指定超时的时间长度和时间点。如果在超时的时间范围内没有能获取到锁，则直接返回，不再继续等待。
2. **可重入**：`recursive_mutex`和`recursive_timed_mutex`的名称都带有recursive。可重入或者叫做可递归，是指在同一个线程中，同一把锁可以锁定多次。这就避免了一些不必要的死锁。
3. **共享**：`shared_timed_mutex`和`shared_mutex`提供了共享功能。对于这类互斥体，实际上是提供了两把锁：一把是共享锁，一把是互斥锁。一旦某个线程获取了互斥锁，任何其他线程都无法再获取互斥锁和共享锁；但是如果有某个线程获取到了共享锁，其他线程无法再获取到互斥锁，但是还有获取到共享锁。共享锁通常用在读者写者模型上。 共享锁有如下方法：
   1. lock_shared：获取互斥体的共享锁，如果无法获取则阻塞
   2. try_lock_shared：尝试获取共享锁，如果不可用，直接返回
   3. unlock_shared：解锁共享锁

### 通用互斥管理
`<mutex>`中提供了以下类来提供通用互斥管理：
1. lock_guard: 实现严格基于作用域的互斥体所有权包装器;
2. unique_lock: 实现可移动的互斥体所有权包装器
3. shared_lock(C++14): 实现可移动的共享互斥体所有权封装器
4. scoped_lock(C++17)：用于多个互斥体的免死锁 RAII 封装器

这些类有以下锁定策略：
1. defer_lock：类型为 `defer_lock_t`，不获得互斥的所有权
2. try_to_lock：类型为`try_to_lock_t`，尝试获得互斥的所有权而不阻塞
3. adopt_lock：类型为`adopt_lock_t`，假设调用方已拥有互斥的所有权

通用互斥管理解决的问题是手动的lock和unlock带来的繁琐和易错。标准库基于RAII思想提供了以上API；

### RAII
RAII全称是Resource Acquisition Is Initialization，直译过来就是：资源获取即初始化。

RAII会把对象使用前请求的资源的生命周期和对象本身的生命周期绑定。它保证对象存在的时候里面的资源能被访问，并且当对象生命周期结束的时候，资源会按照获取顺序逆序释放。如果某一个资源获取失败，那么以获取的资源也会逆序释放。

RAII 可总结如下:
1. 将每个资源封装入一个类，其中：
   1. 构造函数请求资源，并建立所有类不变式，或在它无法完成时抛出异常，
   2. 析构函数释放资源并决不抛出异常；
2. 始终经由 RAII 类的实例使用满足要求的资源，该资源
   1. 自身拥有自动存储期或临时生存期，或
   2. 具有与自动或临时对象的生存期绑定的生存期

## condition_variable 库
`<condition_variable>`库提供了以下环境变量：
1. condition_variable：提供与 std::unique_lock 关联的条件变量
2. condition_variable_any：提供与任何锁类型关联的条件变量
3. notify_all_at_thread_exit：安排到在此线程完全结束时对 notify_all 的调用
4. cv_status：列出条件变量上定时等待的可能结果

condition_variable有以下公共接口：
1. wait 阻塞
2. wait_for 阻塞特定时间
3. wait_until 阻塞到某一个时间点
4. notify_one 放行一个线程，如果此时托管了多个线程，则随机抽取。
5. notify_all 放行所有线程。
6. native_handle 返回原生句柄。

## future 库
`<future>`库提供了以下API：
1. async：异步运行一个函数，并返回保有其结果的std::future
2. future：等待被异步设置的值
3. packaged_task：打包一个函数，存储其返回值以进行异步获取；(packaged_task绑定到一个函数或者可调用对象上。当它被调用时，它就会调用其绑定的函数或者可调用对象。并且，可以通过与之相关联的future来获取任务的结果。调度程序只需要处理packaged_task，而非各个函数。)
4. promise：存储一个值以进行异步获取
5. shared_future：等待被异步设置的值（可能为其他 future 所引用）


## 使用注意点
### 线程传参
1. 线程传参是如果是用detach那么需要考虑主线程先结束，子线程还没有结束的情况；此时传参不建议使用引用，**不可以使用指针**(因为指针传过去的地址会被回收)； 建议使用创建线程的同时构造临时对象的方法传递参数(他可以保证在主线程结束前将myprint()的第二个参数构造出来，隐式类型转换再detach()则不能保证);总而言之，对于detach的thread:
   1. 若果传递内置类型的变量，建议都是按照值传递，不推荐使用引用，坚决不能用指针
   2. 如果传递类对象，避免使用隐式类型转换。全部都在创建线程这一行创建出临时对象来，并且在函数参数中使用**引用**来接收参数。
   3. 除非万不得已！是使用join()就不会出现局部变量失效导致线程对内存非法访问的情形。
2. 可以使用get_ID()对构造过程进行追踪，根据thread_test_2的结果：
   1. 当使用隐式类型转换时，临时对象是在**子线程**中被构建的，这就会导致detach()出现问题。（主线程执行完，释放了临时变量可能导致子线程无法构造）
   2. 当创建线程使用临时对象时，临时对象是在主线程中被构建出来的，这样确保使用子线程不会出问题。
### shared_ptr 线程安全
1. 线程安全的定义：
   1. 多个线程同时访问的时候，表现出正确的行为；
   2. 无论操作系统如何调度这些线程，无论这些线程的执行顺序如何交织
   3. 调用端代码无须额外的同步或者其他协调动作。
2. `shared_ptr`有两部分组成：引用计数(指向控制块的指针)和原始指针。`shared_ptr`的引用计数本身是安全并且无锁的；多线程环境下，调用不同`shared_ptr`实例的成员函数是不需要额外的同步手段的；**但是**，智能指针的另一个成员，原始指针不是原子的。因此shared_ptr不是线程安全的。
3. 多线程同时读同一个`shared_ptr`对象是线程安全的，但是如果是多个线程对同一个`shared_ptr`对象进行读和写，则需要加锁。[文档](https://www.boost.org/doc/libs/1_39_0/libs/smart_ptr/shared_ptr.htm#ThreadSafety)案例如下：
   1. Example 0: 引用计数改变 原子操作 安全
      ```
      shared_ptr<int> p(new int(42));
      ```
   2. Example 1: 两个线程读 安全
      ```
      // thread A
      shared_ptr<int> p2(p); // reads p

      // thread B
      shared_ptr<int> p3(p); // OK, multiple reads are safe
      ```
   3. Example 2: 两个线程写不同的`shared_ptr` 安全(reset()包含两个操作。当智能指针中有值的时候，调用reset()会使引用计数减1.当调用reset(new xxx())重新赋值时，智能指针首先是生成新对象，然后将就对象的引用计数减1（当然，如果发现引用计数为0时，则析构旧对象），然后将新对象的指针交给智能指针保管)
      ```
      // thread A
      p.reset(new int(1912)); // writes p

      // thread B
      p2.reset(); // OK, writes p2
      ```
   4. Example 3: 两个线程读写一个`shared_ptr` 不安全：
      ```
      // thread A
      p = p3; // reads p3, writes p

      // thread B
      p3.reset(); // writes p3; undefined, simultaneous read/write
      ```
      y=x操作涉及到两个成员的操作，也就是`shared_ptr`中裸指针的赋值和引用计数的变化，如果没有mutex那么多线程就有race condition
   5. Example 4: 两个线程读/销毁一个`shared_ptr` 不安全；
      ```
      // thread A
      p3 = p2; // reads p2, writes p3

      // thread B
      // p2 goes out of scope: undefined, the destructor is considered a "write access"

      ```
   6. Example 5: 两个线程同时写一个`shared_ptr` 不安全
      ```
      // thread A
      p3.reset(new int(1));

      // thread B
      p3.reset(new int(2)); // undefined, multiple writes
      
      ```
4. 多线程无保护写可能会出现race condition:
   1. 以下场景: 有 3 个 `shared_ptr<Foo>`对象 x、g、n：
      ```
      shared_ptr<Foo> g(new Foo1); // 线程之间共享的 shared_ptr
      shared_ptr<Foo> x; // 线程 A 的局部变量
      shared_ptr<Foo> n(new Foo2); // 线程 B 的局部变量
      -------------------------------------------
      线程 A
      x = g; （即 read g） //代码1 :赋值指针,赋值 引用计数
      -------------------------------------------
      线程 B
      g = n;//代码2 :动作A 清空原来G指向Foo1, 动作B 然后重新赋值 Foo2

      测试场景：

      线程A 
        智能指针x 读取Foo1,然后还重置Foo1计数。

      线程 B:
       销毁了Foo1
      线程A
      重置计数是，foo1已经被销毁。
      
      ```
      见[thread_test_4.cpp](https://github.com/Tyler-ytr/CPP_example/blob/main/multithread_example/multithread_playground/example/thread_test_4.cpp) 执行之后会概率性的segmentation fault;
      > 首先 shared_ptr赋值 a=b有三步，一个步骤是修改裸指针指向(1)，一个步骤是a中指向的控制块的引用计数-1(2)，一个步骤是a根据b指向的控制块赋值自己的控制块指针，并把对应的引用计数+1(3); 下面的控制块计数前面的表示Foo1,后面的表示Foo2;
      
      > 阶段1:x(null,null);g(Foo1,1);n(Foo2,1);控制块引用计数为1,1
      
      > 阶段2:x(**Foo1**,null);g(Foo1,1);n(Foo2,1);控制块引用计数为1,1 //x = g 的指针赋值部分(1)
      
      > 阶段3:x(Foo1,null);g(**Foo2**,1);n(Foo2,1);控制块引用计数为1,1 //g = n 的指针赋值部分(1)
      
      > 阶段4:x(Foo1,null);g(Foo2,**2**);n(Foo2,**2**);控制块引用计数为**0,2** //g = n 引用计数修改部分(2);导致了Foo1的引用计数为0，此时Foo1会被销毁,x.ptr变成空悬指针;
      
      > 阶段5:x(Foo1,**3**);g(Foo2,**3**);n(Foo2,**3**);控制块引用计数为**0,2** // x = g 的引用计数修改部分(3); 

      原理可以参考[该博客](https://cloud.tencent.com/developer/article/1654442);




### 互斥锁自旋锁的应用场景

## 多线程基本模型
### 生产者消费者模型


### 读者写者问题

### 银行家算法

### 哲学家就餐问题

# 参考资料
1. [C++并发编程](https://paul.pub/cpp-concurrency/)
2. [现代C++教程/并行并发](https://changkun.de/modern-cpp/zh-cn/07-thread/index.html)
3. [C++多线程专栏](https://gutsgwh1997.github.io/categories/C/C-%E5%A4%9A%E7%BA%BF%E7%A8%8B/)

