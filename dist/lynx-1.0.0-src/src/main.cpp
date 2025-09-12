#include "shell.h"
#include <iostream>

int main() {
    std::cout << "Welcome to Lynx Shell!" << std::endl;
    std::cout << "Type 'help' for available commands or 'exit' to quit." << std::endl;
    std::cout << std::endl;
    
    Shell shell;
    shell.run();
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
