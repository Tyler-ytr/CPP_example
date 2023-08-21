/*** 
 * @Author: tylerytr
 * @Date: 2023-08-16 20:24:07
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-21 14:25:56
 * @FilePath: /CPP_example/linux_playground/process_communication_example/example/Pipe.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#define MAXLINE 80
using namespace std;
int main(void)
{
	int _pipe[2]={0,0};
	int ret = pipe(_pipe);// 创建无名管道,参数返回写和读文件操作符

	if (ret == -1) {
        cout << "create pipe fail!" << endl;
        return 1;
    }
	cout << "create pipe: " << _pipe[0]<<" " << _pipe[1] << endl;
	pid_t pid = fork();
	if(pid<0){//fork 失败
		cout<<"Fork error"<<endl;
		return -1;
	}
	//子进程写-->父进程读
	if(pid==0){
		//目前是子进程
		cout<<"******Child process******"<<endl;
		close(_pipe[0]);    //子进程关闭管道读文件操作符

		// 发送数据到pipe
		const char* msg = "Child process send message to parent process!";
		int count = 5;
		while (count--) {
            write(_pipe[1], msg, strlen(msg));
            sleep(1);
        }
		close(_pipe[1]);
		exit(1); 
	}else{
		//目前是父进程
		cout<<"******Parent process******"<<endl;
		close(_pipe[1]);    //父进程关闭管道写文件操作符
		// 读取pipe数据
		char msg[1024];
		int count = 5;
		while (count--) {
			memset(msg,'\0',sizeof(msg));
            ssize_t n = read(_pipe[0], msg, sizeof(msg) );
			cout << "recive from child: " << msg <<" Read return "<<n<< endl;
        }
		// 等待子进程结束再退出父进程
        if (waitpid(pid, 0, 0) != -1) {
            cout << "child closed!" << endl;
        }


	}

	return 0;
}