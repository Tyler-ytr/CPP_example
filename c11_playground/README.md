<!--
 * @Author: tylerytr
 * @Date: 2023-08-02 09:48:35
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-15 14:55:20
 * @FilePath: /CPP_example/c11_playground/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->


# C11特性测试
1. 虚函数表(见Virtual_Table1.cpp,Virtual_Table2.cpp);参考[博客1](https://www.cnblogs.com/hamwj1991/p/12907683.html)和[博客2](https://www.cnblogs.com/Mered1th/p/10924545.html)
   1. 编译命令:`g++ -fdump-lang-class Virtual_Table1.cpp` (g++ version >g++ 7)
   2. 补充:`-fdump-lang-class`可以输出类的相关信息,可以看到虚函数表;`-fdump-lang-raw`会生成一个包含源代码的中间表示文件
   3. Virtual_Table1.cpp用来演示使用g++显示虚函数表;Virtual_Table2.cpp用来演示行为；
   4. Base_1,Devired_1表示单继承的情况；首先每一个class都有一个虚函数表，继承的类的虚函数表中，父类的虚函数在子类的上面，如果有override的情况，子类的虚函数会覆盖父类的虚函数。
   5. Base_2_1,Base_2_2,Devired_2表示多继承的情况；每一个class都有一个虚函数表；对于继承的子类，它的虚函数表分为两个部分，一部分来自于Base_2_1，一部分来自于Base_2_2；
      1. 对于f的override会导致前一部分的虚函数表中的f变成Deirved2::f，后半部分变成Devired_2::_ZThn8_N9Devired_21fEv；调用f的表现为覆盖之后的子类函数，输出"df";
      2. g函数在 Base_2_1,Base_2_2中都存在，Devired_2的对象直接调用g是过不了编译的。
   6. Super_3,Base_3_1,Base_3_2,Devired_3表示菱形继承的情况;
      1. 虚继承会引入VTT概念；表示的是virtual table table，虚函数表的表，里面存的是虚表的入口地址;在Base_3_1,Base_3_2,Devired_3中出现。
      2. Devired_3中会额外出现VTT以及两个Construction vtable;Construction vtable里有Devired_3继承的Base_3_1,Base_3_2的虚表信息；经过比对基本一致；里面包含了Base overide的函数以及他们继承的Super的函数；
      3. Devired_3的virtual table中有f,h1,g,h2,h各一份；顺序是先Base_3_1然后Base_3_2然后Super；
      4. 调用顺序：参考该[知乎文章](https://zhuanlan.zhihu.com/p/268324735)；首先Derived对内存初始化，这部分内存存放Super和base的内容，称为内存A；内存A偏移特定字节的位置调用super::super()初始化，然后这里会存放super的虚指针(super_vptr);读取VTT得到Construction vtable位置，实际上是拿到指向Base_3_1虚表里面的第一个虚函数的地址的指针，下面称为 Base1_vptr;调用Base1::Base1() 对内存A进行初始化；在 Base1_vptr - 24，即获取 Base1 虚表的 virtual-base offset，让 Base1_vptr 加上这个 offset 得到 Super 的虚指针;然后根据super_vptr再次更新这块内存；然后进入Base2部分，同理。
      5. 内存布局可以参考该[CSDN](https://blog.csdn.net/qq_41431406/article/details/84933450);可以看到虚继承多了vbptr;Deirved中的每一个Base都有一个vbptr指向Super
        
   7. 总结：
      1. 单继承
         1. 虚表中派生类覆盖的虚函数的地址被放在了基类相应的函数原来的位置
         2. 派生类没有覆盖的虚函数就延用基类的。同时，虚函数按照其声明顺序放于表中，父类的虚函数在子类的虚函数前面。
      2. 多继承
         1. 每个基类都有自己的虚函数表
         2. 派生类的虚函数地址存依照声明顺序放在第一个基类的虚表最后
      3. 虚继承 如果没有虚继承，菱形继承会有两份Super实例；虚继承可以控制只有一份。
      4. 当我们直接通过父类指针调用子类中的未覆盖父类的成员函数，编译器会报错
2. 符号表;参考了该[博客](https://csstormq.github.io/blog/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%B3%BB%E7%BB%9F%E7%AF%87%E4%B9%8B%E9%93%BE%E6%8E%A5%EF%BC%8810%EF%BC%89%EF%BC%9A.bss%E3%80%81.data%20%E5%92%8C%20.rodata%20section%20%E4%B9%8B%E9%97%B4%E7%9A%84%E5%8C%BA%E5%88%AB)
   1. 使用`g++ -c Symbol_table.cpp -o Symbol_table.o `可以编译；然后使用`readelf -S Symbol_table.o`查看elf信息;
      ```
      There are 13 section headers, starting at offset 0x398:

      Section Headers:
        [Nr] Name              Type             Address           Offset
             Size              EntSize          Flags  Link  Info  Align
        [ 0]                   NULL             0000000000000000  00000000
             0000000000000000  0000000000000000           0     0     0
        [ 1] .text             PROGBITS         0000000000000000  00000040
             000000000000000f  0000000000000000  AX       0     0     1
        [ 2] .data             PROGBITS         0000000000000000  00000050
             0000000000000008  0000000000000000  WA       0     0     4
        [ 3] .bss              NOBITS           0000000000000000  00000058
             0000000000000010  0000000000000000  WA       0     0     4
        [ 4] .rodata           PROGBITS         0000000000000000  00000058
             0000000000000004  0000000000000000   A       0     0     4
        [ 5] .comment          PROGBITS         0000000000000000  0000005c
             000000000000002c  0000000000000001  MS       0     0     1
        [ 6] .note.GNU-stack   PROGBITS         0000000000000000  00000088
             0000000000000000  0000000000000000           0     0     1
        [ 7] .note.gnu.propert NOTE             0000000000000000  00000088
             0000000000000020  0000000000000000   A       0     0     8
        [ 8] .eh_frame         PROGBITS         0000000000000000  000000a8
             0000000000000038  0000000000000000   A       0     0     8
        [ 9] .rela.eh_frame    RELA             0000000000000000  00000310
             0000000000000018  0000000000000018   I      10     8     8
        [10] .symtab           SYMTAB           0000000000000000  000000e0
             00000000000001b0  0000000000000018          11    16     8
        [11] .strtab           STRTAB           0000000000000000  00000290
             000000000000007c  0000000000000000           0     0     1
        [12] .shstrtab         STRTAB           0000000000000000  00000328
             000000000000006f  0000000000000000           0     0     1
      Key to Flags:
        W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
        L (link order), O (extra OS processing required), G (group), T (TLS),
        C (compressed), x (unknown), o (OS specific), E (exclude),
        l (large), p (processor specific)
      ```
   2. .data大小为8(0000000000000008);在目标文件占用8字节(00000050-00000058)
   3. .bss大小为16(0000000000000010);在目标文件占用0字节(00000058-00000058)
   4. .rodata的大小为4(0000000000000004);在目标文件占用4字节(00000058-0000005c)
   5. 综上:
      1. .rodata	用于维护只读数据，比如：常量字符串、带 const 修饰的全局变量和静态变量等	在目标文件中占用空间
      2. .data	用于维护初始化的且初始值非0的全局变量和静态变量（不带 const 修饰）	在目标文件中占用空间
      3. .bss	用于维护未初始化的或初始值为0的全局变量和静态变量（不带 const 修饰）	不占用目标文件的空间