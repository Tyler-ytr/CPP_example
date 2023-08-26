/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-27 15:19:47
 * @LastEditTime: 2023-03-31 13:32:24
 * @LastEditors: tylerytr
 * @FilePath: /CPP_example/skip_list_example/skip_list/skip_node.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
# pragma once
#include <vector>
template <typename T=int>
struct SkipNode{
    int key;
    T value;
    std::vector<SkipNode*> next;//用于存储结点在所有层数上的下一个结点。

    SkipNode(int k,T v,int level);
};

#include "skip_node.tpp"
