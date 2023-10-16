<!--
 * @Author: tylerytr
 * @Date: 2023-08-02 09:48:35
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-10-16 16:47:45
 * @FilePath: /stl_playground/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->


# STL容器实现
## function
lambda仿函数原理
1. 基本函数指针使用:test_function_1.cpp;使用函数指针实现闭包功能;
2. lambda仿函数基本原理:test_function_2.cpp;实现了`struct func_printnum_t`;lambda函数相当于一个语法糖，编译器自己实现了一个类似于`struct func_printnum_t`的匿名仿函数;下面的代码来自于cppinsights.io;这个就是该文件中lambda函数在C++17标准的实例化;
     ```cpp
     class __lambda_27_12
          {
          public: 
          inline /*constexpr */ void operator()() const
          {
               std::operator<<(std::operator<<(std::cout, "x: ").operator<<(x), " y: ").operator<<(y).operator<<(std::endl);
          }
          
          private: 
          int & x;
          int & y;
          
          public:
          __lambda_27_12(int & _x, int & _y)
          : x{_x}
          , y{_y}
          {}
          
          };
     ```
3. 实现了自定义的Function与stl库中的类似;
     1. 类型擦除在function和boost::any中有应用，实际上是一种值语意的多态技术，和void*在类似生态位;比如[boost中的any类](https://glemontree.github.io/2017/06/28/[C++]%20C++%E4%B8%ADboostany%E7%9A%84%E4%BD%BF%E7%94%A8/)可以1. 存储任何类型的变量 2. 可以相互拷贝 3. 可以查询所存变量的类型信息 4. 可以转化回原来的类型(any_cast<>);类型擦除可以达到以下两个目的;
          1. 用类型 S 的接口代表一系列类型 T 的的共性。
          2. 如果 s 是 S 类型的变量，那么，任何 T 类型的的对象都可以赋值给s。
     比如，any不用通过`any<someType>x=y`来赋值，而是通过`any x=y`来赋值; function也不用考虑它的类型;
     
## UniquePtr
TOBEDONE     

# 参考
1. [类型擦除相关的博客](https://www.cnblogs.com/muxue/archive/2009/12/10/1621451.html)
2. [STL标准库探索](https://www.zhihu.com/column/c_1637993298085314560)
3. [知乎回答](https://www.zhihu.com/question/359420368/answer/2971536952)