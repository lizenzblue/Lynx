# Lynx Shell

A modern, **highly customizable** shell implementation built from scratch in C++ with **plugin and theme system** for unlimited extensibility.

## 🚀 Features

### Core Functionality

- **Full shell implementation** with built-in and external command execution
- **Plugin system** - Add custom commands and functionality without recompiling
- **External theme system** - Load themes from JSON/INI files at runtime
- **Modern C++17** codebase with smart pointers and STL containers
- **Cross-platform** support (Linux and macOS)

### Plugin & Theme System 🎨

- **Dynamic plugin loading** from shared libraries (`.so`, `.dylib`, `.dll`)
- **Event-driven architecture** - Plugins respond to shell events
- **Plugin API** - Safe interface for shell interaction
- **External themes** - JSON/INI theme files with hot-swapping
- **Example plugins**: Time utilities, enhanced file operations
- **Example themes**: Cyberpunk, minimal, ocean styles

### User Experience

- **Multiple built-in themes** (default, dark, blue, green, purple, rainbow)
- **File-based configuration** - edit config files like zsh/bash dotfiles
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
curl -sSL https://raw.githubusercontent.com/lizenzblue/lynx/main/install.sh | bash
```

### Plugin & Theme Setup

```bash
# After installing Lynx, set up plugins and themes
./setup_plugins.sh

# Or install from source
git clone https://github.com/lizenzblue/lynx.git
cd lynx
./setup_plugins.sh
```

### Manual Download

```bash
# Download latest release
wget https://github.com/lizenzblue/lynx/releases/latest/download/lynx-linux.tar.gz
tar -xzf lynx-linux.tar.gz
cd lynx-*
sudo ./install.sh
```

### Build from Source

```bash
git clone https://github.com/lizenzblue/lynx.git
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
history                           # View command history
clear                            # Clear screen
pwd                              # Show current directory
```

**Configuration**: Lynx loads settings automatically from `~/.lynx/` files at startup, just like zsh with its dotfiles.

## 📦 Available Commands

| Command   | Description                   | Usage            |
| --------- | ----------------------------- | ---------------- |
| `cd`      | Change directory              | `cd <directory>` |
| `pwd`     | Print working directory       | `pwd`            |
| `help`    | Show help message             | `help`           |
| `history` | Show command history          | `history`        |
| `env`     | Display environment variables | `env`            |
| `clear`   | Clear the screen              | `clear`          |
| `exit`    | Exit the shell                | `exit`           |
| `version` | Show version information      | `version`        |

### Plugin Commands

When plugins are loaded, additional commands become available:

| Command  | Description                 | Plugin    |
| -------- | --------------------------- | --------- |
| `time`   | Display current time        | time      |
| `date`   | Display current date        | time      |
| `uptime` | Show shell uptime           | time      |
| `lsa`    | Enhanced file listing       | fileutils |
| `tree`   | Directory tree view         | fileutils |
| `find`   | Find files by pattern       | fileutils |
| `count`  | Count files and directories | fileutils |

## 🔌 Plugin & Theme System

### Plugin Development

Create powerful extensions without modifying source code:

```cpp
#include "plugin.h"

class MyPlugin : public IPlugin {
public:
    bool initialize(Shell* shell) override {
        // Register commands, set up event handlers
        return true;
    }

    void onEvent(PluginEvent event, const std::map<std::string, std::string>& context) override {
        // Respond to shell events
    }
};

LYNX_PLUGIN_ENTRY_POINT(MyPlugin)
```

**Compile and install:**

```bash
./examples/plugins/build_plugin.sh myplugin --install
```

### Theme Development

Create custom themes with JSON or INI files:

```json
{
  "info": {
    "name": "mytheme",
    "description": "My custom theme"
  },
  "colors": {
    "prompt_prefix": "#00FF00",
    "prompt_directory": "#0080FF",
    "output_success": "#00FF7F"
  },
  "settings": {
    "prompt_format": "➜ {directory} ",
    "show_exit_code": true
  }
}
```

**Install theme:**

```bash
cp mytheme.json ~/.lynx/themes/
# Set theme=mytheme in config
```

## 🎨 File-Based Configuration

Lynx loads configuration automatically from `~/.lynx/` at startup (similar to how zsh loads `.zshrc`):

- `config` - Main configuration file
- `plugins/` - Plugin shared libraries
- `themes/` - Custom theme definitions

**No interactive configuration needed** - just edit the files and restart the shell!

### Available Themes

**Built-in themes:**

- **default** - Clean and minimal
- **dark** - Dark theme with bright accents
- **blue** - Blue-based color scheme
- **green** - Nature-inspired green theme
- **purple** - Elegant purple theme
- **rainbow** - Colorful multi-hue theme

**Example external themes:**

- **cyberpunk** - Neon-inspired futuristic theme
- **minimal** - Ultra-clean minimal design
- **ocean** - Blue and teal ocean colors

### Configuration Examples

Edit `~/.lynx/config`:

```ini
# Theme and appearance
theme = cyberpunk

# Plugin settings
plugins_enabled = true
plugin_autoload = true

# Shell settings
welcome_message = Welcome to Lynx Shell with Plugins!
show_welcome = true
history_size = 2000
```

**That's it!** Restart the shell and your settings are active.

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
git clone https://github.com/lizenzblue/lynx.git
cd lynx
make debug
./build/lynx
```

## 📊 Platform Support

| Platform | Status           | Notes                                |
| -------- | ---------------- | ------------------------------------ |
| Linux    | ✅ Supported     | Ubuntu 20.04+, CentOS 8+, Arch Linux |
| macOS    | ✅ Supported     | macOS 11+                            |
| Windows  | ❌ Not supported | Contributions welcome                |

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

## 💬 Support & Community

- **Issues**: [GitHub Issues](https://github.com/lizenzblue/lynx/issues)
- **Discussions**: [GitHub Discussions](https://github.com/lizenzblue/lynx/discussions)
- **Documentation**: [Project Wiki](https://github.com/lizenzblue/lynx/wiki)

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

**Lynx Shell** - _Where customization meets performance_ 🚀

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
