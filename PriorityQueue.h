#ifndef PRIORITY_H
#define PRIORITY_H

#include <vector>

template<typename T>
class PriorityQueue{
private:
    std::vector<T> heap;

    void upheap();
    void downheap();

public:
    PriorityQueue();
    void push();

};

#endif