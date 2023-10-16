//函数指针版本;
#include <iostream>

struct printnum_args_t{
    int &x;
    int &y;
};

void func_printnum(void*arg){
    auto a=(printnum_args_t*)arg;
    std::cout<<"x: "<<a->x<<" y: "<<a->y<<std::endl;
}





typedef void (*func_pointer)(void*arg);

void repeat(func_pointer func,void*arg){
    func(arg);
    func(arg);
}

int main(){
    int x;
    int y;
    std::cin>>x>>y;
    printnum_args_t args={x,y};
    repeat(func_printnum,&args);
    std::cin>>x>>y;
    repeat(func_printnum,&args);
    return 0;
}