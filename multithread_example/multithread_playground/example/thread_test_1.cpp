/*** 
 * @Author: tylerytr
 * @Date: 2023-08-14 11:53:48
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-14 13:53:25
 * @FilePath: /CPP_example/multithread_example/multithread_playground/example/thread_test_1.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <iostream>
#include <thread>

using namespace std;

void myprint(const int& i, char* pmybuf) { // 这里尽管是按照引用传递，但是观察内存地址可以发现这里内部一定做了复制
	cout << i << endl;                     // 按照指针传递，pmybuf的地址和传入进来的参数地址一样
	cout << pmybuf << endl;
}
void myprint2(const int& i, const string& pmybuf){
	cout << i << endl;                   
	cout << pmybuf << endl;
}
int main()
{
	//测试1
	// int mvar = 1;       // 创建临时对象
	// int &mvary = mvar;
	// char mybuf[] = "This is a test";

	// thread mytobj(myprint, mvar, mybuf);  // i并不是mvar的引用，实际上是按照值传递；但是mybuf是按照地址传递！
	// mytobj.join();
	// //mytobj.detach();                   // 尽管主线程detach了子线程，那么子线程中用i值也是安全的，但是使用mybuf一定会出问题！
	// cout << "I Love China" << endl;
	
	//测试2
	int mvar = 1;      
	int &mvary = mvar;
	char mybuf[] = "This is a test";
	//thread mytobj(myprint2, mvar, mybuf);     // 这里存在一个隐式的类型转换char-->string 
    thread mytobj(myprint2, mvar, string(mybuf)); // 一个可行的方法！！！！！程序会先调用string中的构造函数创建一个临时对象，接着并不是引用这个临时对象，而是调用copy构造函数又复制出来一个对象给子线程使用
	mytobj.detach();                         // 一个潜在的风险是转换的时机可能发生在mybuf[]被回收后
	// mytobj.join();                  
	cout << "I Love China" << endl;
	
	return 0;
}