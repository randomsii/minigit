// main.cpp
#include <iostream>
#include <string>
#include "FileSystem.hpp"

int main() {
    MyFileSystem system; 
    std::string user_input;
    
    while (std::getline(std::cin, user_input)) {
        if (user_input.size() == 0) { 
            std::cout << std::endl;
            continue; 
        }
        system.run_command(user_input); 
    }   
    return 0;
}