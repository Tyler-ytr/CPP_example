/*** 
 * @Author: tylerytr
 * @Date: 2023-08-07 14:42:27
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-07 15:54:39
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/03_call_once.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

void init() {
  cout << "Initialing..." << endl;
  // Do something...
}

void worker(once_flag* flag) {
  call_once(*flag, init);
}

int main() {
  once_flag flag;

  thread t1(worker, &flag);
  thread t2(worker, &flag);
  thread t3(worker, &flag);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}