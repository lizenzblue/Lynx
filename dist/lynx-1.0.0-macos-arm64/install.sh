#!/bin/bash

# Lynx Shell Installation Script
# Usage: curl -sSL https://raw.githubusercontent.com/YOUR_USERNAME/Lynx/main/install.sh | bash

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
REPO_URL="https://github.com/YOUR_USERNAME/Lynx"
INSTALL_DIR="$HOME/.local/bin"
CONFIG_DIR="$HOME/.lynx"
TEMP_DIR="/tmp/lynx-install"

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

# Detect OS and architecture
detect_platform() {
    case "$(uname -s)" in
        Linux*)
            PLATFORM="linux"
            ;;
        Darwin*)
            PLATFORM="macos"
            ;;
        *)
            log_error "Unsupported operating system: $(uname -s)"
            exit 1
            ;;
    esac

    case "$(uname -m)" in
        x86_64|amd64)
            ARCH="x64"
            ;;
        arm64|aarch64)
            ARCH="arm64"
            ;;
        *)
            log_error "Unsupported architecture: $(uname -m)"
            exit 1
            ;;
    esac

    log_info "Detected platform: $PLATFORM-$ARCH"
}

# Check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Install dependencies
install_dependencies() {
    log_info "Checking dependencies..."

    if [[ "$PLATFORM" == "linux" ]]; then
        if ! command_exists "g++" || ! command_exists "make"; then
            log_info "Installing build dependencies..."
            if command_exists "apt-get"; then
                sudo apt-get update
                sudo apt-get install -y build-essential cmake
            elif command_exists "yum"; then
                sudo yum groupinstall -y "Development Tools"
                sudo yum install -y cmake
            elif command_exists "pacman"; then
                sudo pacman -S --noconfirm base-devel cmake
            else
                log_error "Unable to install dependencies automatically. Please install g++, make, and cmake manually."
                exit 1
            fi
        fi
    elif [[ "$PLATFORM" == "macos" ]]; then
        if ! command_exists "g++" || ! command_exists "make"; then
            if command_exists "brew"; then
                log_info "Installing dependencies via Homebrew..."
                brew install cmake
            else
                log_warning "Homebrew not found. Please install Xcode Command Line Tools:"
                log_warning "xcode-select --install"
                exit 1
            fi
        fi
    fi
}

# Download latest release or build from source
install_lynx() {
    log_info "Installing Lynx Shell..."

    # Create installation directory
    mkdir -p "$INSTALL_DIR"
    mkdir -p "$CONFIG_DIR"
    mkdir -p "$TEMP_DIR"

    cd "$TEMP_DIR"

    # Try to download prebuilt binary first
    RELEASE_URL="$REPO_URL/releases/latest/download/lynx-$PLATFORM-$ARCH.tar.gz"
    
    if curl -sSL --head "$RELEASE_URL" | head -n 1 | grep -q "200 OK"; then
        log_info "Downloading prebuilt binary..."
        curl -sSL "$RELEASE_URL" -o "lynx.tar.gz"
        tar -xzf "lynx.tar.gz"
        cp lynx "$INSTALL_DIR/"
        
        # Copy configuration files
        [[ -f config.sample ]] && cp config.sample "$CONFIG_DIR/"
        [[ -f aliases.sample ]] && cp aliases.sample "$CONFIG_DIR/"
        [[ -d themes ]] && cp -r themes "$CONFIG_DIR/"
        
        log_success "Installed prebuilt binary"
    else
        log_info "Prebuilt binary not available, building from source..."
        build_from_source
    fi
}

# Build from source
build_from_source() {
    log_info "Cloning repository..."
    git clone "$REPO_URL" lynx-source
    cd lynx-source

    log_info "Building Lynx Shell..."
    make release

    # Install binary
    cp build/lynx "$INSTALL_DIR/"

    # Copy configuration files
    cp config.sample "$CONFIG_DIR/"
    cp aliases.sample "$CONFIG_DIR/"
    cp -r themes "$CONFIG_DIR/"

    log_success "Built and installed from source"
}

# Setup shell integration
setup_shell_integration() {
    log_info "Setting up shell integration..."

    # Add to PATH if not already there
    if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
        SHELL_RC=""
        case "$SHELL" in
            */bash)
                SHELL_RC="$HOME/.bashrc"
                ;;
            */zsh)
                SHELL_RC="$HOME/.zshrc"
                ;;
            */fish)
                SHELL_RC="$HOME/.config/fish/config.fish"
                ;;
        esac

        if [[ -n "$SHELL_RC" ]]; then
            echo "" >> "$SHELL_RC"
            echo "# Lynx Shell" >> "$SHELL_RC"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$SHELL_RC"
            log_success "Added $INSTALL_DIR to PATH in $SHELL_RC"
        else
            log_warning "Unknown shell. Please add $INSTALL_DIR to your PATH manually."
        fi
    fi

    # Make binary executable
    chmod +x "$INSTALL_DIR/lynx"
}

# Setup default configuration
setup_configuration() {
    log_info "Setting up default configuration..."

    # Copy sample files if they don't exist
    [[ ! -f "$CONFIG_DIR/config" && -f "$CONFIG_DIR/config.sample" ]] && cp "$CONFIG_DIR/config.sample" "$CONFIG_DIR/config"
    [[ ! -f "$CONFIG_DIR/aliases" && -f "$CONFIG_DIR/aliases.sample" ]] && cp "$CONFIG_DIR/aliases.sample" "$CONFIG_DIR/aliases"

    log_success "Configuration setup complete"
}

# Cleanup
cleanup() {
    log_info "Cleaning up..."
    rm -rf "$TEMP_DIR"
}

# Main installation process
main() {
    echo -e "${BLUE}"
    echo "╔══════════════════════════════════════╗"
    echo "║           Lynx Shell Installer       ║"
    echo "║     A Customizable C++ Shell        ║"
    echo "╚══════════════════════════════════════╝"
    echo -e "${NC}"

    detect_platform
    install_dependencies
    install_lynx
    setup_shell_integration
    setup_configuration
    cleanup

    echo ""
    log_success "Lynx Shell installation complete!"
    echo ""
    echo -e "${GREEN}To get started:${NC}"
    echo "1. Restart your terminal or run: source ~/.bashrc (or ~/.zshrc)"
    echo "2. Run: lynx"
    echo "3. Type 'help' to see available commands"
    echo "4. Customize with: theme dark, alias ll 'ls -la', etc."
    echo ""
    echo -e "${BLUE}Configuration files are located in: $CONFIG_DIR${NC}"
    echo -e "${BLUE}Documentation: $REPO_URL${NC}"
    echo ""
}

# Error handling
trap cleanup EXIT

# Check if running as root
if [[ $EUID -eq 0 ]]; then
    log_error "This script should not be run as root"
    exit 1
fi

# Run main installation
main "$@"