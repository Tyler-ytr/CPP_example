/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-04-01 13:43:53
 * @LastEditTime: 2023-04-01 14:08:00
 * @LastEditors: tylerytr
 * @FilePath: /db_example/include/bit_map.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#pragma once
#include <algorithm>
#include <vector>
using std::vector;

class BitMap{
    public:
        BitMap(const int&size){//开辟一个大小为size的bitmap
            _bitmap.resize((size>>5)+1);
        }
        void set_1(const int&index);//将index位置的bit设置为1
        void set_0(const int&index);//将index位置的bit设置为0
        bool get(const int&index);//获取index位置的bit
    private:
        vector<int> _bitmap;
        
};