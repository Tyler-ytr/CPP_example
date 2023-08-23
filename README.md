<!--
 * @Author: tylerytr
 * @Date: 2023-03-27 14:16:52
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-20 22:34:57
 * @FilePath: /CPP_example/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->
# CPP_example
1. makefile_example makefile的简要例子
2. skip_list_example 跳表,cmake编译；
3. multithread_example 
   1. mempool 实现
   2. multithread_playground C++ thread 一些例子
4. c11_playground
   1. 虚函数表分析
   2. 符号表分析
5. cpp_server_example
   1. IO多路复用，多种基本TCP server实现(socket,select,poll,epoll);
## 附录
### 编译器与C++标准
使用特定的特性就需要对应版本的编译器。版本的支持可以参考[C++ Standards Support in GCC](https://www.gnu.org/software/gcc/projects/cxx-status.html)和[C++ Support in Clang](https://clang.llvm.org/cxx_status.html)
另外可以利用-std=c++xx告诉编译器需要啥版本;比如:
```
g++ -std=c++17 your_file.cpp -o your_program
```
