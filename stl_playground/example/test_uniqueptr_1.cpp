#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
using namespace std;


class temp{
    void *p;
public:
    temp(const char*name){
        p=malloc(10);
        cerr<<__PRETTY_FUNCTION__<<endl;
        cerr<<p<<" = malloc()"<<endl;
    }
    ~temp(){
        free(p);

        cerr<<p<<" = free()"<<endl;
        cerr<<__PRETTY_FUNCTION__<<endl;
    }

    // //拷贝构造
    temp(const temp& other){
        // p=malloc(1);
        //p=other.p;
        cerr<<__PRETTY_FUNCTION__<<endl;
        
    }

    //拷贝赋值
    temp& operator=(const temp& other){
        // p=malloc(1);
        cerr<<__PRETTY_FUNCTION__<<endl;

        return *this;
    }
    
    //移动构造
    temp(temp&& other){
        p=exchange(other.p,nullptr);
        cerr<<__PRETTY_FUNCTION__<<endl;
    }

    //移动赋值
    temp& operator=(temp&& other){
        if(this!=&other){
            free(p);
            cerr<<p<<" = free()"<<endl;
            p=exchange(other.p,nullptr);
            cerr<<__PRETTY_FUNCTION__<<endl;
        }
        return *this;
    }

};
void test(temp cur){
    cerr<<__PRETTY_FUNCTION__<<endl;
    return ;
}
vector<temp> temp_list;
void test2(temp cur){
    cerr<<__PRETTY_FUNCTION__<<endl;
    temp_list.push_back(move(cur));
    return ;
}


int main(){
    // // 拷贝构造
    // {
    //     auto a=temp("1");
    //     cerr<<"begin test call"<<endl;
    //     test(a);
    //     cerr<<"end test call"<<endl;
    // }    
    


    // // 拷贝赋值
    // {
    //     auto a=temp("1");
    //     cerr<<"begin copy"<<endl;
    //     auto b=a;
    //     cerr<<"end copy"<<endl;
    // }

    // //移动构造
    // {
    //     auto a=temp("1");
    //     cerr<<"begin test2 call"<<endl;
    //     test2(move(a));
    //     cerr<<"end test2 call"<<endl;
    // }
    // cerr<<"123"<<endl;
    
    //移动赋值函数
    {
        auto a=temp("1");
        auto b=temp("2");
        cerr<<"begin move"<<endl;
        b=move(a);
        cerr<<"end move"<<endl;
    }

    return 0;
}