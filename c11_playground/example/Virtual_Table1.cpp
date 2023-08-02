/*** 
 * @Author: tylerytr
 * @Date: 2023-08-02 09:48:48
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-02 10:52:34
 * @FilePath: /CPP_example/c11_playground/example/Virtual_Table1.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
/*** 
 * @Author: tylerytr
 * @Date: 2023-08-02 09:48:48
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-02 09:52:38
 * @FilePath: /CPP_example/c11_playground/example/Virtual_Table.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
//参考:https://www.cnblogs.com/Mered1th/p/10924545.html
#include "string.h"

using namespace std;

//单继承及覆盖
// class Base_1 {
//      public:
//             virtual void f() {  ;}
//             virtual void g() { ;}
//             virtual void h() { ;}
			
// };
// class Devired_1 :public Base_1{
// public:
// 	virtual void x() { ; }
// 	virtual void f() { int a=1; }
	
// };

//多继承及覆盖
// class Base_2_1 {
//      public:
//             virtual void f() {  ;}
//             virtual void g() { ;}
//             virtual void h() { ;}
// 			virtual void h1() { ;}
			
// };
// class Base_2_2 {
//      public:
//             virtual void f() {  ;}
//             virtual void g() { ;}
//             virtual void h() { ;}
// 			virtual void h2() { ;}
			
// };


// class Devired_2 :public Base_2_1,public Base_2_2{
// public:
// 	virtual void x() { ; }
// 	virtual void f() { int a=1; }
	
// };

//菱形继承

class Super_3{
public:
    int a;
    int b;
public:
    virtual void f() { a=0; }
    virtual void g() { b=0; }
    virtual void h() { ; }
};
class Base_3_1:virtual public Super_3{

public:
    virtual void f() {a=1; }
    virtual void h1() { ; }
};
class Base_3_2:virtual public Super_3{
public:
    virtual void f() { a=2; }
    virtual void h2() { ; }
};
class Devired_3 :public Base_3_1,public Base_3_2{
public:
    virtual void f() { ; }//不能注释，否则会报重写不明确的错误;
    virtual void g() { b=3; }

};


int main()
{
	
	return 0;
}