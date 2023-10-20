#pragma once

#include<cstdio>
#include<utility>

template <class T>
struct DefaultDeleter{// 默认使用 delete 释放内存
    void operator()(T* ptr){
        delete ptr;
    }
};

template <class T>
struct DefaultDeleter<T[]>{ //数组 偏特化
    void operator()(T* ptr){
        delete[] ptr;
    }
};

template <>
struct DefaultDeleter<FILE>{ // FILE 全特化
    void operator()(FILE* ptr){
        fclose(ptr);
    }
};
// 实现了exchange函数 与标准库功能一致
template <class T,class U>
T exchange(T&dst,U&&val){
    T tmp=std::move(dst);
    dst=std::forward<U>(val);
    return tmp;
}

template <typename T,typename Deleter=DefaultDeleter<T>>
struct UniquePtr{
    private:
        T* m_p;//裸指针

        //没有这个的话会在动态多态使用的过程中报错，对应test_uniqueptr_2.cpp中的for循环部分
        template <class U, class UDeleter>
        friend struct UniquePtr;
    public:
        UniquePtr(std::nullptr_t dummy = nullptr) { // 默认构造函数
            m_p = nullptr;
        }
        UniquePtr(T* p) { // 普通构造函数
            m_p = p;
        }
        template <class U, class UDeleter, class = std::enable_if_t<std::is_convertible_v<U *, T *>>>
        UniquePtr(UniquePtr<U, UDeleter> &&that) {// 从子类型U的智能指针转换到T类型的智能指针
            m_p = exchange(that.m_p, nullptr);
        }
        ~UniquePtr() { // 析构函数
        if (m_p)
            Deleter{}(m_p);
        }
        //uniqueptr的实现原理：禁用拷贝构造拷贝赋值
        UniquePtr(UniquePtr const &that) = delete; // 拷贝构造函数
        UniquePtr &operator=(UniquePtr const &that) = delete; // 拷贝赋值函数

        UniquePtr(UniquePtr &&that) { // 移动构造函数
            m_p = exchange(that.m_p, nullptr);
        }

        UniquePtr &operator=(UniquePtr &&that) { // 移动赋值函数
            if (this != &that) {
                if (m_p)
                    Deleter{}(m_p);
                m_p = exchange(that.m_p, nullptr);
            }
            return *this;
        }

        //uniqueptr的功能:
        T *get() const { // 获取裸指针
            return m_p;
        }

        T *release(){ //释放裸指针的同时不delete对应内存
            return exchange(m_p,nullptr);
        }
        T *reset(T*p=nullptr){
            if(m_p)
                Deleter{}(m_p);
            m_p=p;
        }

        T &operator*() const { // 解引用
            return *m_p;
        }
        T *operator->() const { // 指针访问成员
            return m_p;
        }

};
template <class T, class Deleter>
struct UniquePtr<T[], Deleter> : UniquePtr<T, Deleter> {};

//完美转发任意数量的参数
template <class T, class ...Args>
UniquePtr<T> makeUnique(Args &&...args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
UniquePtr<T> makeUniqueForOverwrite() {
    return UniquePtr<T>(new T);
}
