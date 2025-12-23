// HashMap.hpp
#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include "TreeNode.hpp"

class MyHashMap {
private:
    struct Item {
        int key;
        TreeNode* value;
        Item* next;
        
        Item(int k, TreeNode* v) {
            key = k;
            value = v;
            next = nullptr; 
        }
    };
    
    std::vector<Item*> table; 
    int size;
    int hash_func(int key) const {
        if(key < 0) {
            key = -key; // make positive
        }
        return key % size;
    }

public:
    MyHashMap(int cap = 101) {
        size = cap; 
        table.resize(size, nullptr); 
    }
    
    ~MyHashMap() {
        for(Item* head : table) {
            while(head) { 
                Item* temp = head; 
                head = head->next; 
                delete temp; 
            }
        }
    }
    
    void add(int key, TreeNode* val) {
        int bucket = hash_func(key);
        Item* curr = table[bucket];
        
        while(curr) {
            if(curr->key == key) {
                curr->value = val;
                return; 
            } 
            curr = curr->next; 
        }  
        Item* new_item = new Item(key, val);
        new_item->next = table[bucket]; 
        table[bucket] = new_item;
    }
    
    TreeNode* find(int key) const {
        int bucket = hash_func(key);
        Item* curr = table[bucket];
        
        while(curr) {
            if(curr->key == key) {
                return curr->value;
            }
            curr = curr->next;
        }
        return nullptr; 
    }
    bool has_key(int key) const { 
        return find(key) != nullptr; 
    }
};

#endif