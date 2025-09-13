#include "utils.h"
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <pwd.h>
#include <limits.h>
#include <iostream>

std::string Utils::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string Utils::join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) {
        return "";
    }
    
    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }
    
    return result;
}

std::string Utils::getCurrentDirectory() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    }
    return "";
}

bool Utils::changeDirectory(const std::string& path) {
    return chdir(path.c_str()) == 0;
}

std::string Utils::getEnvVar(const std::string& name) {
    const char* value = getenv(name.c_str());
    return value ? std::string(value) : "";
}

void Utils::setEnvVar(const std::string& name, const std::string& value) {
    setenv(name.c_str(), value.c_str(), 1);
}

void Utils::clearScreen() {
    // ANSI escape sequence to clear screen
    std::cout << "\033[2J\033[H" << std::flush;
}

std::string Utils::getUsername() {
    const char* username = getenv("USER");
    if (username) {
        return std::string(username);
    }
    
    // Fallback to getpwuid
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        return std::string(pw->pw_name);
    }
    
    return "unknown";
}

std::string Utils::getHostname() {
    char hostname[256];  // Use a reasonable fixed size instead of HOST_NAME_MAX
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    return "localhost";
}

std::string Utils::getHomeDirectory() {
    const char* home = std::getenv("HOME");
    if (home) {
        return std::string(home);
    }
    
    // Fallback to getpwuid
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        return std::string(pw->pw_dir);
    }
    
    return "/tmp";  // Last resort fallback
}

std::string Utils::expandPath(const std::string& path) {
    if (path.empty()) {
        return path;
    }
    
    if (path[0] == '~') {
        if (path.length() == 1 || path[1] == '/') {
            return getHomeDirectory() + path.substr(1);
        }
    }
    
    return path;
}
