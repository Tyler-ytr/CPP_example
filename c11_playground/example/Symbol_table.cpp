/*** 
 * @Author: tylerytr
 * @Date: 2023-08-15 14:21:48
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-15 14:54:42
 * @FilePath: /CPP_example/c11_playground/example/Symbol_table.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
/*
 * @Author: tylerytr
 * @Date: 2023-08-15 14:21:48
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-15 14:50:55
 * @FilePath: /CPP_example/c11_playground/example/Symbol_table.c
 * Email:601576661@qq.com
 * Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
/**
 *函数funcA
 */
// int funcA() {
// return 0;
// }
// int temp;
// int main(int argc, char *argv[]){
  
//   return 0;
// }
int temp;//.bss
int main(){
  static int val_1;//.bss
  static int val_2 = 0;//.bss
  static int val_3 = 1;//.data
  static int val_4 = 0;//.bss
  static int val_5 = 2;//.data
  const static int val_6 = 0;//.rodata
    return 0;

}