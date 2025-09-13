# Plugin Development Guide

This comprehensive guide covers everything you need to know about developing plugins for Lynx Shell.

## 🚀 Quick Start

### 1. Create Your First Plugin

```bash
# Navigate to Lynx source directory
cd /path/to/Lynx

# Create a new plugin from template
./create_plugin.sh my_awesome_plugin

# Build and install
cd examples/plugins/my_awesome_plugin/
make
make install
```

### 2. Test Your Plugin

```bash
# Start Lynx and load your plugin
lynx
> load_plugin my_awesome_plugin
> help  # Your plugin commands should appear
```

## 📁 Plugin Structure

### Basic Plugin Directory Layout

```
my_plugin/
├── Makefile              # Build configuration
├── my_plugin.cpp         # Main plugin source
├── README.md            # Plugin documentation
└── config.json          # Plugin metadata (optional)
```

### Advanced Plugin Structure

```
complex_plugin/
├── Makefile
├── src/
│   ├── main.cpp         # Plugin entry point
│   ├── commands/        # Command implementations
│   │   ├── cmd1.cpp
│   │   └── cmd2.cpp
│   └── utils/          # Utility functions
│       └── helper.cpp
├── include/            # Header files
│   ├── plugin.h
│   └── commands/
├── resources/          # Static resources
│   ├── templates/
│   └── data/
├── tests/             # Unit tests
│   └── test_plugin.cpp
├── README.md
└── config.json
```

## 🔧 Plugin API Reference

### Core Interface

Every plugin must implement the `IPlugin` interface:

```cpp
#include "plugin.h"

class MyPlugin : public IPlugin {
public:
    // Plugin lifecycle
    bool initialize(PluginAPI* api) override;
    void cleanup() override;

    // Plugin metadata
    std::string getName() const override;
    std::string getVersion() const override;
    std::string getDescription() const override;

    // Command handling
    std::vector<std::string> getCommands() const override;
    bool executeCommand(const std::string& command,
                       const std::vector<std::string>& args) override;

    // Event handling (optional)
    void onShellEvent(ShellEvent event, const std::string& data) override;
};
```

### Plugin Entry Point

```cpp
// Required: Export plugin creation function
extern "C" {
    IPlugin* create_plugin() {
        return new MyPlugin();
    }

    void destroy_plugin(IPlugin* plugin) {
        delete plugin;
    }

    // Optional: Plugin information
    const char* plugin_name() { return "MyPlugin"; }
    const char* plugin_version() { return "1.0.0"; }
    const char* plugin_api_version() { return "1.0"; }
}
```

### Available API Functions

The `PluginAPI` provides these functions for plugin use:

```cpp
class PluginAPI {
public:
    // Output functions
    void print(const std::string& message);
    void printError(const std::string& error);
    void printWarning(const std::string& warning);
    void printSuccess(const std::string& message);

    // Shell interaction
    std::string getCurrentDirectory();
    bool changeDirectory(const std::string& path);
    std::vector<std::string> listFiles(const std::string& path = ".");

    // Configuration
    std::string getConfigValue(const std::string& key);
    void setConfigValue(const std::string& key, const std::string& value);

    // Command registration
    bool registerCommand(const std::string& command, const std::string& description);
    bool unregisterCommand(const std::string& command);

    // Event system
    void addEventListener(ShellEvent event);
    void removeEventListener(ShellEvent event);
    void broadcastEvent(ShellEvent event, const std::string& data);

    // File operations
    bool fileExists(const std::string& path);
    std::string readFile(const std::string& path);
    bool writeFile(const std::string& path, const std::string& content);

    // Shell execution
    std::string executeShellCommand(const std::string& command);
    int getLastExitCode();
};
```

## 🎯 Event System

### Available Events

