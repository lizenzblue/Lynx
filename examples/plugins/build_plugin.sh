#!/bin/bash

# Lynx Plugin Development Kit
# Script to compile plugins for Lynx Shell

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LYNX_ROOT="$(dirname "$SCRIPT_DIR")"
PLUGIN_DIR="$SCRIPT_DIR"
BUILD_DIR="$SCRIPT_DIR/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Help function
show_help() {
    cat << EOF
Lynx Plugin Development Kit

Usage: $0 [OPTIONS] [PLUGIN_NAME]

Options:
    -h, --help      Show this help message
    -c, --clean     Clean build directory
    -a, --all       Compile all plugins
    -l, --list      List available plugins
    -i, --install   Install plugin to ~/.lynx/plugins/
    -u, --uninstall Remove plugin from ~/.lynx/plugins/

Examples:
    $0 time_plugin              # Compile time_plugin.cpp
    $0 --all                    # Compile all plugins
    $0 --install time_plugin    # Compile and install time_plugin
    $0 --clean                  # Clean build directory

Plugin Development:
    1. Create your plugin .cpp file in this directory
    2. Include the plugin.h header: #include "../../include/plugin.h"
    3. Implement the IPlugin interface
    4. Use LYNX_PLUGIN_ENTRY_POINT(YourPluginClass) macro
    5. Compile with: $0 your_plugin_name
EOF
}

# Function to create build directory
create_build_dir() {
    if [ ! -d "$BUILD_DIR" ]; then
        print_status "Creating build directory..."
        mkdir -p "$BUILD_DIR"
    fi
}

# Function to clean build directory
clean_build() {
    print_status "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
    print_success "Build directory cleaned"
}

# Function to list available plugins
list_plugins() {
    print_status "Available plugins:"
    for plugin in "$PLUGIN_DIR"/*.cpp; do
        if [ -f "$plugin" ]; then
            basename "$plugin" .cpp
        fi
    done
}

# Function to compile a plugin
compile_plugin() {
    local plugin_name="$1"
    local plugin_file="$PLUGIN_DIR/${plugin_name}.cpp"
    
    if [ ! -f "$plugin_file" ]; then
        print_error "Plugin file not found: $plugin_file"
        return 1
    fi
    
    create_build_dir
    
    local output_file="$BUILD_DIR/lib${plugin_name}.so"
    
    print_status "Compiling plugin: $plugin_name"
    print_status "Source: $plugin_file"
    print_status "Output: $output_file"
    
    # Compilation flags
    local flags=(
        -std=c++17
        -fPIC
        -shared
        -O2
        -Wall
        -Wextra
        -I"$LYNX_ROOT/include"
        -I"$LYNX_ROOT/src"
    )
    
    # Libraries to link
    local libs=(
        -ldl
        -lstdc++fs
    )
    
    # Compile the plugin
    if g++ "${flags[@]}" "$plugin_file" "${libs[@]}" -o "$output_file"; then
        print_success "Plugin compiled successfully: $output_file"
        return 0
    else
        print_error "Failed to compile plugin: $plugin_name"
        return 1
    fi
}

# Function to install plugin
install_plugin() {
    local plugin_name="$1"
    local plugin_file="$BUILD_DIR/lib${plugin_name}.so"
    
    if [ ! -f "$plugin_file" ]; then
        print_warning "Plugin not compiled, compiling first..."
        if ! compile_plugin "$plugin_name"; then
            return 1
        fi
    fi
    
    local install_dir="$HOME/.lynx/plugins"
    mkdir -p "$install_dir"
    
    print_status "Installing plugin to: $install_dir"
    cp "$plugin_file" "$install_dir/"
    
    print_success "Plugin installed: $install_dir/lib${plugin_name}.so"
    print_status "Restart Lynx shell to load the plugin"
}

# Function to uninstall plugin
uninstall_plugin() {
    local plugin_name="$1"
    local plugin_file="$HOME/.lynx/plugins/lib${plugin_name}.so"
    
    if [ -f "$plugin_file" ]; then
        rm "$plugin_file"
        print_success "Plugin uninstalled: lib${plugin_name}.so"
    else
        print_warning "Plugin not found in ~/.lynx/plugins/: lib${plugin_name}.so"
    fi
}

# Function to compile all plugins
compile_all() {
    print_status "Compiling all plugins..."
    
    local success_count=0
    local total_count=0
    
    for plugin in "$PLUGIN_DIR"/*.cpp; do
        if [ -f "$plugin" ]; then
            local plugin_name=$(basename "$plugin" .cpp)
            total_count=$((total_count + 1))
            
            echo ""
            if compile_plugin "$plugin_name"; then
                success_count=$((success_count + 1))
            fi
        fi
    done
    
    echo ""
    print_status "Compilation complete: $success_count/$total_count plugins compiled successfully"
}

# Main script logic
main() {
    local clean_flag=false
    local all_flag=false
    local list_flag=false
    local install_flag=false
    local uninstall_flag=false
    local plugin_name=""
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -c|--clean)
                clean_flag=true
                shift
                ;;
            -a|--all)
                all_flag=true
                shift
                ;;
            -l|--list)
                list_flag=true
                shift
                ;;
            -i|--install)
                install_flag=true
                shift
                ;;
            -u|--uninstall)
                uninstall_flag=true
                shift
                ;;
            -*)
                print_error "Unknown option: $1"
                show_help
                exit 1
                ;;
            *)
                plugin_name="$1"
                shift
                ;;
        esac
    done
    
    # Execute based on flags
    if [ "$clean_flag" = true ]; then
        clean_build
    elif [ "$list_flag" = true ]; then
        list_plugins
    elif [ "$all_flag" = true ]; then
        compile_all
    elif [ "$install_flag" = true ]; then
        if [ -z "$plugin_name" ]; then
            print_error "Plugin name required for installation"
            exit 1
        fi
        install_plugin "$plugin_name"
    elif [ "$uninstall_flag" = true ]; then
        if [ -z "$plugin_name" ]; then
            print_error "Plugin name required for uninstallation"
            exit 1
        fi
        uninstall_plugin "$plugin_name"
    elif [ -n "$plugin_name" ]; then
        compile_plugin "$plugin_name"
    else
        show_help
    fi
}

main "$@"