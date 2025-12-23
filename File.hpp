// File.hp
#ifndef FILE_HPP
#define FILE_HPP

#include "TreeNode.hpp"
#include "HashMap.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

class MyFile {
public:
    std::string name; 
    TreeNode* first_version; 
    TreeNode* current_version; 
    MyHashMap version_lookup; 
    int total_versions; 
    time_t last_change_time; 

    MyFile(const std::string& file_name) : version_lookup(101) {
        name = file_name;
        total_versions = 1;
        first_version = new TreeNode(0, ""); 
        first_version->msg = "empty file created";
        first_version->made_time = std::time(nullptr);
        first_version->snap_time = first_version->made_time; 
        first_version->dad = nullptr;
        
        version_lookup.add(0, first_version);
        current_version = first_version;
        last_change_time = first_version->made_time;
    }

    ~MyFile() {
        std::vector<TreeNode*> to_delete;
        to_delete.push_back(first_version);
      
        while (!to_delete.empty()) {
            TreeNode* node = to_delete.back(); 
            to_delete.pop_back();
            
            for (TreeNode* child : node->kids) {
                to_delete.push_back(child);
            }
            delete node;
        }
    }
    std::string format_time(time_t t) const {
        char buffer[64];
        if (t == 0) return "no time";
        struct tm* time_info = std::localtime(&t);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
        return std::string(buffer);
    }

    void show_content() {
        std::cout << "Content of '" << name << "' (Version " << current_version->id << "):" << std::endl;
        std::cout << current_version->data << std::endl;
    }


    void add_text(const std::string& text) {
        if (current_version->is_snap()) {

            TreeNode* new_version = new TreeNode(total_versions, current_version->data + text);
            new_version->dad = current_version;
            current_version->kids.push_back(new_version);
            
            std::cout << "New version " << total_versions << " created for '" << name << "'.";
            if (new_version->dad->id != (total_versions - 1)) {
                std::cout << " Parent is version " << new_version->dad->id << ".";
            }
            std::cout << std::endl;
            
            version_lookup.add(total_versions, new_version);
            current_version = new_version;
            total_versions++;  
            last_change_time = std::time(nullptr);
        } else {        
 
            current_version->data += text;
            last_change_time = std::time(nullptr);
            std::cout << "Added text to version " << current_version->id << " of '" << name << "'." << std::endl;
        }
    }
    void replace_content(const std::string& new_content) {
        if (current_version->is_snap()) {
            TreeNode* new_version = new TreeNode(total_versions, new_content);
            new_version->dad = current_version;
            current_version->kids.push_back(new_version);
            
            std::cout << "New version " << total_versions << " created for '" << name << "'.";
            if (new_version->dad->id != (total_versions - 1)) {
                std::cout << " Parent is version " << new_version->dad->id << ".";
            }
            std::cout << std::endl;
            
            version_lookup.add(total_versions, new_version);
            current_version = new_version;
            total_versions++;  
            last_change_time = std::time(nullptr);
        } else {
            current_version->data = new_content;
            last_change_time = std::time(nullptr);
            std::cout << "Updated version " << current_version->id << " of '" << name << "'." << std::endl;
        }
    }
    void make_snapshot(const std::string& message) {
        current_version->msg = message;
        current_version->snap_time = std::time(nullptr);
        std::cout << "Snapshot created for version " << current_version->id << " of '" << name << "'." << std::endl;
    }
    void go_back(int version_id = -1) {
        if (version_id == -1) {
            if (current_version->dad == nullptr) {
                std::cout << "Error: Cannot go back from first version." << std::endl;
                return;
            }
            current_version = current_version->dad;
            std::cout << "Switched to parent version " << current_version->id << " for '" << name << "'." << std::endl;
        } else {
            TreeNode* target = version_lookup.find(version_id);
            if (!target) {
                std::cout << "Error: Version " << version_id << " not found for file '" << name << "'." << std::endl;
                return;
            }
            current_version = target;
            std::cout << "Switched to version " << current_version->id << " for '" << name << "'." << std::endl;
        }
    }
    void show_history() {
        std::cout << "History for " << name << ":" << std::endl;
        TreeNode* node = current_version;
        while (node) {
            if (node->is_snap()) {
                std::cout << "Version " << node->id << ": " << format_time(node->snap_time) << " - " << node->msg << std::endl;
            }
            node = node->dad;
        }
    }
};

#endif