//构造函数初始化
#include "skip_node.h"
template<typename T> SkipNode<T>::SkipNode(int k, T v, int level) {
    key = k;
    value = v;
    next.resize(level, nullptr);
}
template class SkipNode<int>;//特化 否则会报错 undefined reference to `SkipNode<int>::SkipNode(int, int, int)'