```cpp
enum class ShellEvent {
    COMMAND_EXECUTED,   // After any command execution
    DIRECTORY_CHANGED,  // When current directory changes
    FILE_CREATED,       // When a file is created
    FILE_DELETED,       // When a file is deleted
    SHELL_STARTUP,      // When shell starts
    SHELL_SHUTDOWN,     // When shell exits
    PLUGIN_LOADED,      // When a plugin is loaded
    PLUGIN_UNLOADED,    // When a plugin is unloaded
    CONFIG_CHANGED,     // When configuration changes
    THEME_CHANGED       // When theme changes
};
```

### Event Handling Example

```cpp
void MyPlugin::onShellEvent(ShellEvent event, const std::string& data) {
    switch (event) {
        case ShellEvent::COMMAND_EXECUTED:
            api->print("Command executed: " + data);
            break;

        case ShellEvent::DIRECTORY_CHANGED:
            api->print("Directory changed to: " + data);
            break;

        case ShellEvent::FILE_CREATED:
            api->print("File created: " + data);
            break;

        default:
            break;
    }
}
```

## 📝 Complete Plugin Examples

### 1. Simple Command Plugin

```cpp
// hello_plugin.cpp
#include "plugin.h"
#include <iostream>

class HelloPlugin : public IPlugin {
private:
    PluginAPI* api;

public:
    bool initialize(PluginAPI* pluginApi) override {
        api = pluginApi;
        api->registerCommand("hello", "Print a greeting message");
        api->registerCommand("greet", "Greet someone by name");
        return true;
    }

    void cleanup() override {
        api->unregisterCommand("hello");
        api->unregisterCommand("greet");
    }

    std::string getName() const override { return "HelloPlugin"; }
    std::string getVersion() const override { return "1.0.0"; }
    std::string getDescription() const override { return "Simple greeting plugin"; }

    std::vector<std::string> getCommands() const override {
        return {"hello", "greet"};
    }

    bool executeCommand(const std::string& command,
                       const std::vector<std::string>& args) override {
        if (command == "hello") {
            api->printSuccess("Hello from Lynx Shell!");
            return true;
        }

        if (command == "greet") {
            if (args.empty()) {
                api->printError("Usage: greet <name>");
                return false;
            }
            api->print("Hello, " + args[0] + "!");
            return true;
        }

        return false;
    }
};

// Plugin entry point
extern "C" {
    IPlugin* create_plugin() { return new HelloPlugin(); }
    void destroy_plugin(IPlugin* plugin) { delete plugin; }
    const char* plugin_name() { return "HelloPlugin"; }
    const char* plugin_version() { return "1.0.0"; }
}
```

### 2. File Operations Plugin

