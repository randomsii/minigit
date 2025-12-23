// Heap.hpp
#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <algorithm>

template<typename T, typename Compare>
class MyHeap {
private:
    std::vector<T> data; 
    Compare comp; 
    void bubble_up(int index) {
        while (index > 0) {
            int parent_idx = (index - 1) / 2;
            if (comp(data[index], data[parent_idx])) {
                std::swap(data[index], data[parent_idx]);
                index = parent_idx; 
            }
            else {
                break;
            }
        }
    }
    void bubble_down(int index) {
        int heap_size = data.size();
        while (true) {
            int left_child = 2 * index + 1;
            int right_child = 2 * index + 2;
            int best = index;
            if (left_child < heap_size && comp(data[left_child], data[best])) {
                best = left_child;
            }
            if (right_child < heap_size && comp(data[right_child], data[best])) {
                best = right_child;
            }              
            if (best == index) {
                break; 
            }
            
            std::swap(data[index], data[best]);
            index = best;
        }
    }

public:
    MyHeap() : data(), comp() {}
    void insert(const T& item) { 
        data.push_back(item); 
        bubble_up(data.size() - 1); 
    }
    void remove_top() { 
        if (data.empty()) return;
        data[0] = data.back(); 
        data.pop_back(); 
        
        if (!data.empty()) {
            bubble_down(0); 
        }
    }
    bool is_empty() const {
        return data.empty(); 
    }
    const T& peek() const { 
        return data.front(); 
    }
    int count() const { 
        return data.size(); 
    }
};

#endif