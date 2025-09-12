#!/bin/bash

# Lynx Shell Release Packaging Script
# Creates distribution packages for different platforms

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
PROJECT_NAME="lynx"
VERSION=$(grep "project(Lynx VERSION" CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/')
BUILD_DIR="build"
DIST_DIR="dist"
RELEASE_DIR="release"

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

# Detect platform
detect_platform() {
    case "$(uname -s)" in
        Linux*)
            PLATFORM="linux"
            ;;
        Darwin*)
            PLATFORM="macos"
            ;;
        *)
            log_error "Unsupported platform: $(uname -s)"
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

    PLATFORM_ARCH="${PLATFORM}-${ARCH}"
    log_info "Building for: $PLATFORM_ARCH"
}

# Clean previous builds
clean_build() {
    log_info "Cleaning previous builds..."
    rm -rf "$BUILD_DIR" "$DIST_DIR" "$RELEASE_DIR"
    mkdir -p "$DIST_DIR" "$RELEASE_DIR"
}

# Build binary
build_binary() {
    log_info "Building Lynx Shell v$VERSION..."
    
    # Build with Make (faster for simple builds)
    make clean
    make release
    
    if [[ ! -f "$BUILD_DIR/$PROJECT_NAME" ]]; then
        log_error "Build failed - binary not found"
        exit 1
    fi
    
    log_success "Build completed successfully"
}

# Create binary package
create_binary_package() {
    log_info "Creating binary package..."
    
    PACKAGE_NAME="${PROJECT_NAME}-${VERSION}-${PLATFORM_ARCH}"
    PACKAGE_DIR="$DIST_DIR/$PACKAGE_NAME"
    
    mkdir -p "$PACKAGE_DIR"
    
    # Copy binary
    cp "$BUILD_DIR/$PROJECT_NAME" "$PACKAGE_DIR/"
    
    # Copy documentation
    cp README.md CUSTOMIZATION.md DEVELOPMENT.md "$PACKAGE_DIR/"
    
    # Copy configuration files
    cp config.sample "$PACKAGE_DIR/"
    cp aliases.sample "$PACKAGE_DIR/"
    cp -r themes "$PACKAGE_DIR/"
    
    # Copy scripts
    cp install.sh uninstall.sh "$PACKAGE_DIR/"
    
    # Create archive
    cd "$DIST_DIR"
    tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME/"
    zip -r "${PACKAGE_NAME}.zip" "$PACKAGE_NAME/" > /dev/null
    cd ..
    
    mv "$DIST_DIR/${PACKAGE_NAME}.tar.gz" "$RELEASE_DIR/"
    mv "$DIST_DIR/${PACKAGE_NAME}.zip" "$RELEASE_DIR/"
    
    log_success "Binary package created: ${PACKAGE_NAME}.tar.gz"
    log_success "Binary package created: ${PACKAGE_NAME}.zip"
}

# Create source package
create_source_package() {
    log_info "Creating source package..."
    
    SOURCE_NAME="${PROJECT_NAME}-${VERSION}-src"
    SOURCE_DIR="$DIST_DIR/$SOURCE_NAME"
    
    mkdir -p "$SOURCE_DIR"
    
    # Copy source files
    cp -r src include "$SOURCE_DIR/"
    cp CMakeLists.txt Makefile "$SOURCE_DIR/"
    cp README.md CUSTOMIZATION.md DEVELOPMENT.md "$SOURCE_DIR/"
    cp config.sample aliases.sample "$SOURCE_DIR/"
    cp -r themes "$SOURCE_DIR/"
    cp install.sh uninstall.sh "$SOURCE_DIR/"
    cp .gitignore "$SOURCE_DIR/"
    
    # Create archive
    cd "$DIST_DIR"
    tar -czf "${SOURCE_NAME}.tar.gz" "$SOURCE_NAME/"
    zip -r "${SOURCE_NAME}.zip" "$SOURCE_NAME/" > /dev/null
    cd ..
    
    mv "$DIST_DIR/${SOURCE_NAME}.tar.gz" "$RELEASE_DIR/"
    mv "$DIST_DIR/${SOURCE_NAME}.zip" "$RELEASE_DIR/"
    
    log_success "Source package created: ${SOURCE_NAME}.tar.gz"
    log_success "Source package created: ${SOURCE_NAME}.zip"
}

