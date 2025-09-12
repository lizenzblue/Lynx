#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    // String utilities
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    
    // File system utilities
    std::string getCurrentDirectory();
    bool changeDirectory(const std::string& path);
    
    // Environment utilities
    std::string getEnvVar(const std::string& name);
    void setEnvVar(const std::string& name, const std::string& value);
    
    // Terminal utilities
    void clearScreen();
    std::string getUsername();
    std::string getHostname();
}

#endif // UTILS_H
