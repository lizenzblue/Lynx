#ifndef VERSION_H
#define VERSION_H

#include <string>

namespace Version {
    extern const std::string MAJOR;
    extern const std::string MINOR;
    extern const std::string PATCH;
    extern const std::string VERSION;
    extern const std::string BUILD_DATE;
    extern const std::string BUILD_PLATFORM;
    
    std::string getVersionString();
    std::string getFullVersionString();
    void printVersionInfo();
}

#endif // VERSION_H