#include <iostream>

#include "page.h"
#include <string>
#include <vector>
using namespace std;
//names 是 a b c d …… o
//test 是 A B C D …… O
const vector<string> names={"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o"};
const vector<string> test={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O"};
int main(){
    page p;
    cout<<"single_test"<<endl;
    p.insert_row(1,1,"a","A");
    p.delete_row(1);

    cout<<"single_test success"<<endl;

    cout<<"multi_test"<<endl;
    for(int i=0;i<names.size();i++){
        p.insert_row(i,i,names[i],test[i]);
    }
    if(p.min_id!=0||p.max_id!=names.size()-1){
        cout<<"error"<<endl;
    }
    if(p.insert_row(16,16,"p","P")){
        cout<<"error"<<endl;
    }
    for(int i=0;i<names.size();i++){
        if(i%2==0)
        p.delete_row(i);
    }
    for(int i=0;i<names.size();i++){
        if(i%2==0){
            if(p.get_row(i)!=nullptr){
                cout<<"error"<<endl;
            }
            if(p.update_row(i,99,names[i],test[i])){
                cout<<"error"<<endl;
            }
        }else{
            if(p.get_row(i)==nullptr){
                cout<<"error"<<endl;
            }
            if(!p.update_row(i,99,names[i],test[i])){
                cout<<"error"<<endl;
            }
        }
    }
    p.print_page();
    cout<<p.min_id<<" "<<p.max_id<<endl;
    for(int i=0;i<names.size();i++){
        if(i%2!=0){
            cout<<p.min_id<<" "<<p.max_id<<endl;
            p.delete_row(i);
        }
    }
    cout<<p.min_id<<" "<<p.max_id<<endl;
    
    cout<<"multi_test success"<<endl;



    




}