/*** 
 * @Author: tylerytr
 * @Date: 2023-08-07 14:14:05
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-07 15:54:33
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/01_hello_thread.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>
#include <thread>  

using namespace std; 

void hello() { 
    cout << "Hello World from new thread." << endl;
}
void hello3(string name) {
    cout << "Welcome to " << name << endl;
}

int main() {
    thread t1(hello); // 创建线程的方式就是构造一个thread对象，并指定入口函数。与普通对象不一样的是，此时编译器便会为我们创建一个新的操作系统线程，并在新的线程中执行我们的入口函数。
    cout<<"执行join前:"<<t1.joinable()<<endl;
    t1.join(); // 
    cout<<"执行join后:"<<t1.joinable()<<endl;
    thread t2([] {
        cout << "Hello World from lambda thread." << endl;
    });

    t2.join();
    thread t3(hello3, "https://tyler-ytr.github.io/");
    t3.join();
    thread t4;
    cout<<t1.joinable()<< " "<<t2.joinable()<<" "<<t3.joinable()<<" "<<t4.joinable()<<endl;
    return 0;
}
