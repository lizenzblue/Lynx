#include "version.h"
#include <iostream>
#include <sstream>

namespace Version {
    const std::string MAJOR = "1";
    const std::string MINOR = "0";
    const std::string PATCH = "0";
    const std::string VERSION = MAJOR + "." + MINOR + "." + PATCH;
    const std::string BUILD_DATE = __DATE__ " " __TIME__;
    
    #ifdef __linux__
        const std::string BUILD_PLATFORM = "Linux";
    #elif __APPLE__
        const std::string BUILD_PLATFORM = "macOS";
    #elif __FreeBSD__
        const std::string BUILD_PLATFORM = "FreeBSD";
    #else
        const std::string BUILD_PLATFORM = "Unknown";
    #endif
    
    std::string getVersionString() {
        return VERSION;
    }
    
    std::string getFullVersionString() {
        std::stringstream ss;
        ss << "Lynx Shell v" << VERSION;
        ss << " (" << BUILD_PLATFORM << ")";
        ss << " - Built on " << BUILD_DATE;
        return ss.str();
    }
    
    void printVersionInfo() {
        std::cout << getFullVersionString() << std::endl;
        std::cout << "A highly customizable shell built from scratch in C++" << std::endl;
        std::cout << "Homepage: https://github.com/YOUR_USERNAME/Lynx" << std::endl;
    }
}