/*** 
 * @Author: tylerytr
 * @Date: 2023-08-07 14:31:53
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-07 15:53:01
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/02_thread_manage.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

void print_time() {
  auto now = chrono::system_clock::now();
  auto in_time_t = chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
  cout << "now is: " << ss.str() << endl;
}

void sleep_thread() {
  this_thread::sleep_for(chrono::seconds(3));
  //this_thread::sleep_for(chrono::microseconds(1));//此时会发现loop_thread还没有执行完，但是主线程已经退出了。
  cout << "[thread-" << this_thread::get_id() << "] is waking up" << endl;
}

void loop_thread() {
  for (int i = 0; i < 10000; i++) {
    cout << "[thread-" << this_thread::get_id() << "] print: " << i << endl;
  }
}

int main() {
  print_time();

  thread t1(sleep_thread);
  thread t2(loop_thread);

  t1.join();
  t2.detach();

  print_time();
  return 0;
}