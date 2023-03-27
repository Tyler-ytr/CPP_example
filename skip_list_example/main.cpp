#include <iostream>
#include "skip_list/skip_list.h"
#include <time.h>
//template class SkipList<int>;
int main(){
    int maxLevel=6;
    srand (time(NULL)); 
    SkipList<int> l(maxLevel, 0);
    for (size_t i = 0; i < 100; i++)
	{
		l.insert(i, i);
	}

	//l.printList();
    return 0;

    ;
}