<!--
 * @Author: tylerytr
 * @Date: 2023-08-02 09:48:35
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-02 11:08:33
 * @FilePath: /CPP_example/c11_playground/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->


# C11特性测试
1. 虚函数表(见Virtual_Table1.cpp,Virtual_Table2.cpp);参考[博客1](https://www.cnblogs.com/hamwj1991/p/12907683.html)和[博客2](https://www.cnblogs.com/Mered1th/p/10924545.html)
   1. 编译命令:`g++ -fdump-lang-class Virtual_Table1.cpp` (g++ version >g++ 7)
   2. 补充:`-fdump-lang-class`可以输出类的相关信息,可以看到虚函数表;`-fdump-lang-raw`会生成一个包含源代码的中间表示文件
   3. Virtual_Table1.cpp用来演示使用g++显示虚函数表;Virtual_Table2.cpp用来演示行为
   4. Base_1,Devired_1表示单继承的情况；首先每一个class都有一个虚函数表，继承的类的虚函数表中，父类的虚函数在子类的上面，如果有override的情况，子类的虚函数会覆盖父类的虚函数。
   5. Base_2_1,Base_2_2,Devired_2表示多继承的情况；每一个class都有一个虚函数表；对于继承的子类，它的虚函数表分为两个部分，一部分来自于Base_2_1，一部分来自于Base_2_2；
      1. 对于f的override会导致前一部分的虚函数表中的f变成Deirved2::f，后半部分变成Devired_2::_ZThn8_N9Devired_21fEv；调用f的表现为覆盖之后的子类函数，输出"df";
      2. g函数在 Base_2_1,Base_2_2中都存在，Devired_2的对象直接调用g是过不了编译的。
   6. Super_3,Base_3_1,Base_3_2,Devired_3表示菱形继承的情况;
      1. 虚继承会引入VTT概念；表示的是virtual table table，虚函数表的表，里面存的是虚表的入口地址;在Base_3_1,Base_3_2,Devired_3中出现。
      2. Devired_3中会额外出现VTT以及两个Construction vtable;Construction vtable里有Devired_3继承的Base_3_1,Base_3_2的虚表信息；经过比对基本一致；里面包含了Base overide的函数以及他们继承的Super的函数；
      3. Devired_3中overide的函数会出现在它的virtual table中，比如f,h1,g,h2;
2. To be done 