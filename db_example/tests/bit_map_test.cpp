#include "bit_map.h"
#include <iostream>
using namespace std;
struct row{
    int id;
    int age;
    char username[32];
    char email[211];
    int exist;
};
int main(){
    BitMap bm(66666);
    for(int i=0;i<=66666;i++){
        if(i%2==0)
        bm.set_1(i);
    }
    for(int i=0;i<=66666;i++){
        if(i%2==0){
            if(bm.get(i)==false){
                cout<<"error"<<endl;
            }
        }else{
            if(bm.get(i)==true){
                cout<<"error"<<endl;
            }
        }

    }

    cout<<"success"<<endl;
    cout<<sizeof(row)<<endl;


    return 0;
}