# Installation Guide

This guide covers all methods to install Lynx Shell on your system.

## 🚀 Quick Installation (Recommended)

### One-Line Install Script

```bash
curl -sSL https://raw.githubusercontent.com/lizenzblue/lynx/main/install.sh | bash
```

This script will:

- Detect your operating system and architecture
- Download the appropriate release
- Install Lynx to `/usr/local/bin/`
- Set up shell integration
- Configure the plugin and theme system

## 📦 Manual Installation

### Download Pre-built Binaries

1. Go to the [Releases page](https://github.com/lizenzblue/Lynx/releases/latest)
2. Download the appropriate package for your system:

   - `lynx-linux-x64.tar.gz` - Linux x86_64
   - `lynx-macos-x64.tar.gz` - macOS Intel/Apple Silicon
   - `lynx-linux-static.tar.gz` - Static Linux binary

3. Extract and install:

```bash
tar -xzf lynx-*.tar.gz
cd lynx-*
sudo ./install.sh
```

## 🔧 Build from Source

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+)
- CMake 3.16+
- Make

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake git

git clone https://github.com/lizenzblue/Lynx.git
cd Lynx
make
sudo make install
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or install with Homebrew
brew install cmake

git clone https://github.com/lizenzblue/Lynx.git
cd Lynx
make
sudo make install
```

### CentOS/RHEL/Fedora

```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake

git clone https://github.com/lizenzblue/Lynx.git
cd Lynx
make
sudo make install
```

## 🔌 Plugin and Theme Setup

After installing Lynx, set up the plugin and theme system:

```bash
# Navigate to the Lynx source directory
cd Lynx

# Run the setup script
./setup_plugins.sh

# Or install specific components
./setup_plugins.sh --plugins-only
./setup_plugins.sh --themes-only
```

This will:

- Create `~/.lynx/` directory structure
- Install example plugins
- Install example themes
- Create default configuration

## 🐚 Shell Integration

### Make Lynx Your Default Shell

⚠️ **Warning**: Always test Lynx thoroughly before making it your default shell. Keep a way to access other shells in case of issues.

```bash
# Add Lynx to valid shells
echo "/usr/local/bin/lynx" | sudo tee -a /etc/shells

# Change your default shell
chsh -s /usr/local/bin/lynx
```

### Temporary Usage

To try Lynx without changing your default shell:

```bash
lynx
```

### Shell-specific Integration

#### Bash Integration

Add to your `~/.bashrc`:

```bash
# Launch Lynx if in interactive mode
if [[ $- == *i* ]]; then
    exec lynx
fi
```

#### Zsh Integration

Add to your `~/.zshrc`:

```bash
# Launch Lynx if in interactive mode
if [[ -o interactive ]]; then
    exec lynx
fi
```

## 📁 File Locations

After installation, Lynx files are located at:

### System Files

- **Binary**: `/usr/local/bin/lynx`
- **Documentation**: `/usr/local/share/doc/lynx/`
- **Examples**: `/usr/local/share/lynx/examples/`

### User Files

- **Configuration**: `~/.lynx/config`
- **Plugins**: `~/.lynx/plugins/`
- **Themes**: `~/.lynx/themes/`
- **History**: `~/.lynx/history`

## 🗑️ Uninstallation

### Remove Lynx

```bash
sudo rm -f /usr/local/bin/lynx
sudo rm -rf /usr/local/share/lynx/
sudo rm -rf /usr/local/share/doc/lynx/
```

### Remove User Data

```bash
# Remove configuration and plugins (optional)
rm -rf ~/.lynx/
```

### Restore Default Shell

```bash
# Change back to bash/zsh
chsh -s /bin/bash
# or
chsh -s /bin/zsh
```

## 🐛 Troubleshooting

### Common Issues

#### "Permission denied" when running

```bash
# Make sure the binary is executable
chmod +x /usr/local/bin/lynx
```

#### "Command not found"

```bash
# Check if Lynx is in your PATH
echo $PATH
which lynx

# Add to PATH if needed
export PATH="/usr/local/bin:$PATH"
```

#### Plugin compilation errors

```bash
# Install development tools
sudo apt install build-essential  # Ubuntu/Debian
sudo yum groupinstall "Development Tools"  # CentOS/RHEL
```

#### Theme not loading

```bash
# Check theme file permissions
ls -la ~/.lynx/themes/

# Verify theme syntax
cat ~/.lynx/themes/yourtheme.theme
```

### Getting Help

If you encounter issues:

1. Check the [Issue Tracker](https://github.com/lizenzblue/Lynx/issues)
2. Search existing issues for solutions
3. Create a new issue with:
   - Your operating system and version
   - Lynx version (`lynx --version`)
   - Error messages
   - Steps to reproduce

## ✅ Verification

After installation, verify everything works:

```bash
# Check version
lynx --version

# Test basic commands
echo "help" | lynx
echo "version" | lynx

# Check plugin system
ls ~/.lynx/plugins/

# Check theme system
ls ~/.lynx/themes/
```

## 🔄 Updates

To update Lynx to the latest version:

```bash
# Using the install script
curl -sSL https://raw.githubusercontent.com/lizenzblue/lynx/main/install.sh | bash

# Or build from source
cd Lynx
git pull origin main
make clean
make
sudo make install
```

Congratulations! You now have Lynx Shell installed and ready to use. 🎉
