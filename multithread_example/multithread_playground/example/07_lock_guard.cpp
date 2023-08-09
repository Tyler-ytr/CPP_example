/*** 
 * @Author: tylerytr
 * @Date: 2023-08-07 15:36:26
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-08 00:13:00
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/07_lock_guard.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <thread>
#include <mutex>
#include <iostream>
 
int g_i = 0;
std::mutex g_i_mutex;  
 
void safe_increment()
{
  std::lock_guard<std::mutex> lock(g_i_mutex);  
  ++g_i;
  std::cout << std::this_thread::get_id() << ": " << g_i << '\n';
  
}
 
int main()
{
  std::cout << "main: " << g_i << '\n';
 
  std::thread t1(safe_increment); 
  std::thread t2(safe_increment);
 
  t1.join();
  t2.join();
 
  std::cout << "main: " << g_i << '\n';
}