```cpp
// fileops_plugin.cpp
#include "plugin.h"
#include <fstream>
#include <filesystem>

class FileOpsPlugin : public IPlugin {
private:
    PluginAPI* api;

    bool createFile(const std::string& filename, const std::string& content = "") {
        try {
            std::ofstream file(filename);
            if (content.empty()) {
                file << "# Created by FileOps Plugin\n";
            } else {
                file << content;
            }
            return file.good();
        } catch (...) {
            return false;
        }
    }

    bool deleteFile(const std::string& filename) {
        return std::filesystem::remove(filename);
    }

    std::vector<std::string> searchFiles(const std::string& pattern) {
        std::vector<std::string> results;
        // Implementation for file search
        return results;
    }

public:
    bool initialize(PluginAPI* pluginApi) override {
        api = pluginApi;
        api->registerCommand("touch", "Create a new file");
        api->registerCommand("rm", "Delete a file");
        api->registerCommand("find", "Search for files");
        api->registerCommand("size", "Get file size");

        // Listen for file events
        api->addEventListener(ShellEvent::FILE_CREATED);
        api->addEventListener(ShellEvent::FILE_DELETED);

        return true;
    }

    void cleanup() override {
        api->unregisterCommand("touch");
        api->unregisterCommand("rm");
        api->unregisterCommand("find");
        api->unregisterCommand("size");
    }

    std::string getName() const override { return "FileOpsPlugin"; }
    std::string getVersion() const override { return "1.2.0"; }
    std::string getDescription() const override { return "Advanced file operations"; }

    std::vector<std::string> getCommands() const override {
        return {"touch", "rm", "find", "size"};
    }

    bool executeCommand(const std::string& command,
                       const std::vector<std::string>& args) override {
        if (command == "touch") {
            if (args.empty()) {
                api->printError("Usage: touch <filename> [content]");
                return false;
            }

            std::string content = args.size() > 1 ? args[1] : "";
            if (createFile(args[0], content)) {
                api->printSuccess("File '" + args[0] + "' created");
                return true;
            } else {
                api->printError("Failed to create file '" + args[0] + "'");
                return false;
            }
        }

        if (command == "rm") {
            if (args.empty()) {
                api->printError("Usage: rm <filename>");
                return false;
            }

            if (deleteFile(args[0])) {
                api->printSuccess("File '" + args[0] + "' deleted");
                return true;
            } else {
                api->printError("Failed to delete file '" + args[0] + "'");
                return false;
            }
        }

        if (command == "size") {
            if (args.empty()) {
                api->printError("Usage: size <filename>");
                return false;
            }

            try {
                auto size = std::filesystem::file_size(args[0]);
                api->print("Size of '" + args[0] + "': " + std::to_string(size) + " bytes");
                return true;
            } catch (...) {
                api->printError("Cannot get size of '" + args[0] + "'");
                return false;
            }
        }

        return false;
    }

    void onShellEvent(ShellEvent event, const std::string& data) override {
        switch (event) {
            case ShellEvent::FILE_CREATED:
                api->print("📄 File created: " + data);
                break;
            case ShellEvent::FILE_DELETED:
                api->print("🗑️  File deleted: " + data);
                break;
            default:
                break;
        }
    }
};

extern "C" {
    IPlugin* create_plugin() { return new FileOpsPlugin(); }
    void destroy_plugin(IPlugin* plugin) { delete plugin; }
    const char* plugin_name() { return "FileOpsPlugin"; }
    const char* plugin_version() { return "1.2.0"; }
}
```

## 🔨 Building Plugins

### Basic Makefile

```makefile
# Makefile for Lynx Plugin
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -Wall -Wextra -O2
INCLUDES = -I../../../include
LDFLAGS = -shared

PLUGIN_NAME = my_plugin
SOURCE = $(PLUGIN_NAME).cpp
TARGET = $(PLUGIN_NAME).so

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $<

install: $(TARGET)
	mkdir -p ~/.lynx/plugins
	cp $(TARGET) ~/.lynx/plugins/
	@echo "Plugin installed to ~/.lynx/plugins/"

uninstall:
	rm -f ~/.lynx/plugins/$(TARGET)
	@echo "Plugin uninstalled"

clean:
	rm -f $(TARGET)

.PHONY: all install uninstall clean
```

### Advanced Makefile (Multiple Sources)

```makefile
# Advanced Makefile for Complex Plugins
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -Wall -Wextra -O2 -g
INCLUDES = -I../../../include -Iinclude
LDFLAGS = -shared

PLUGIN_NAME = advanced_plugin
SRCDIR = src
OBJDIR = obj
INCDIR = include

SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(PLUGIN_NAME).so

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install: $(TARGET)
	mkdir -p ~/.lynx/plugins
	cp $(TARGET) ~/.lynx/plugins/
	@echo "Plugin installed to ~/.lynx/plugins/"

test: $(TARGET)
	# Add test commands here
	@echo "Running plugin tests..."

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all install test clean
```

## 🧪 Testing Plugins

### Manual Testing

```bash
# Build your plugin
make

# Install locally
make install

# Start Lynx shell
lynx

# Load and test your plugin
> load_plugin my_plugin
> help  # Verify your commands appear
> my_command arg1 arg2  # Test your commands
> unload_plugin my_plugin
```

### Automated Testing

Create a test script `test_plugin.sh`:

