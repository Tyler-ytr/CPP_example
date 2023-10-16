#include <iostream>

struct func_printnum_t{
    void operator()() const{
        std::cout<<"x: "<<x<<" y: "<<y<<std::endl;
    }
    
    int &x;
    int &y;
};

template <class Fn>
void repeat(Fn const& func){
    func();
    func();
}

int main(){
    int x;
    int y;
    std::cin>>x>>y;
    func_printnum_t func={x,y};
    // repeat(func);
    // std::cin>>x>>y;
    // repeat(func);
    repeat([&x,&y](){
        std::cout<<"x: "<<x<<" y: "<<y<<std::endl;
    });
    repeat(func);
    return 0;
}