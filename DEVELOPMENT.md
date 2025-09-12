# Lynx Shell Development Guide

## Project Structure

```
Lynx/
├── CMakeLists.txt          # CMake build configuration
├── Makefile               # Alternative GNU Make build
├── README.md              # Project documentation
├── demo.sh               # Demo script
├── .gitignore            # Git ignore rules
├── include/              # Header files
│   ├── shell.h           # Main shell class
│   ├── command.h         # Command parsing and execution
│   └── utils.h           # Utility functions
├── src/                  # Source files
│   ├── main.cpp          # Entry point
│   ├── shell.cpp         # Shell implementation
│   ├── command.cpp       # Command handling
│   └── utils.cpp         # Utility implementations
└── build/                # Build output (created automatically)
    ├── obj/              # Object files
    └── lynx              # Final executable
```

## Building

### Using Make (Recommended)

```bash
# Debug build (with debugging symbols)
make debug

# Release build (optimized)
make release

# Clean build files
make clean

# Build and run
make run
```

### Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

## Architecture

### Core Components

1. **Shell Class** (`shell.h/cpp`)

   - Main shell loop
   - Command history management
   - Environment variable handling
   - User prompt generation

2. **Command System** (`command.h/cpp`)

   - Command parsing and tokenization
   - Built-in command execution
   - External command execution via fork/exec

3. **Utilities** (`utils.h/cpp`)
   - String manipulation
   - File system operations
   - Environment variable access
   - Terminal utilities

### Current Features

- ✅ Command execution (built-in and external)
- ✅ Built-in commands: `cd`, `pwd`, `exit`, `help`, `env`, `clear`
- ✅ Command history
- ✅ Environment variables
- ✅ Modern C++17 codebase
- ✅ Cross-platform compatible

### Planned Features

- [ ] Pipeline support (`command1 | command2`)
- [ ] Input/Output redirection (`>`, `>>`, `<`)
- [ ] Background processes (`&`)
- [ ] Job control (`jobs`, `fg`, `bg`)
- [ ] Tab completion
- [ ] Configuration file support
- [ ] Scripting support
- [ ] Advanced prompt customization

## Adding New Built-in Commands

1. Add the command name to `CommandExecutor::isBuiltinCommand()`
2. Add a case in `CommandExecutor::executeBuiltinCommand()`
3. Implement the command function (e.g., `executeMyCommand()`)
4. Update the help text in `executeHelp()`

Example:

```cpp
// In command.cpp
bool CommandExecutor::executeMyCommand(const std::vector<std::string>& args) {
    // Your command implementation here
    std::cout << "My custom command executed!" << std::endl;
    return true;
}
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Test thoroughly
5. Commit your changes (`git commit -m 'Add amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Create a Pull Request

## Testing

```bash
# Run the demo script
./demo.sh

# Interactive testing
./build/lynx

# Test specific commands
echo "pwd" | ./build/lynx
```

## Debugging

Build with debug symbols:

```bash
make debug
gdb ./build/lynx
```

Common debugging commands:

- `break main` - Set breakpoint at main
- `run` - Start execution
- `continue` - Continue execution
- `step` - Step into functions
- `next` - Step over functions
- `print variable` - Print variable value

## Performance Considerations

- The shell uses `fork()` and `execvp()` for external commands
- Command history is stored in memory (consider file persistence for large histories)
- Environment variables are cached locally for performance

## Security Notes

- Input validation is basic - consider adding more robust parsing
- External commands run with the same privileges as the shell
- No sandboxing is implemented for external commands
