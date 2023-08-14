/*** 
 * @Author: tylerytr
 * @Date: 2023-08-14 09:52:14
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 10:53:57
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/09_async_task.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <cmath>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std;

static const int MAX = 10e8;
static double sum = 0;

void worker(int min, int max) {
  for (int i = min; i <= max; i++) {
    sum += sqrt(i);
  }
}

class Worker {
public:
  Worker(int min, int max): mMin(min), mMax(max) {}
  double work() {
    mResult = 0;
    for (int i = mMin; i <= mMax; i++) {
      mResult += sqrt(i);
    }
    return mResult;
  }
  double getResult() {
    return mResult;
  }

private:
  int mMin;
  int mMax;
  double mResult;
};

int main() {
  /************第一部分***************/
  sum = 0;
  auto f1 = async(worker, 0, MAX);//这里以异步的方式启动了任务。它会返回一个future对象。
  cout << "Async task triggered" << endl;
  f1.wait();//等待异步任务执行完成。
  cout << "Async task finish, result: " << sum << endl << endl;

  /************第二部分***************/
  double result = 0;
  cout << "Async task with lambda triggered, thread: " << this_thread::get_id() << endl;
  //这里以异步的方式启动了任务。它会返回一个future对象。
  auto f2 = async(launch::async, [&result]() {
    cout << "Lambda task in thread: " << this_thread::get_id() << endl;
    for (int i = 0; i <= MAX; i++) {
      result += sqrt(i);
    }
  });
  f2.wait();
  cout << "Async task with lambda finish, result: " << result << endl << endl;
  /************第三部分***************/
  Worker w(0, MAX);
  cout << "Task in class triggered" << endl;
  auto f3 = async(&Worker::work, &w);//work是一个成员函数，隐藏参数是一个this指针，通过传递&w 传递给work函数this指针；这里使用&是传递引用，如果不使用会创建临时对象并拷贝。
  f3.wait();
  cout << "Task in class finish, result: " << w.getResult() << endl << endl;

  return 0;
}