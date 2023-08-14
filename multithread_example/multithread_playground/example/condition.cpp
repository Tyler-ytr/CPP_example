/*** 
 * @Author: tylerytr
 * @Date: 2023-08-08 15:48:27
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 09:55:34
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/condition.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */




//宋询测试代码
#include<iostream>
#include<vector>
#include<condition_variable>
#include<thread>
using namespace std;

condition_variable cv;
mutex m;
int flag = 2;

void SubRoutine() {
  unique_lock<mutex> ul(m);
  this_thread::sleep_for(chrono::milliseconds(2300));
  cv.wait(ul, [](){
    cout << "waiting..." << endl;
    return false;
  });
  cout << " wait finished " << endl;

}

int main () {
  thread t1(SubRoutine);
  {
    cout << " main thread running... " << endl;
    flag = 8;
  }
  this_thread::sleep_for(chrono::milliseconds(2500));
  cv.notify_one();
  t1.join();
  return 0;
}