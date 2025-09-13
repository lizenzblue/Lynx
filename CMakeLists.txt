cmake_minimum_required(VERSION 3.16)
project(Lynx VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(include)

# Find required packages
find_package(PkgConfig REQUIRED)

# Source files (exclude plugin.cpp for now due to dependencies)


# Gather sources, but exclude nothing now
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add plugin.cpp and theme_manager.cpp explicitly (if not already included)
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/plugin.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/theme_manager.cpp")

# Create executable
add_executable(lynx ${SOURCES})

# Link libraries for dynamic loading
target_link_libraries(lynx ${CMAKE_DL_LIBS})

# Install targets
install(TARGETS lynx 
    RUNTIME DESTINATION bin
    COMPONENT Runtime)

# Install configuration files
install(FILES 
    config.sample
    aliases.sample
    DESTINATION share/lynx
    COMPONENT Configuration)

# Install themes
install(DIRECTORY themes/
    DESTINATION share/lynx/themes
    COMPONENT Configuration)

# Install documentation
install(FILES 
    README.md
    CUSTOMIZATION.md
    DEVELOPMENT.md
    DESTINATION share/doc/lynx
    COMPONENT Documentation)

# Package configuration
set(CPACK_PACKAGE_NAME "Lynx")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A highly customizable shell implementation built from scratch in C++")
set(CPACK_PACKAGE_VENDOR "Lynx Shell Project")
set(CPACK_PACKAGE_CONTACT "your-email@example.com")

# Platform-specific packaging
if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lynx Shell Project")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17)")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "System Environment/Shells")
elseif(APPLE)
    set(CPACK_GENERATOR "TGZ;ZIP")
endif()

include(CPack)
