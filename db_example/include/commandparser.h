/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-31 16:12:48
 * @LastEditTime: 2023-03-31 21:14:09
 * @LastEditors: tylerytr
 * @FilePath: /db_example/include/commandparser.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <limits.h>
using std::string;
enum class ParserResult{
    //SQL语句
    SUCCESS,
    //shell其他命令
    EXIT,
    RESET,
    HELP,
    //错误
    SYBTAX_ERROR,
    BAD_ID,
    STRING_TOO_LONG,
    UNRECONGNIZED_STATEMENT
};
enum class StatementType
{
    /*index,name,age,email*/
    INSERT,
    UPDATE,
    /*index*/
    SELECT_ONE,
    /*min_index,max_index*/
    SELECT_RANGE,
    /*index*/
    DELETE,
    NONE
};
struct Row{
    int id;
    int age;
    int min_index;
    int max_index;
    string username;
    string email;
};
struct Statement{
    StatementType type;
    Row row;
};

class CommandParser{
    public:
        CommandParser();
        Statement state;
        ParserResult command(const std::string& command);
        void printState();
};