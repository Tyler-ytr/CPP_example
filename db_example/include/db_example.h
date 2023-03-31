/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-31 14:30:01
 * @LastEditTime: 2023-03-31 21:31:04
 * @LastEditors: tylerytr
 * @FilePath: /db_example/include/db_example.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#pragma once
#include <iostream>
#include <iomanip> 
#include <string>
#include "command_parser.h"
class db_example{
    public:
        db_example();
        void run_db();
    private:
        void shellProgress();//解析cin命令行，调用对应函数

    
};

