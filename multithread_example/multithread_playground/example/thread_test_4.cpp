/*** 
 * @Author: tylerytr
 * @Date: 2023-08-14 15:45:17
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 17:10:00
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/thread_test_4.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include<stdio.h>
#include<iostream>
#include <string.h>
#include <memory>
#include <mutex>
#include <thread>
using namespace std;
class TestThread {
public:
	TestThread(int i) :data_(i) { 
		cout << "构造函数被执行" << this <<"创建此对象的ID是："<<std::this_thread::get_id()<< endl;
	}
	TestThread(const TestThread& testthread) :data_(testthread.data_) { 
		cout << "Copy构造函数被执行" << this <<"执行copy操作的线程ID是："<<std::this_thread::get_id()<< endl;
	}
	~TestThread() { 
		cout << "析构函数被执行" << this <<"执行析构的线程ID是："<<std::this_thread::get_id()<< endl;
	}
    void thread_work(int num){
        cout<<"类TestThread中的线程函数"<<this<<"所在线程ID"<<std::this_thread::get_id()<<endl;
    }
public:
	int data_;
};

shared_ptr<TestThread> g(new TestThread(0));
shared_ptr<TestThread> x;
shared_ptr<TestThread> n(new TestThread(1));


void thread_f1()
{
    x=g;
}
void thread_f2()
{
    g=n;
}
//g++ -std=c++11 thead_01.cpp -lpthread 
int main(int argc, char** argv)
{
    thread thread1(thread_f1);
    thread thread2(thread_f2);

    thread1.detach();
    thread2.detach();

    cout << "g->data_ is " << g->data_ << endl;
    cout <<"x->data_ is " << x->data_<<endl;



    return 0;
}