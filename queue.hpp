#include <iostream>
#include <vector>

namespace Queue{
template<typename param_t>
class Queue{

    public:
    std::vector<param_t> queue;   

    Queue(){
        
    }

    Queue(std::vector<param_t> arr){
        queue = arr;
    }

    void push_back(param_t object){
        queue.push_back(object);
    }

    int size(){
        queue.size();
    }

    param_t pop_front(){
        if(queue.size() > 0){
            param_t p = queue[0];
            queue.erase(queue.begin());
            return p;
        } else throw std::out_of_range("The queue is empty.");
    }
};
}