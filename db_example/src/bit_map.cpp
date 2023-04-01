#include "bit_map.h"
void BitMap::set_1(const int&index){
    _bitmap[index>>5]|=(1<<(index&31));
}
void BitMap::set_0(const int&index){
    _bitmap[index>>5]&=~(1<<(index&31));
}
bool BitMap::get(const int&index){
    return _bitmap[index>>5]&(1<<(index&31));
}