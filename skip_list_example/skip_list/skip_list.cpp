/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-27 14:32:19
 * @LastEditTime: 2023-03-27 17:07:04
 * @LastEditors: tylerytr
 * @FilePath: /CPP_example/skip_list_example/skip_list/skip_list.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/

#include "skip_list.h"
//初始化
template<class T>SkipList<T>::SkipList(int maxLevel,T iniValue):maxLevel(maxLevel){
    head=new SkipNode<T>(minInt,iniValue,maxLevel);
    tail=new SkipNode<T>(maxInt,iniValue,maxLevel);
    //所有层数上面的头结点都指向尾结点
    for(int i=0;i<maxLevel;i++){
        head->next[i]=tail;
    }
}
//析构
template<class T>SkipList<T>::~SkipList(){
    //删除所有节点;需要测试;
    SkipNode<T>* p=head;
    SkipNode<T>* q;
    while(p!=tail){
        q=p;
        p=p->next[0];
        delete q;
    }
    // assert(tail)
    // delete tail;
    

}
//随机层数
template<class T>int SkipList<T>::randomLevel(){
    //25%的概率让node的level+1

    int level=1;
    while(rand()%4==0){
        level++;
    }
    level=(level<maxLevel)?level:maxLevel;
    return level;
}

template<class T>int SkipList<T>::nodeLevel(std::vector<SkipNode<T>*> next){
    int nodeLevel=0;

    if(next[0]->key==maxInt){
        return nodeLevel;//这个是头结点;
    }

    for(SkipNode<T>*nodePtr:next){
        if(nodePtr!=nullptr&&nodePtr->key!=maxInt){
            nodeLevel++;
        }else{
            break;
        }
    }
    return nodeLevel;

}

//操作的函数
/*
    插入:
    1. 首先用查找函数判断这个函数是否存在，如果存在的话就更新该节点的值
    2. 获取新节点的随即层数
    3. 找到合适的插入位置
    4. 插入，调整每层前后的指针
*/
template<class T>SkipNode<T>*SkipList<T>:: insert(int key,T value){
    SkipNode<T>* newNode = nullptr;
    SkipNode<T>* tmp = head;
    newNode = find(key);
    if(newNode){
        //节点存在那么更新;
        newNode->value=value;
        cout << "\nThis node " << key << " has already existed. And its value has been updated to " << value << endl;
        return head;

    }
    int x_level=randomLevel();
    cout << "key: " << key  << ", randomLevel: " << x_level << endl;
    newNode=new SkipNode<T>(key,value,x_level);
    for(int i=x_level-1;i>=0;i--){
        while(tmp->next[i]!=nullptr&&tmp->next[i]->key<key){
            tmp=tmp->next[i];
        }
        newNode->next[i]=tmp->next[i];
        tmp->next[i]=newNode;
    }
    return head;

}

/*
    查找:
    1. 首先找到小于该节点的最近的节点，如果下一个节点等于目标节点就返回，否则返回空
*/
/*
                           +------------+
                           |  select 60 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/

template<class T>SkipNode<T>*SkipList<T>::find(int key){
    SkipNode<T>* p=head;
    int currentLevel=nodeLevel(p->next);
    //从高往低走，尽可能跳的更多
    for(int i=currentLevel-1;i>=0;i--){
        while(p->next[i]!=nullptr&&p->next[i]->key<key){
            p=p->next[i];
        }
    }

    p=p->next[0];
    
    if(p->key==key){
        cout << "\nThis key " << key << " has been found\n";
        return p;
    }else{
        return nullptr;//没找到
    }
    
}
/*
    删除:
    1. 用find判断节点是否存在，不存在的话返回当前list，并且告知
    2. 找到小于该节点的最近的节点;
    3. 更改该节点每层的前面节点的指针;
*/
template<class T> SkipNode<T>* SkipList<T>::deletenode(int key){
    SkipNode<T>*node=find(key);
    if(node==nullptr){
        cout << "\n This deleting node" << key << "doesn't exist" << endl;
        return head;
    }
    else{
        SkipNode<T>*tmp=head;
        int x_level=node->next.size();
        cout << "\nThe deleting node " << key << "'s level is " << x_level << endl;
        for(int i=x_level-1;i>=0;i--){
            while(tmp->next[i]!=nullptr&&tmp->next[i]->key<key){
                tmp=tmp->next[i];
            }
            tmp->next[i]=tmp->next[i]->next[i];
            cout << "This node " << key << " has been deleted from level " << i << endl;
            
        }
        return head;
    }
}

//打印函数
template<class T> void SkipList<T>::printList(){
    for(int i=0;i<maxLevel;i++){
        SkipNode<T>*tmp=head;
        int lineLen=1;
        if(tmp->next[i]->key!=maxInt){
            cout << "\n";
			cout << "This is level " << i << ":" << endl;
			cout << "{";
        }

        while(tmp->next[i]!=nullptr&&tmp->next[i]->key!=maxInt){
            cout << "(" << "Key: " << tmp->next[i]->key << ", ";
			cout << "Value: " << tmp->next[i]->value << ")";

            tmp = tmp->next[i];

            if (tmp->next[i] != nullptr && tmp->next[i]->key != maxInt){
					cout << ", ";
			}

            if (lineLen++ % 5 == 0) cout << "\n";
        }
        cout << "}" << "\n";
    }
    
}
template class SkipList<int>;