```bash
#!/bin/bash

PLUGIN_NAME="my_plugin"
LYNX_PATH="lynx"

echo "Testing plugin: $PLUGIN_NAME"

# Build plugin
make clean && make
if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi

# Install plugin
make install
if [ $? -ne 0 ]; then
    echo "❌ Installation failed"
    exit 1
fi

# Test plugin loading
echo "load_plugin $PLUGIN_NAME" | $LYNX_PATH > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Plugin loads successfully"
else
    echo "❌ Plugin failed to load"
    exit 1
fi

# Test commands
echo "✅ All tests passed!"
```

## 🚀 Best Practices

### 1. Error Handling

```cpp
bool MyPlugin::executeCommand(const std::string& command,
                             const std::vector<std::string>& args) {
    try {
        if (command == "risky_operation") {
            // Validate arguments
            if (args.empty()) {
                api->printError("Usage: risky_operation <param>");
                return false;
            }

            // Perform operation with error checking
            if (!performOperation(args[0])) {
                api->printError("Operation failed");
                return false;
            }

            api->printSuccess("Operation completed");
            return true;
        }
    } catch (const std::exception& e) {
        api->printError("Exception: " + std::string(e.what()));
        return false;
    } catch (...) {
        api->printError("Unknown error occurred");
        return false;
    }

    return false;
}
```

### 2. Resource Management

```cpp
class ResourcePlugin : public IPlugin {
private:
    std::vector<std::unique_ptr<Resource>> resources;

public:
    bool initialize(PluginAPI* api) override {
        // Allocate resources
        resources.push_back(std::make_unique<Resource>());
        return true;
    }

    void cleanup() override {
        // Clean up resources automatically with RAII
        resources.clear();
    }
};
```

### 3. Configuration Management

```cpp
class ConfigurablePlugin : public IPlugin {
private:
    std::string getConfigPath() {
        return api->getCurrentDirectory() + "/.lynx/plugins/" + getName() + ".conf";
    }

    void loadConfig() {
        std::string configFile = getConfigPath();
        if (api->fileExists(configFile)) {
            std::string content = api->readFile(configFile);
            // Parse configuration
        }
    }

    void saveConfig() {
        std::string configFile = getConfigPath();
        std::string content = generateConfigContent();
        api->writeFile(configFile, content);
    }
};
```

### 4. Command Help System

```cpp
bool MyPlugin::executeCommand(const std::string& command,
                             const std::vector<std::string>& args) {
    if (command == "mycommand") {
        // Check for help request
        if (!args.empty() && (args[0] == "-h" || args[0] == "--help")) {
            showHelp("mycommand");
            return true;
        }

        // Command implementation
        return true;
    }

    return false;
}

void MyPlugin::showHelp(const std::string& command) {
    if (command == "mycommand") {
        api->print("Usage: mycommand [options] <args>");
        api->print("  -h, --help    Show this help");
        api->print("  -v, --verbose Enable verbose output");
        api->print("");
        api->print("Description:");
        api->print("  This command does something useful.");
    }
}
```

## 📚 Plugin Metadata

### config.json Format

```json
{
  "name": "MyAwesomePlugin",
  "version": "1.0.0",
  "description": "A plugin that does awesome things",
  "author": "Your Name",
  "email": "your.email@example.com",
  "license": "MIT",
  "api_version": "1.0",
  "dependencies": {
    "lynx": ">=1.0.0"
  },
  "commands": [
    {
      "name": "awesome",
      "description": "Do something awesome",
      "usage": "awesome [options] <args>",
      "examples": ["awesome --help", "awesome file.txt"]
    }
  ],
  "events": ["COMMAND_EXECUTED", "FILE_CREATED"],
  "configuration": {
    "default_setting": "value",
    "enable_feature": true
  }
}
```

## 🔄 Plugin Lifecycle

### Loading Sequence

