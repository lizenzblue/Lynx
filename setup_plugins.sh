#!/bin/bash

# Lynx Shell Plugin and Theme Setup Script
# This script sets up the directory structure and installs example plugins/themes

set -e

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

# Configuration
LYNX_DIR="$HOME/.lynx"
PLUGINS_DIR="$LYNX_DIR/plugins"
THEMES_DIR="$LYNX_DIR/themes"
CONFIG_FILE="$LYNX_DIR/config"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Help function
show_help() {
    cat << EOF
Lynx Shell Plugin and Theme Setup

Usage: $0 [OPTIONS]

Options:
    -h, --help          Show this help message
    -i, --install       Install plugins and themes (default)
    -u, --uninstall     Remove plugins and themes
    -c, --clean         Clean installation (remove and reinstall)
    --plugins-only      Install only plugins
    --themes-only       Install only themes
    --setup-dirs        Setup directories only

This script will:
    1. Create ~/.lynx directory structure
    2. Install example plugins to ~/.lynx/plugins/
    3. Install example themes to ~/.lynx/themes/
    4. Create default configuration if needed

Examples:
    $0                  # Install everything
    $0 --plugins-only   # Install only plugins
    $0 --clean          # Clean install
    $0 --uninstall      # Remove installation
EOF
}

# Function to create directory structure
setup_directories() {
    print_status "Setting up Lynx directory structure..."
    
    mkdir -p "$LYNX_DIR"
    mkdir -p "$PLUGINS_DIR"
    mkdir -p "$THEMES_DIR"
    
    print_success "Created directories:"
    print_success "  $LYNX_DIR"
    print_success "  $PLUGINS_DIR"
    print_success "  $THEMES_DIR"
}

# Function to create default config
create_default_config() {
    if [ ! -f "$CONFIG_FILE" ]; then
        print_status "Creating default configuration..."
        
        cat > "$CONFIG_FILE" << EOF
# Lynx Shell Configuration File

# Theme settings
theme = default

# Plugin settings
plugins_enabled = true
plugin_autoload = true

# Shell settings
welcome_message = Welcome to Lynx Shell with Plugin Support!
show_welcome = true
history_size = 1000

# Prompt settings
show_exit_code = true
show_timestamp = false

# Plugin-specific settings
time_plugin_format = %H:%M:%S
fileutils_tree_depth = 3
EOF
        
        print_success "Created default configuration: $CONFIG_FILE"
    else
        print_status "Configuration file already exists: $CONFIG_FILE"
    fi
}

# Function to compile and install plugins
install_plugins() {
    print_status "Installing example plugins..."
    
    local plugins_src="$PROJECT_ROOT/examples/plugins"
    local build_script="$plugins_src/build_plugin.sh"
    
    if [ ! -f "$build_script" ]; then
        print_error "Build script not found: $build_script"
        return 1
    fi
    
    # Make build script executable
    chmod +x "$build_script"
    
    # Compile and install plugins
    local plugins=("time_plugin" "fileutils_plugin")
    
    for plugin in "${plugins[@]}"; do
        print_status "Compiling and installing $plugin..."
        
        cd "$plugins_src"
        if ./build_plugin.sh "$plugin"; then
            if ./build_plugin.sh --install "$plugin"; then
                print_success "Installed plugin: $plugin"
            else
                print_warning "Failed to install plugin: $plugin"
            fi
        else
            print_warning "Failed to compile plugin: $plugin"
        fi
    done
}

