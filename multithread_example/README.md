# C++多线程
## C++内存模型
### 关系术语
#### sequenced-before
sequenced-before用于表示单线程之间，两个操作上的先后顺序，这个顺序是非对称、可以进行传递的关系。

它不仅仅表示两个操作之间的先后顺序，还表示了操作结果之间的可见性关系。两个操作A和操作B，如果有A sequenced-before B，除了表示操作A的顺序在B之前，还表示了操作A的结果操作B可见。

#### happens-before
与sequenced-before不同的是，happens-before关系表示的不同线程之间的操作先后顺序，同样的也是非对称、可传递的关系。

如果A happens-before B，则A的内存状态将在B操作执行之前就可见。在上一篇文章中，某些情况下一个写操作只是简单的写入内存就返回了，其他核心上的操作不一定能马上见到操作的结果，这样的关系是不满足happens-before的。

#### synchronizes-with
synchronizes-with关系强调的是变量被修改之后的传播关系（propagate），即如果一个线程修改某变量的之后的结果能被其它线程可见，那么就是满足synchronizes-with关系的。

显然，满足synchronizes-with关系的操作一定满足happens-before关系。

### C++ 11 内存模型
1. 支持以下内存模型:
   ````
   enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};
   ````
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