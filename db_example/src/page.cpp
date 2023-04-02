#include "page.h"
page::page():next(nullptr),min_id(-1),max_id(-1),max_row_num(15){

    head=free_node=new row[max_row_num]();
    min_ptr=max_ptr=nullptr;
    for(int i=0;i<max_row_num-1;i++){
        free_node[i].next=&free_node[i+1];
    }
    free_node[max_row_num-1].next=nullptr;
}
row* page::new_row(){
    if(free_node==nullptr){
        return nullptr;
    }
    row* r=free_node;
    free_node=free_node->next;
    return r;
}
void page::free_row(row* r){
    r->next=free_node;
    free_node=r;
}


bool page::insert_row(const int id,const int age,const std::string& username,const std::string& email){
    row* place=new_row();
    if(place==nullptr){
        //满了 没法申请空间;
        return false;
    }
    place->id=id;
    place->age=age;
    strncpy(place->username,username.c_str(),32);
    strncpy(place->email,email.c_str(),211);
    place->next=nullptr;
    //初始化:
    if(min_ptr==nullptr){
        min_ptr=max_ptr=place;
        min_id=max_id=id;
        return true;
    }
    else{
        //从min_ptr开始遍历
        row* p=min_ptr;
        row* pre=nullptr;
        while(p!=nullptr){
            if(p->id==id){
                //已经存在
                return false;
            }
            else if(p->id>id){
                //插入到p前面
                if(pre==nullptr){
                    //插入到最前面
                    place->next=min_ptr;
                    min_ptr=place;
                    min_id=id;
                    return true;
                }else{
                    //插入到中间
                    place->next=p;
                    pre->next=place;
                    return true;
                }
            }
            else{
                //继续遍历
                pre=p;
                p=p->next;
            }
        }
        //插入到最后面
        pre->next=place;
        max_ptr=place;
        max_id=id;
        return true;
    }


}
row* page::get_row(const int id){
    if(max_id==-1){
        return nullptr;
    }
    if(id<min_id||id>max_id){
        return nullptr;
    }
    row* p=min_ptr;
    while(p!=nullptr){
        if(p->id==id){
            return p;
        }
        p=p->next;
    }
    return nullptr;
}
row* page::get_row_with_pre(const int id,row*& pre){
    if(id<min_id||id>max_id){
        return nullptr;
    }
    row* p=min_ptr;
    while(p!=nullptr){
        if(p->id==id){
            return p;
        }
        pre=p;
        p=p->next;
    }
    return nullptr;
}

bool page::delete_row(const int id){
    if(max_id==-1){
        return false;
    }
    row* pre=nullptr;
    row* p=get_row_with_pre(id,pre);
    if(p==nullptr){
        return false;
    }
    if(pre==nullptr){
        //删除的是第一个
        min_ptr=min_ptr->next;
        if(min_ptr==nullptr){
            max_ptr=nullptr;
            min_id=max_id=-1;
        }
        else{
            min_id=min_ptr->id;
        }
        free_row(p);
        return true;
    }
    else{
        if(p->next==nullptr){
            //删除的是最后一个
            max_ptr=pre;//pre一定不是空指针
            max_id=pre->id;
            pre->next=nullptr;
            free_row(p);
            return true;
        }
        else{
            //删除的是中间的
            pre->next=p->next;
            free_row(p);
            return true;
        }
    }

}

bool page::update_row(const int id,const int age,const std::string& username,const std::string& email){
    row* p=get_row(id);
    if(p==nullptr){
        return false;
    }
    p->age=age;
    strncpy(p->username,username.c_str(),32);
    strncpy(p->email,email.c_str(),211);
    return true;
}
page::~page(){
    delete[] head;
}

void page::print_page(){
    row* p=min_ptr;
    while(p!=nullptr){
        std::cout<<p->id<<" "<<p->age<<" "<<p->username<<" "<<p->email<<std::endl;
        p=p->next;
    }
}