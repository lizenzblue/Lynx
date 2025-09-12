# Lynx Shell

A **highly customizable** shell implementation built from scratch in C++.

## 🚀 Features

- **Command execution** (built-in and external commands)
- **Comprehensive customization system** with themes, aliases, and settings
- **Multiple built-in themes** (default, dark, minimal, rainbow)
- **Runtime configuration** - change settings without restarting
- **Command aliases** with automatic expansion
- **Colored prompts** with customizable formats
- **Command history** with configurable size
- **Environment variable** support
- **Modern C++17** codebase
- **Cross-platform** compatible

## ⚡ Quick Start

```bash
# Build the shell
make debug

# Run it
./build/lynx

# Try some commands
help           # See all available commands
theme dark     # Switch to dark theme
alias ll ls -la # Create an alias
set prompt_format "{user} ➜ {cwd} $ "  # Customize prompt
```

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./lynx
```

## 🎨 Customization

Your shell is **fully customizable**! You can:

- **Switch themes**: `theme dark`, `theme minimal`, `theme rainbow`
- **Create aliases**: `alias ll ls -la`, `alias gc git commit`
- **Customize prompts**: `set prompt_format "{user}@{host}:{cwd} ➜ "`
- **Configure behavior**: `set history_size 2000`, `set auto_cd true`

See [CUSTOMIZATION.md](CUSTOMIZATION.md) for complete customization guide.

## 📋 Built-in Commands

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