1. **Discovery**: Lynx scans `~/.lynx/plugins/` for `.so` files
2. **Loading**: Dynamic library is loaded with `dlopen()`
3. **Validation**: Plugin API version is checked
4. **Creation**: `create_plugin()` function is called
5. **Initialization**: `initialize()` method is called
6. **Registration**: Commands and events are registered

### Unloading Sequence

1. **Cleanup**: `cleanup()` method is called
2. **Deregistration**: Commands and events are unregistered
3. **Destruction**: `destroy_plugin()` function is called
4. **Unloading**: Dynamic library is unloaded with `dlclose()`

## 🐛 Debugging

### Debug Build

```makefile
# Add debug flags
CXXFLAGS = -std=c++17 -fPIC -Wall -Wextra -g -DDEBUG

# Debug target
debug: CXXFLAGS += -g -DDEBUG -O0
debug: $(TARGET)
```

### Logging

```cpp
#ifdef DEBUG
    #define DEBUG_LOG(msg) api->print("[DEBUG] " + std::string(msg))
#else
    #define DEBUG_LOG(msg)
#endif

void MyPlugin::someFunction() {
    DEBUG_LOG("Entering someFunction()");
    // Function implementation
    DEBUG_LOG("Exiting someFunction()");
}
```

### Common Issues

1. **Plugin won't load**: Check that all required symbols are exported
2. **Segmentation fault**: Ensure proper memory management and null checks
3. **Commands not working**: Verify command registration in `initialize()`
4. **API calls failing**: Check that PluginAPI pointer is valid

## 📖 Advanced Topics

### Thread Safety

Plugins should be thread-safe if they will be used in multi-threaded contexts:

```cpp
class ThreadSafePlugin : public IPlugin {
private:
    std::mutex api_mutex;

public:
    bool executeCommand(const std::string& command,
                       const std::vector<std::string>& args) override {
        std::lock_guard<std::mutex> lock(api_mutex);
        // Thread-safe command execution
        return true;
    }
};
```

### Shared State

Plugins can share state through the configuration system:

```cpp
// Plugin A sets shared data
api->setConfigValue("shared.data", "value_from_plugin_a");

// Plugin B reads shared data
std::string sharedData = api->getConfigValue("shared.data");
```

### Dynamic Command Registration

```cpp
void MyPlugin::onShellEvent(ShellEvent event, const std::string& data) {
    if (event == ShellEvent::CONFIG_CHANGED) {
        // Dynamically register/unregister commands based on config
        if (api->getConfigValue("enable_advanced") == "true") {
            api->registerCommand("advanced_cmd", "Advanced command");
        } else {
            api->unregisterCommand("advanced_cmd");
        }
    }
}
```

## 🎉 Publishing Your Plugin

### 1. Create a Repository

```bash
# Create plugin repository
mkdir lynx-plugin-myawesome
cd lynx-plugin-myawesome

# Initialize git
git init
git add .
git commit -m "Initial plugin commit"

# Push to GitHub
git remote add origin https://github.com/yourusername/lynx-plugin-myawesome.git
git push -u origin main
```

### 2. Add Installation Script

Create `install.sh`:

```bash
#!/bin/bash

PLUGIN_NAME="myawesome"
LYNX_PLUGINS_DIR="$HOME/.lynx/plugins"

echo "Installing Lynx Plugin: $PLUGIN_NAME"

# Create plugins directory
mkdir -p "$LYNX_PLUGINS_DIR"

# Build plugin
make clean && make
if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi

# Install plugin
cp "${PLUGIN_NAME}.so" "$LYNX_PLUGINS_DIR/"

echo "✅ Plugin installed successfully!"
echo "Load it in Lynx with: load_plugin $PLUGIN_NAME"
```

### 3. Documentation

Create comprehensive documentation:

- `README.md` with usage examples
- `CHANGELOG.md` for version history
- `LICENSE` file
- `examples/` directory with sample usage

Congratulations! You now have everything you need to create powerful plugins for Lynx Shell. Happy coding! 🚀
