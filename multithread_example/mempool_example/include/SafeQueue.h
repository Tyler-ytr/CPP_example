#pragma once
//实现线程安全的队列
#include <queue>
#include <mutex>
template <typename T>
class SafeQueue
{
    private:
        std::queue<T> m_queue;//利用模板函数构造队列
        std::mutex m_mutex;//构造互斥信号量

    public:
        SafeQueue() {}
        SafeQueue(SafeQueue &&other) {}
        ~SafeQueue() {}

        bool empty(){
            // 返回队列是否为空
            std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
            return m_queue.empty();
        }

        int size()
        {
            //返回队列大小
            std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
            return m_queue.size();
        }

        void enqueue(T&t){
            // 入队
            std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变

            m_queue.emplace(t); // 将t压入队列
        }
        bool dequeue(T&t){
            //取出元素到t
            std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
            //队列为空，返回false
            if(m_queue.empty())
                return false;
            
            t=std::move(m_queue.front()); // 将队列首元素移动到t
            m_queue.pop(); // 弹出队列首元素

            return true;

        }


};