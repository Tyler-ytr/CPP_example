/*** 
 * @Author: tylerytr
 * @Date: 2023-08-27 18:45:57
 * @LastEditors: tylerytr
 * @LastEditTime: 2023-08-27 18:46:24
 * @FilePath: /cpp_server/include/inetaddress.h
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
 */
#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};