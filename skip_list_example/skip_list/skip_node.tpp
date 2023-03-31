/*
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-27 15:19:18
 * @LastEditTime: 2023-03-31 13:21:14
 * @LastEditors: tylerytr
 * @FilePath: /CPP_example/skip_list_example/skip_list/skip_node.tpp
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
//构造函数初始化
#pragma once
#include "skip_node.h"
template<typename T> SkipNode<T>::SkipNode(int k, T v, int level) {
    key = k;
    value = v;
    next.resize(level, nullptr);
}
//template class SkipNode<int>;//特化 否则会报错 undefined reference to `SkipNode<int>::SkipNode(int, int, int)'