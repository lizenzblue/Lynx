# Lynx Shell

A modern, **highly customizable** shell implementation built from scratch in C++ with production-ready packaging and distribution.

## 🚀 Features

### Core Functionality
- **Full shell implementation** with built-in and external command execution
- **Comprehensive customization system** with themes, aliases, and runtime settings
- **Modern C++17** codebase with smart pointers and STL containers
- **Cross-platform** support (Linux and macOS)

### User Experience
- **Multiple built-in themes** (default, dark, blue, green, purple, rainbow)
- **Runtime configuration** - change settings without restarting
- **Command aliases** with automatic expansion and persistence
- **Colored prompts** with customizable formats and user/host/path variables
- **Command history** with configurable size and persistence
- **Environment variable** support and display

### Production Ready
- **CI/CD pipeline** with automated builds and releases
- **Package management** with tar.gz, zip, DEB, and RPM generation
- **One-command installation** with automatic shell integration
- **Version management** system with build information
- **Comprehensive documentation** and user guides

## ⚡ Quick Installation

### One-Line Install (Recommended)
```bash
curl -sSL https://raw.githubusercontent.com/your-username/lynx/main/install.sh | bash
```

### Manual Download
```bash
# Download latest release
wget https://github.com/your-username/lynx/releases/latest/download/lynx-linux.tar.gz
tar -xzf lynx-linux.tar.gz
cd lynx-*
sudo ./install.sh
```

### Build from Source
```bash
git clone https://github.com/your-username/lynx.git
cd lynx
make
sudo make install
```

## 🎯 Usage

```bash
# Start the shell
lynx

# Try some commands
help                               # See all available commands
version                           # Show version information
theme set dark                    # Switch to dark theme
alias ll ls -la --color=auto      # Create an alias
set prompt_format "%u@%h:%w$ "    # Customize prompt
history                           # View command history
clear                            # Clear screen
```

## 📦 Available Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `cd` | Change directory | `cd <directory>` |
| `pwd` | Print working directory | `pwd` |
| `help` | Show help message | `help` |
| `history` | Show command history | `history` |
| `env` | Display environment variables | `env` |
| `clear` | Clear the screen | `clear` |
| `exit` | Exit the shell | `exit` |
| `set` | Set configuration option | `set <key> <value>` |
| `alias` | Manage command aliases | `alias [name] [command]` |
| `theme` | Change color theme | `theme [set <name>]` |
| `version` | Show version information | `version` |

## 🎨 Customization & Configuration

Lynx stores configuration in `~/.lynx/`:
- `config.ini` - Main configuration file
- `aliases.ini` - Command aliases  
- `themes/` - Custom theme definitions

### Available Themes
- **default** - Clean and minimal
- **dark** - Dark theme with bright accents
- **blue** - Blue-based color scheme
- **green** - Nature-inspired green theme
- **purple** - Elegant purple theme
- **rainbow** - Colorful multi-hue theme

### Configuration Examples
```bash
# Switch themes
theme set dark

# Create useful aliases
alias ll ls -la --color=auto
alias gs git status
alias .. cd ..

# Customize prompt and behavior
set prompt_format "%u@%h:%w$ "
set history_size 2000
set welcome_message "Welcome to your custom shell!"
```

### Custom Theme Creation
Create `~/.lynx/themes/mytheme.ini`:
```ini
[colors]
primary = \033[1;36m
secondary = \033[1;35m
accent = \033[1;33m
error = \033[1;31m
success = \033[1;32m
prompt = \033[1;34m
text = \033[0m
```

## 🔧 Development & Building

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+)
- Make or CMake
- Git

### Build Options
```bash
# Production build
make

# Debug build
make debug

# CMake alternative
mkdir build && cd build
cmake .. && make

# Install system-wide
sudo make install
```

## 🚀 Production Pipeline

### For Users
- **Automated releases** via GitHub Actions
- **Multi-platform packages** (Linux, macOS)
- **Checksums** for verification
- **One-command installation**

### For Maintainers
```bash
# Generate release packages
./release.sh

# Manual packaging
make package
```

## 📦 Package Formats
- **tar.gz** - Universal archive format
- **zip** - Alternative archive format  
- **DEB** - Debian/Ubuntu packages
- **RPM** - RedHat/CentOS packages

## 🗑️ Uninstallation

```bash
# Using uninstaller
sudo lynx-uninstall

# Manual removal
sudo rm /usr/local/bin/lynx /usr/local/bin/lynx-uninstall
rm -rf ~/.lynx  # Optional: removes configuration
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Setup
```bash
git clone https://github.com/your-username/lynx.git
cd lynx
make debug
./build/lynx
```

## 📊 Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Linux | ✅ Supported | Ubuntu 20.04+, CentOS 8+, Arch Linux |
| macOS | ✅ Supported | macOS 11+ |
| Windows | ❌ Not supported | Contributions welcome |

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

## 💬 Support & Community

- **Issues**: [GitHub Issues](https://github.com/your-username/lynx/issues)
- **Discussions**: [GitHub Discussions](https://github.com/your-username/lynx/discussions)
- **Documentation**: [Project Wiki](https://github.com/your-username/lynx/wiki)

## 📈 Changelog

### v1.0.0 (Latest)
- ✅ Complete shell implementation with all core commands
- ✅ Advanced theming and customization system
- ✅ Alias management with persistence
- ✅ Cross-platform support (Linux/macOS)
- ✅ CI/CD pipeline with automated releases
- ✅ Production packaging (tar.gz, zip, DEB, RPM)
- ✅ One-command installation system
- ✅ Version management and build information
- ✅ Comprehensive documentation

---

**Lynx Shell** - *Where customization meets performance* 🚀

- `cd <directory>` - Change directory
- `pwd` - Print working directory
- `exit` - Exit the shell
- `help` - Show help information
- `history` - Show command history
- `env` - Display environment variables
- `clear` - Clear the screen
- `set [key] [value]` - Configure shell settings
- `alias [name] [command]` - Manage command aliases
- `theme [name]` - Switch color themes

Plus all external commands available in your PATH!

## Project Structure

```
├── CMakeLists.txt
├── README.md
├── include/
│   ├── shell.h
│   ├── command.h
│   └── utils.h
├── src/
│   ├── main.cpp
│   ├── shell.cpp
│   ├── command.cpp
│   └── utils.cpp
└── build/
```

## Contributing

Feel free to contribute to this project by adding new features or improving existing ones!
