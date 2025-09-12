# Lynx Shell

A simple shell implementation built from scratch in C++.

## Features

- Command execution
- Built-in commands (cd, pwd, exit, help)
- Command history
- Environment variable support
- Pipeline support (planned)
- Input/Output redirection (planned)

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

## Usage

The shell supports basic commands and has several built-in commands:

- `cd <directory>` - Change directory
- `pwd` - Print working directory
- `exit` - Exit the shell
- `help` - Show help information
- `history` - Show command history
- `env` - Display environment variables

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
