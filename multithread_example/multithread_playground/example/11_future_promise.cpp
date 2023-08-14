#include <cmath>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

static const int MAX = 10e8;

double concurrent_worker(int min, int max) {
  double sum = 0;
  for (int i = min; i <= max; i++) {
    sum += sqrt(i);
  }
  return sum;
}
//concurrent_task不再直接返回计算结果，而是增加了一个promise对象来存放结果。
void concurrent_task(int min, int max, promise<double>* result) {
  vector<future<double>> results;

  unsigned concurrent_count = thread::hardware_concurrency();
  min = 0;
  for (int i = 0; i < concurrent_count; i++) {
    packaged_task<double(int, int)> task(concurrent_worker);
    results.push_back(task.get_future());

    int range = max / concurrent_count * (i + 1);
    thread t(std::move(task), min, range);
    t.detach();

    min = range + 1;
  }

  cout << "threads create finish" << endl;
  double sum = 0;
  for (auto& r : results) {
    sum += r.get();
  }
  result->set_value(sum);//在任务计算完成之后，将总结果设置到promise对象上。一旦这里调用了set_value，其相关联的future对象就会就绪。
  cout << "concurrent_task finish" << endl;
}

int main() {
  auto start_time = chrono::steady_clock::now();

  promise<double> sum;//这里是在main中创建一个promoise来存放结果，并以指针的形式传递进concurrent_task中。
  concurrent_task(0, MAX, &sum);

  auto end_time = chrono::steady_clock::now();
  auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
  cout << "Concurrent task finish, " << ms << " ms consumed." << endl;
  cout << "Result: " << sum.get_future().get() << endl;//通过sum.get_future().get()来获取结果。第2点中已经说了：一旦调用了set_value，其相关联的future对象就会就绪。
  return 0;
}