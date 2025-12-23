// TreeNode.hpp
#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <vector>
#include <ctime>

struct TreeNode {
    int id; 
    std::string data; 
    std::string msg; 
    time_t made_time; 
    time_t snap_time; 
    TreeNode* dad; 
    std::vector<TreeNode*> kids;
    TreeNode(int ver_id, const std::string& content) {
        id = ver_id;
        data = content;
        msg = "";
        made_time = std::time(nullptr);
        snap_time = 0;
        dad = nullptr;  
    }
    bool is_snap() const {
        return snap_time != 0;
    }
};

#endif       