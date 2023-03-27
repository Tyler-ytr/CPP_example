/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-27 14:32:09
 * @LastEditTime: 2023-03-27 17:15:24
 * @LastEditors: tylerytr
 * @FilePath: /CPP_example/skip_list_example/skip_list/skip_list.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#pragma once
//注意，在调用的时候需要在main函数中 srand (time(NULL));  
#include <iostream>
#include "skip_node.h"
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <limits>

template<class T=int>
class SkipList{
    public:
        //头节点
        SkipNode<T>* head;

        //最大层数
        int maxLevel;

        //整型的最大最小值
        const int minInt=std::numeric_limits<int>::min();
        const int maxInt=std::numeric_limits<int>::max();
    
    public:
        //构造函数
        SkipList(int maxLevel,T iniValue);
        //析构函数
        ~SkipList();
        //随机层数方法
        int randomLevel();

        //插入查找删除
        SkipNode<T>* insert(int key,T value);
        SkipNode<T>* find(int key);
        SkipNode<T>* deletenode(int key);

        //打印
        void printList();

    private:
        //尾部节点
        SkipNode<T>* tail;
        //找到当前列表或者node的最大层数
        int nodeLevel(std::vector<SkipNode<T>*> next);
};
