/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-04-02 14:02:18
 * @LastEditTime: 2023-04-02 15:52:56
 * @LastEditors: tylerytr
 * @FilePath: /db_example/include/page.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
using std::vector;
struct row{
    int id;
    int age;
    char username[32];
    char email[211];
    row* next;
    row():id(-1),next(nullptr){};
};
//类似于内存池
class page{
    public:
        page();
        bool insert_row(const int id,const int age,const std::string& username,const std::string& email);
        row* get_row(const int id);
        bool delete_row(const int id);
        bool update_row(const int id,const int age,const std::string& username,const std::string& email);
    
        void print_page();
        ~page();
    public:
        class page* next;
    public:
        int min_id;
        int max_id;//如果为-1，说明这个page是空的
        
        row*min_ptr;
        row*max_ptr;
        
    private:
        int max_row_num;//最大行数 设置为15
        row* free_node;//指向第一个空闲节点
        row* head;//保存整块空间的位置
        
        row* new_row();//申请空闲row
        void free_row(row* r);//释放空闲row

        row* get_row_with_pre(const int id,row*& pre);

        

};
