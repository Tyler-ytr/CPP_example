/*** 
 * @Author: tylerytr
 * @Date: 2023-08-14 13:59:39
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 14:19:52
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/thread_test_2.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
// #include "pch.h"
#include <iostream>
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
void MyTestThread(const TestThread& testthread) { // 参数不使用值传递，不然会构建出3个对象（执行2次copy构造函数）
	cout << "子线程的ID是：" <<std::this_thread::get_id()<< endl;
}
int main()
{
	cout << "主线程的ID是：" << std::this_thread::get_id() << endl;
    int mvar = 1;
    //测试1
    //thread mytobj1(MyTestThread,mvar);             // 这种情况是在子线程中构造的临时对象
	//测试2
    //thread mytobj1(MyTestThread,TestThread(mvar)); // 这种情况是在主线程中构造的临时对象
    //测试3
    // TestThread aaa(20);
	// thread mytobj1(MyTestThread, std::ref(aaa));    // 不会再copy出来一份了
    //测试4
    TestThread aaa(20);
    thread mytobj1(&TestThread::thread_work, aaa, 15);
    // thread mytobj1(&TestThread::thread_work, std::ref(aaa), 15); //这时候使用detach就会出现问题
    // thread mytobj1(&TestThread::thread_work, &aaa, 15); // 等同于上一种写法
	mytobj1.join();	
	return 0;
}