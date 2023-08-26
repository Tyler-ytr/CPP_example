<!--
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-27 14:23:10
 * @LastEditTime: 2023-08-26 16:19:33
 * @LastEditors: tylerytr
 * @FilePath: /CPP_example/skip_list_example/README.md
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
-->
# Skip_list
skil_list文件夹中实现了跳表，通过模板支持任意的类型

1. 跳表是一个可以快速查找的有序链表, 搜索、插入、删除操作的时间均为O(logn);(因为链表的随机查找性能是O(n));
2. **跳表节点**,具有key,value以及一个next数组，用来存储节点在所有层数的下一个节点;**跳表**需要存储头节点，maxlevel以及对外提供插入查找删除打印等操作。
3. 跳表实现:
   1. randomlevel: 跳表多层的关键，通过随机数的方法,25%的概率让节点的level+1;
   2. insert:
      1. 首先用查找函数判断这个函数是否存在，如果存在的话就更新该节点的值
      2. 调用randomlevel获取新节点的随机层数
      3. 从next数组的高位往低位，逐层的向后遍历，直到找到合适的位置(也就是下一个节点的key比当前的大);
      4. 插入，调整前后指针
   3. find:
      1. 操作和insert的第三步类似;
   4. delete
      1. 使用find判断节点是否存在，不存在的话返回当前list并且告知上层
      2. 找到小于该节点的最近的节点
      3. 更改该节点每层的前面节点的指针;
4. 跳表时间复杂度计算,这里假设randomlevel的系数是50%，也就是1/2的概率多一层:
   1. 时间复杂度 = 索引的高度 * 每层索引遍历元素的个数。
   2. **索引高度的计算**：原始的链表有n个元素，则一级索引有$n/2$ 个元素、二级索引有 $n/4$ 个元素、k级索引就有 $n/(2^k)$个元素。最高级索引一般有2个元素，即：最高级索引 h 满足 $2 = n/2^h$，即 $h = \log_{2}n - 1$，最高级索引 h 为索引层的高度加上原始数据一层，跳表的总高度 $h =\log_{2}n$。
   3. **每层遍历元素的计算**：每层索引最多遍历三个元素(对于非随机化的实现而言，随机化其实就是期望上小于等于三);
   4. 因此是O($\log_{2}n*3$)=O($\log n$)
5. 跳表的空间复杂度为O(n);



## 参考
1. [项目结构与cmake](https://zhuanlan.zhihu.com/p/534439206)
2. [跳表实现1](https://blog.csdn.net/weixin_44387066/article/details/90766034)
3. [跳表原理与实现](https://juejin.cn/post/7019982872761237535)