# Function to install themes
install_themes() {
    print_status "Installing example themes..."
    
    local themes_src="$PROJECT_ROOT/examples/themes"
    
    if [ ! -d "$themes_src" ]; then
        print_error "Themes directory not found: $themes_src"
        return 1
    fi
    
    # Copy theme files
    for theme_file in "$themes_src"/*.{theme,json,ini}; do
        if [ -f "$theme_file" ]; then
            local theme_name=$(basename "$theme_file")
            cp "$theme_file" "$THEMES_DIR/"
            print_success "Installed theme: $theme_name"
        fi
    done
}

# Function to uninstall
uninstall() {
    print_status "Uninstalling Lynx plugins and themes..."
    
    if [ -d "$PLUGINS_DIR" ]; then
        rm -rf "$PLUGINS_DIR"
        print_success "Removed plugins directory"
    fi
    
    if [ -d "$THEMES_DIR" ]; then
        rm -rf "$THEMES_DIR"
        print_success "Removed themes directory"
    fi
    
    # Keep config file but ask user
    if [ -f "$CONFIG_FILE" ]; then
        echo -n "Remove configuration file? [y/N]: "
        read -r response
        if [[ "$response" =~ ^[Yy]$ ]]; then
            rm "$CONFIG_FILE"
            print_success "Removed configuration file"
        else
            print_status "Kept configuration file"
        fi
    fi
    
    # Remove directory if empty
    if [ -d "$LYNX_DIR" ] && [ -z "$(ls -A "$LYNX_DIR")" ]; then
        rmdir "$LYNX_DIR"
        print_success "Removed empty Lynx directory"
    fi
}

# Function to show installation status
show_status() {
    print_status "Lynx Shell Plugin System Status:"
    echo ""
    
    if [ -d "$LYNX_DIR" ]; then
        print_success "Lynx directory: $LYNX_DIR"
        
        if [ -f "$CONFIG_FILE" ]; then
            print_success "Configuration file exists"
        else
            print_warning "Configuration file missing"
        fi
        
        if [ -d "$PLUGINS_DIR" ]; then
            local plugin_count=$(find "$PLUGINS_DIR" -name "*.so" -o -name "*.dylib" -o -name "*.dll" | wc -l)
            print_success "Plugins directory exists ($plugin_count plugins)"
            
            if [ "$plugin_count" -gt 0 ]; then
                echo "  Installed plugins:"
                for plugin in "$PLUGINS_DIR"/*{.so,.dylib,.dll}; do
                    if [ -f "$plugin" ]; then
                        echo "    $(basename "$plugin")"
                    fi
                done
            fi
        else
            print_warning "Plugins directory missing"
        fi
        
        if [ -d "$THEMES_DIR" ]; then
            local theme_count=$(find "$THEMES_DIR" -name "*.theme" -o -name "*.json" -o -name "*.ini" | wc -l)
            print_success "Themes directory exists ($theme_count themes)"
            
            if [ "$theme_count" -gt 0 ]; then
                echo "  Installed themes:"
                for theme in "$THEMES_DIR"/*{.theme,.json,.ini}; do
                    if [ -f "$theme" ]; then
                        echo "    $(basename "$theme")"
                    fi
                done
            fi
        else
            print_warning "Themes directory missing"
        fi
    else
        print_error "Lynx directory not found: $LYNX_DIR"
    fi
}

# Main installation function
install_all() {
    setup_directories
    create_default_config
    
    if [ "$PLUGINS_ONLY" != true ]; then
        install_themes
    fi
    
    if [ "$THEMES_ONLY" != true ]; then
        install_plugins
    fi
    
    echo ""
    print_success "Installation complete!"
    print_status "Restart your shell to load plugins and themes"
    print_status "Use 'plugins list' and 'themes list' to see available extensions"
}

# Main script logic
main() {
    local INSTALL=true
    local UNINSTALL=false
    local CLEAN=false
    local PLUGINS_ONLY=false
    local THEMES_ONLY=false
    local SETUP_DIRS_ONLY=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -i|--install)
                INSTALL=true
                shift
                ;;
            -u|--uninstall)
                UNINSTALL=true
                INSTALL=false
                shift
                ;;
            -c|--clean)
                CLEAN=true
                shift
                ;;
            --plugins-only)
                PLUGINS_ONLY=true
                shift
                ;;
            --themes-only)
                THEMES_ONLY=true
                shift
                ;;
            --setup-dirs)
                SETUP_DIRS_ONLY=true
                shift
                ;;
            --status)
                show_status
                exit 0
                ;;
            -*)
                print_error "Unknown option: $1"
                show_help
                exit 1
                ;;
            *)
                print_error "Unexpected argument: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    # Execute based on flags
    if [ "$UNINSTALL" = true ]; then
        uninstall
    elif [ "$CLEAN" = true ]; then
        print_status "Performing clean installation..."
        uninstall
        install_all
    elif [ "$SETUP_DIRS_ONLY" = true ]; then
        setup_directories
        create_default_config
    elif [ "$INSTALL" = true ]; then
        install_all
    fi
}

main "$@"