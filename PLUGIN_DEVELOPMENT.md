# Lynx Shell Plugin and Theme System

## Overview

Lynx Shell features a powerful plugin and theme system that allows users to extend functionality and customize appearance without modifying the source code. This system provides:

- **Dynamic Plugin Loading**: Load plugins at runtime from shared libraries
- **Event System**: Plugins can respond to shell events (startup, commands, etc.)
- **Command Registration**: Plugins can add new commands to the shell
- **External Themes**: Load themes from configuration files (JSON, INI)
- **Plugin API**: Safe interface for plugins to interact with the shell

## Directory Structure

```
~/.lynx/
├── config              # Main configuration file
├── plugins/            # User-installed plugins (*.so, *.dylib, *.dll)
│   ├── libtime.so
│   └── libfileutils.so
└── themes/             # User themes
    ├── cyberpunk.theme
    ├── minimal.theme
    └── ocean.json
```

## Plugin Development

### 1. Plugin Architecture

All plugins must inherit from the `IPlugin` interface:

```cpp
#include "plugin.h"

class MyPlugin : public IPlugin {
public:
    bool initialize(Shell* shell) override;
    void shutdown() override;
    const PluginInfo& getInfo() const override;
    
    // Optional overrides
    void onEvent(PluginEvent event, const std::map<std::string, std::string>& context) override;
    bool handleCommand(const Command& cmd, Shell* shell) override;
    void configure(const std::map<std::string, std::string>& config) override;
};
```

### 2. Plugin Entry Point

Use the macro to export your plugin:

```cpp
LYNX_PLUGIN_ENTRY_POINT(MyPlugin)
```

### 3. Plugin Information

Configure your plugin metadata:

```cpp
MyPlugin() {
    info.name = "myplugin";
    info.version = "1.0.0";
    info.author = "Your Name";
    info.description = "Description of your plugin";
    info.apiVersion = 1;
}
```

### 4. Command Registration

Register commands in the `initialize()` method:

```cpp
bool initialize(Shell* shell) override {
    api = new PluginAPI(shell);
    
    PluginCommand myCmd;
    myCmd.name = "mycommand";
    myCmd.description = "My custom command";
    myCmd.usage = "mycommand [options]";
    myCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
        return handleMyCommand(cmd, shell);
    };
    
    auto* pluginManager = shell->getPluginManager();
    if (pluginManager) {
        pluginManager->registerCommand(info.name, myCmd);
    }
    
    return true;
}
```

### 5. Event Handling

Respond to shell events:

```cpp
void onEvent(PluginEvent event, const std::map<std::string, std::string>& context) override {
    switch (event) {
        case PluginEvent::SHELL_STARTUP:
            api->println("Plugin loaded!");
            break;
        case PluginEvent::COMMAND_BEFORE:
            // Log command execution
            break;
        // Handle other events...
    }
}
```

### 6. Plugin API

Use the Plugin API for safe shell interaction:

```cpp
// Output functions
api->print("Hello ");
api->println("World!");
api->printError("Error message");

// Shell interaction
api->addToHistory("command");
auto history = api->getHistory();

// Configuration
std::string value = api->getConfigValue("key", "default");
api->setConfigValue("key", "value");

// Environment
std::string dir = api->getCurrentDirectory();
int exitCode = api->getLastExitCode();

// File system
bool exists = api->fileExists("/path/to/file");
auto files = api->listDirectory("/path/to/dir");
```

### 7. Compilation

Use the provided build script:

```bash
# Compile a single plugin
./examples/plugins/build_plugin.sh myplugin

# Compile all plugins
./examples/plugins/build_plugin.sh --all

# Compile and install
./examples/plugins/build_plugin.sh --install myplugin

# List available plugins
./examples/plugins/build_plugin.sh --list
```

Manual compilation:

```bash
g++ -std=c++17 -fPIC -shared -O2 -Wall -Wextra \
    -I../../include -I../../src \
    myplugin.cpp -ldl -lstdc++fs \
    -o libmyplugin.so
```

### 8. Example Plugin

See `examples/plugins/template_plugin.cpp` for a complete template.

## Theme Development

### 1. Theme Configuration Format

Themes can be written in INI or JSON format:

#### INI Format (.theme, .ini)

```ini
[info]
name = mytheme
version = 1.0.0
author = Your Name
description = My custom theme

[colors]
prompt_prefix = bright_blue
prompt_directory = cyan
prompt_suffix = green
output_success = bright_green
output_error = bright_red

[settings]
prompt_format = {prefix}[{directory}]{suffix} $ 
show_git_branch = true
show_exit_code = true
```