# Build with CMake for packaging
build_cmake_packages() {
    log_info "Building CMake packages..."
    
    mkdir -p cmake-build
    cd cmake-build
    
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make
    
    # Create packages
    if command -v cpack >/dev/null 2>&1; then
        cpack
        
        # Move packages to release directory
        mv *.tar.gz *.deb *.rpm *.zip "../$RELEASE_DIR/" 2>/dev/null || true
        
        log_success "CMake packages created"
    else
        log_warning "CPack not available, skipping CMake packages"
    fi
    
    cd ..
    rm -rf cmake-build
}

# Create checksums
create_checksums() {
    log_info "Creating checksums..."
    
    cd "$RELEASE_DIR"
    
    if command -v sha256sum >/dev/null 2>&1; then
        sha256sum *.tar.gz *.zip > SHA256SUMS 2>/dev/null || true
    elif command -v shasum >/dev/null 2>&1; then
        shasum -a 256 *.tar.gz *.zip > SHA256SUMS 2>/dev/null || true
    fi
    
    cd ..
    
    log_success "Checksums created"
}

# Generate release notes
generate_release_notes() {
    log_info "Generating release notes..."
    
    cat > "$RELEASE_DIR/RELEASE_NOTES.md" << EOF
# Lynx Shell v$VERSION Release

## What's New
- Highly customizable shell with theme support
- Command aliases and runtime configuration
- Multiple installation methods
- Cross-platform support (Linux, macOS)

## Installation

### Quick Install (Recommended)
\`\`\`bash
curl -sSL https://raw.githubusercontent.com/YOUR_USERNAME/Lynx/main/install.sh | bash
\`\`\`

### Manual Install
1. Download the appropriate package for your platform
2. Extract: \`tar -xzf lynx-${VERSION}-${PLATFORM_ARCH}.tar.gz\`
3. Run: \`./install.sh\`

### Build from Source
1. Download: \`lynx-${VERSION}-src.tar.gz\`
2. Extract and build: \`make release\`
3. Install: \`make install-user\`

## Package Contents
- \`lynx\` - The shell binary
- \`install.sh\` - Installation script
- \`uninstall.sh\` - Uninstallation script
- Configuration files and themes
- Documentation

## Supported Platforms
- Linux x64
- macOS x64
- macOS ARM64 (Apple Silicon)

## Getting Started
After installation, run \`lynx\` to start the shell.
Type \`help\` for available commands.
Customize with \`theme\`, \`alias\`, and \`set\` commands.

## Documentation
- README.md - Basic usage
- CUSTOMIZATION.md - Customization guide
- DEVELOPMENT.md - Development guide

## Checksums
See SHA256SUMS for file integrity verification.
EOF

    log_success "Release notes generated"
}

# Display release summary
show_summary() {
    echo ""
    echo -e "${GREEN}╔══════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║          Release Summary             ║${NC}"
    echo -e "${GREEN}╚══════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${BLUE}Version:${NC} $VERSION"
    echo -e "${BLUE}Platform:${NC} $PLATFORM_ARCH"
    echo ""
    echo -e "${BLUE}Generated packages:${NC}"
    ls -la "$RELEASE_DIR"
    echo ""
    echo -e "${GREEN}Release packages created successfully!${NC}"
    echo -e "${BLUE}Upload these files to your GitHub release.${NC}"
    echo ""
}

# Main release process
main() {
    echo -e "${BLUE}"
    echo "╔══════════════════════════════════════╗"
    echo "║         Lynx Shell Release           ║"
    echo "║           Package Builder            ║"
    echo "╚══════════════════════════════════════╝"
    echo -e "${NC}"

    detect_platform
    clean_build
    build_binary
    create_binary_package
    create_source_package
    build_cmake_packages
    create_checksums
    generate_release_notes
    show_summary
}

# Check if we're in the right directory
if [[ ! -f "CMakeLists.txt" ]] || [[ ! -f "Makefile" ]]; then
    log_error "This script must be run from the Lynx project root directory"
    exit 1
fi

# Run main process
main "$@"