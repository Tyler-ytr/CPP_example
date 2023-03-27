/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-27 15:19:47
 * @LastEditTime: 2023-03-27 17:15:43
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
    int value;
    std::vector<SkipNode*> next;

    SkipNode(int k,T v,int level);
};
