#!/bin/bash

# Lynx Shell Uninstaller
# Usage: ./uninstall.sh [--preserve-config]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
INSTALL_PATHS=(
    "/usr/local/bin/lynx"
    "$HOME/.local/bin/lynx"
)
CONFIG_DIR="$HOME/.lynx"
PRESERVE_CONFIG=false

# Functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Parse command line arguments
parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --preserve-config)
                PRESERVE_CONFIG=true
                shift
                ;;
            -h|--help)
                show_help
                exit 0
                ;;
            *)
                log_error "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

show_help() {
    echo "Lynx Shell Uninstaller"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --preserve-config    Keep configuration files in ~/.lynx/"
    echo "  -h, --help          Show this help message"
    echo ""
}

# Remove binary files
remove_binaries() {
    log_info "Removing Lynx Shell binaries..."
    
    for path in "${INSTALL_PATHS[@]}"; do
        if [[ -f "$path" ]]; then
            if [[ "$path" == "/usr/local/bin/lynx" ]]; then
                log_info "Removing system installation: $path"
                sudo rm -f "$path"
                log_success "Removed $path"
            else
                log_info "Removing user installation: $path"
                rm -f "$path"
                log_success "Removed $path"
            fi
        fi
    done
}

# Remove system files
remove_system_files() {
    log_info "Removing system files..."
    
    # Remove system-wide files
    if [[ -d "/usr/local/share/lynx" ]]; then
        log_info "Removing /usr/local/share/lynx"
        sudo rm -rf "/usr/local/share/lynx"
        log_success "Removed system share files"
    fi
    
    if [[ -d "/usr/local/share/doc/lynx" ]]; then
        log_info "Removing /usr/local/share/doc/lynx"
        sudo rm -rf "/usr/local/share/doc/lynx"
        log_success "Removed system documentation"
    fi
}

# Remove configuration files
remove_config() {
    if [[ "$PRESERVE_CONFIG" == "false" ]]; then
        if [[ -d "$CONFIG_DIR" ]]; then
            log_warning "Removing configuration directory: $CONFIG_DIR"
            echo -n "Are you sure? This will delete all your customizations [y/N]: "
            read -r response
            if [[ "$response" =~ ^[Yy]$ ]]; then
                rm -rf "$CONFIG_DIR"
                log_success "Removed configuration directory"
            else
                log_info "Configuration directory preserved"
            fi
        fi
    else
        log_info "Configuration directory preserved at: $CONFIG_DIR"
    fi
}

# Clean PATH entries
clean_path() {
    log_info "Checking shell configuration files..."
    
    SHELL_FILES=(
        "$HOME/.bashrc"
        "$HOME/.zshrc"
        "$HOME/.config/fish/config.fish"
    )
    
    for file in "${SHELL_FILES[@]}"; do
        if [[ -f "$file" ]]; then
            if grep -q "Lynx Shell" "$file" 2>/dev/null; then
                log_info "Found Lynx Shell entries in $file"
                echo -n "Remove PATH entries from $file? [y/N]: "
                read -r response
                if [[ "$response" =~ ^[Yy]$ ]]; then
                    # Create backup
                    cp "$file" "$file.bak.$(date +%Y%m%d)"
                    # Remove Lynx Shell section
                    sed -i.tmp '/# Lynx Shell/,+1d' "$file" 2>/dev/null || true
                    rm -f "$file.tmp" 2>/dev/null || true
                    log_success "Removed entries from $file (backup created)"
                fi
            fi
        fi
    done
}

# Check if Lynx is running
check_running() {
    if pgrep -f "lynx" > /dev/null 2>&1; then
        log_warning "Lynx Shell appears to be running"
        log_warning "Please close all Lynx Shell instances before uninstalling"
        echo -n "Continue anyway? [y/N]: "
        read -r response
        if [[ ! "$response" =~ ^[Yy]$ ]]; then
            log_info "Uninstallation cancelled"
            exit 0
        fi
    fi
}

# Main uninstallation process
main() {
    echo -e "${RED}"
    echo "╔══════════════════════════════════════╗"
    echo "║         Lynx Shell Uninstaller       ║"
    echo "╚══════════════════════════════════════╝"
    echo -e "${NC}"

    parse_args "$@"
    
    log_warning "This will remove Lynx Shell from your system"
    if [[ "$PRESERVE_CONFIG" == "false" ]]; then
        log_warning "Configuration files will also be removed unless --preserve-config is used"
    fi
    
    echo -n "Continue with uninstallation? [y/N]: "
    read -r response
    if [[ ! "$response" =~ ^[Yy]$ ]]; then
        log_info "Uninstallation cancelled"
        exit 0
    fi

    check_running
    remove_binaries
    remove_system_files
    clean_path
    remove_config

    echo ""
    log_success "Lynx Shell has been uninstalled!"
    
    if [[ "$PRESERVE_CONFIG" == "true" ]]; then
        echo ""
        log_info "Configuration files preserved in: $CONFIG_DIR"
        log_info "Remove them manually if desired: rm -rf $CONFIG_DIR"
    fi
    
    echo ""
    log_info "Thank you for using Lynx Shell!"
    echo ""
}

# Run main uninstallation
main "$@"