#### JSON Format (.json)

```json
{
  "info": {
    "name": "mytheme",
    "version": "1.0.0",
    "author": "Your Name",
    "description": "My custom theme"
  },
  "colors": {
    "prompt_prefix": "#0080FF",
    "prompt_directory": "#00CED1",
    "output_success": "#00FF7F"
  },
  "settings": {
    "prompt_format": "{prefix}➜ {directory} ",
    "show_exit_code": true
  }
}
```

### 2. Color Formats

Themes support multiple color formats:

- **Named colors**: `red`, `green`, `blue`, `bright_red`, etc.
- **Hex colors**: `#FF0000`, `#00FF00`, `#0000FF`
- **ANSI codes**: `\033[31m`, `\033[92m`

### 3. Prompt Format

Use placeholders in `prompt_format`:

- `{prefix}` - Prompt prefix color
- `{directory}` - Current directory with color
- `{suffix}` - Prompt suffix color

### 4. Theme Installation

1. Copy theme file to `~/.lynx/themes/`
2. Set theme in config: `theme = mytheme`
3. Restart shell or use theme command

## Built-in Commands

### Plugin Management

```bash
# List loaded plugins
plugins list

# Plugin information
plugins info <plugin_name>

# Reload plugins
plugins reload
```

### Theme Management

```bash
# List available themes
themes list

# Set theme
themes set <theme_name>

# Current theme info
themes current

# Reload themes
themes reload
```

## API Reference

### PluginEvent Types

- `SHELL_STARTUP` - Shell has started
- `SHELL_SHUTDOWN` - Shell is shutting down  
- `COMMAND_BEFORE` - Before command execution
- `COMMAND_AFTER` - After command execution
- `PROMPT_DISPLAY` - Before displaying prompt
- `INPUT_RECEIVED` - User input received

### PluginAPI Methods

#### Output
- `print(string)` - Print without newline
- `println(string)` - Print with newline
- `printError(string)` - Print error message

#### History
- `addToHistory(string)` - Add to command history
- `getHistory()` - Get command history

#### Configuration
- `getConfigValue(key, default)` - Get config value
- `setConfigValue(key, value)` - Set config value

#### Environment
- `getCurrentDirectory()` - Get current directory
- `getLastExitCode()` - Get last command exit code
- `setLastExitCode(code)` - Set exit code

#### File System
- `expandPath(path)` - Expand ~ and variables
- `fileExists(path)` - Check if file exists
- `listDirectory(path)` - List directory contents

## Examples

### Time Plugin

Adds time-related commands:
- `time` - Display current time
- `date` - Display current date  
- `uptime` - Display shell uptime

### File Utils Plugin

Enhanced file operations:
- `lsa` - Detailed file listing
- `tree` - Directory tree view
- `find` - Find files by pattern
- `count` - Count files and directories

### Cyberpunk Theme

Neon-inspired colors with futuristic prompt format.

### Minimal Theme

Clean, subtle colors with simple prompt.

### Ocean Theme

Blue and teal color scheme with ocean emoji.

## Development Tips

1. **Error Handling**: Always check return values and handle errors gracefully
2. **Memory Management**: Clean up resources in the `shutdown()` method
3. **Thread Safety**: The plugin API is not thread-safe; use from main thread only
4. **Configuration**: Use the config system for persistent plugin settings
5. **Testing**: Test plugins thoroughly before installation
6. **Documentation**: Document your commands and configuration options

## Troubleshooting

### Plugin Not Loading

1. Check file permissions and location
2. Verify shared library format (.so, .dylib, .dll)
3. Check dependencies with `ldd` (Linux) or `otool -L` (macOS)
4. Ensure API version compatibility

### Theme Not Applying

1. Verify theme file syntax
2. Check theme file location in `~/.lynx/themes/`
3. Ensure theme name matches filename
4. Check for color format errors

### Compilation Errors

1. Include proper headers: `#include "plugin.h"`
2. Use C++17 standard: `-std=c++17`
3. Link required libraries: `-ldl -lstdc++fs`
4. Check include paths: `-I../../include`

## Contributing

We welcome plugin and theme contributions! Please:

1. Follow the coding standards
2. Include documentation
3. Add example configurations
4. Test on multiple platforms
5. Submit via pull request

## License

The plugin and theme system is released under the same license as Lynx Shell.