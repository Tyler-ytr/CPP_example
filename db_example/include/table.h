/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-04-01 13:25:46
 * @LastEditTime: 2023-04-02 14:01:08
 * @LastEditors: tylerytr
 * @FilePath: /db_example/include/table.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/

#pragma once
#include "page.h"
// 存储state里面的row到page中;
// 缓存池的超级简化版本，总体设计是一个bitmap记录脏页，然后一个page指针数组指向各个page，每一个page4KB，每一个row 4+4+32+215=255byte
// 一个page有15行，每一行的结构是ID(4 Byte);age(4 Byte);username(32 Byte);email(215 Byte)；dirty bit(1 Byte)
// 最多有1024个page，用一个bitmap表示该页有没有修改；


class table{
    public:

    private:
        int page_num;

};
