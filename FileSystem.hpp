// FileSystem.hpp
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "HashMap.hpp"
#include "Heap.hpp"
#include "TreeNode.hpp"
#include "File.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

class MyFileSystem {
private:
    std::vector<MyFile*> all_files; 
    MyFile* find_file(const std::string& name) {
        for (MyFile* f : all_files) {
            if (f->name == name) {
                return f;
            }
        }
        return nullptr;
    }
    std::string format_time(time_t t) const {
        char buffer[64]; 
        if (t == 0) return "no time";
        struct tm* time_info = std::localtime(&t);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
        return std::string(buffer);
    }

public:
    MyFileSystem() : all_files() {}
    ~MyFileSystem() { 
        for (MyFile* f : all_files) {
            delete f; 
        }
    }
    void run_command(const std::string& line) {
        if (line.empty()) return;
        
        std::istringstream input(line);
        std::string command; 
        input >> command;
        
        if (command == "CREATE") {
            std::string filename; 
            if (!(input >> filename)) { 
                std::cout << "Error: Need filename. Use: CREATE <filename>" << std::endl; 
                return; 
            }
            if (find_file(filename)) { 
                std::cout << "Error: File '" << filename << "' already exists." << std::endl; 
                return; 
            }
            MyFile* new_file = new MyFile(filename);
            all_files.push_back(new_file);
            std::cout << "File '" << filename << "' created with snapshot version 0." << std::endl;
            
        } else if (command == "READ") {
            std::string filename;
            if (!(input >> filename)) {
                std::cout << "Error: Need filename. Use: READ <filename>" << std::endl; 
                return;
            }
            MyFile* file = find_file(filename);
            if (!file) {
                std::cout << "Error: File '" << filename << "' not found." << std::endl;
                return;
            }
            file->show_content();
            
        } else if (command == "INSERT") {
            std::string filename;
            if (!(input >> filename)) {
                std::cout << "Error: Need filename and text. Use: INSERT <filename> <text>" << std::endl; 
                return;
            }
            MyFile* file = find_file(filename);
            if (!file) {
                std::cout << "Error: File '" << filename << "' not found." << std::endl;
                return;
            }
            std::string text;
            std::getline(input, text);
            if (text.empty()) {
                std::cout << "Error: Need text to insert. Use: INSERT <filename> <text>" << std::endl; 
                return;
            }
            // remove space at beginning
            if (!text.empty() && text[0] == ' ') {
                text = text.substr(1);
            }
            file->add_text(text);
            
        } else if (command == "UPDATE") {
            std::string filename;
            if (!(input >> filename)) {
                std::cout << "Error: Need filename and content. Use: UPDATE <filename> <content>" << std::endl; 
                return;
            }
            MyFile* file = find_file(filename); 
            if (!file) { 
                std::cout << "Error: File '" << filename << "' not found." << std::endl; 
                return; 
            }
            std::string content;
            std::getline(input, content);
            if (content.empty()) {
                std::cout << "Error: Need content. Use: UPDATE <filename> <content>" << std::endl; 
                return; 
            }
            if (!content.empty() && content[0] == ' ') {
                content = content.substr(1);
            }
            file->replace_content(content);
            
        } else if (command == "SNAPSHOT") {
            std::string filename;
            if (!(input >> filename)) {
                std::cout << "Error: Need filename and message. Use: SNAPSHOT <filename> <message>" << std::endl; 
                return;
            }
            MyFile* file = find_file(filename); 
            if (!file) { 
                std::cout << "Error: File '" << filename << "' not found." << std::endl; 
                return; 
            }
            std::string message;
            std::getline(input, message);
            if (message.empty()) { 
                std::cout << "Error: Need message. Use: SNAPSHOT <filename> <message>" << std::endl; 
                return; 
            }
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);
            }
            file->make_snapshot(message);
            
        } else if (command == "ROLLBACK") {
            std::string filename;
            if (!(input >> filename)) {
                std::cout << "Error: Need filename. Use: ROLLBACK <filename> [version]" << std::endl; 
                return;
            }
            MyFile* file = find_file(filename); 
            if (!file) { 
                std::cout << "Error: File '" << filename << "' not found." << std::endl; 
                return; 
            }
            std::string version_str; 
            if (!(input >> version_str)) {
                file->go_back();
            } else {
                bool is_number = true; 
                for (char c : version_str) {
                    if (!std::isdigit((unsigned char)c)) { 
                        is_number = false; 
                        break; 
                    }
                }
                if (!is_number) {
                    std::cout << "Error: Version must be a number. Use: ROLLBACK <filename> [version]" << std::endl; 
                    return; 
                }
                int version_num = std::stoi(version_str);
                file->go_back(version_num);
            }
            
        } else if (command == "HISTORY") {
            std::string filename;
            if (!(input >> filename)) {
                std::cout << "Error: Need filename. Use: HISTORY <filename>" << std::endl; 
                return;
            }
            MyFile* file = find_file(filename);
            if (!file) { 
                std::cout << "Error: File '" << filename << "' not found." << std::endl; 
                return; 
            }
            file->show_history();
            
        } else if (command == "RECENT_FILES") {
            std::string num_str; 
            if (!(input >> num_str)) {
                std::cout << "Error: Need number. Use: RECENT_FILES <number>" << std::endl; 
                return; 
            }
            bool is_number = true;
            for (char c : num_str) {
                if (!std::isdigit((unsigned char)c)) {
                    is_number = false;
                    break;
                }
            }           
            if (!is_number) { 
                std::cout << "Error: Must be a number. Use: RECENT_FILES <number>" << std::endl; 
                return; 
            }
            
            int how_many = std::stoi(num_str);
            if (how_many <= 0) { 
                return; 
            }
            struct FileInfo { 
                std::string name; 
                time_t time; 
            };
            struct TimeCompare { 
                bool operator()(const FileInfo& a, const FileInfo& b) const {
                    return a.time > b.time; 
                } 
            };         
            
            MyHeap<FileInfo, TimeCompare> recent_heap;
            for (MyFile* f : all_files) {
                recent_heap.insert(FileInfo{f->name, f->last_change_time});
            }
            
            int count = 0;
            while (!recent_heap.is_empty() && count < how_many) {
                FileInfo file_info = recent_heap.peek(); 
                recent_heap.remove_top();
                std::cout << file_info.name << " (" << format_time(file_info.time) << ")" << std::endl;
                count++;
            }
            
        } else if (command == "BIGGEST_TREES") {
            std::string num_str; 
            if (!(input >> num_str)) {
                std::cout << "Error: Need number. Use: BIGGEST_TREES <number>" << std::endl; 
                return; 
            }
            bool is_number = true;
            for (char c : num_str) {
                if (!std::isdigit((unsigned char)c)) {
                    is_number = false;
                    break;
                }
            }           
            if (!is_number) { 
                std::cout << "Error: Must be a number. Use: BIGGEST_TREES <number>" << std::endl; 
                return; 
            }
            
            int how_many = std::stoi(num_str);
            if (how_many <= 0) { 
                return; 
            }
            struct FileInfo { 
                std::string name; 
                int versions; 
            };
            struct VersionCompare { 
                bool operator()(const FileInfo& a, const FileInfo& b) const { 
                    return a.versions > b.versions;
                } 
            };
            
            MyHeap<FileInfo, VersionCompare> version_heap;
            for (MyFile* f : all_files) {
                version_heap.insert(FileInfo{f->name, f->total_versions});
            }
            
            int count = 0;
            while (!version_heap.is_empty() && count < how_many) {
                FileInfo file_info = version_heap.peek();
                version_heap.remove_top();
                std::cout << file_info.name << " (" << file_info.versions << " versions)" << std::endl;
                count++;
            }
            
        } else {
            std::cout << "Error: Don't know command '" << command << "'." << std::endl;
        }
    }
};

#endif