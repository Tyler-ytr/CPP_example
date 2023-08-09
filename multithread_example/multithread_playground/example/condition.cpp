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