#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

// Forward declarations
class Shell;
struct Command;

/**
 * Plugin Event Types
 */
enum class PluginEvent {
    SHELL_STARTUP,
    SHELL_SHUTDOWN,
    COMMAND_BEFORE,
    COMMAND_AFTER,
    PROMPT_DISPLAY,
    INPUT_RECEIVED
};

/**
 * Plugin Information Structure
 */
struct PluginInfo {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::vector<std::string> dependencies;
    int apiVersion;
};

/**
 * Base Plugin Interface
 * All plugins must inherit from this class
 */
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    // Core lifecycle methods
    virtual bool initialize(Shell* shell) = 0;
    virtual void shutdown() = 0;
    virtual const PluginInfo& getInfo() const = 0;
    
    // Event handling
    virtual void onEvent(PluginEvent event, const std::map<std::string, std::string>& context) {
        (void)event; (void)context; // Suppress unused parameter warnings
    }
    
    // Command handling (optional)
    virtual bool handleCommand(const Command& cmd, Shell* shell) { 
        (void)cmd; (void)shell; // Suppress unused parameter warnings
        return false; 
    }
    
    // Configuration (optional)
    virtual void configure(const std::map<std::string, std::string>& config) {
        (void)config; // Suppress unused parameter warnings
    }
};

/**
 * Plugin Command Registration
 */
struct PluginCommand {
    std::string name;
    std::string description;
    std::string usage;
    std::function<bool(const Command&, Shell*)> handler;
};

/**
 * Plugin Manager - Handles loading, unloading, and managing plugins
 */
class PluginManager {
private:
    Shell* shell;
    std::map<std::string, std::unique_ptr<IPlugin>> loadedPlugins;
    std::map<std::string, PluginCommand> pluginCommands;
    std::vector<std::string> pluginPaths;

public:
    explicit PluginManager(Shell* shell);
    ~PluginManager();
    
    // Plugin lifecycle
    bool loadPlugin(const std::string& pluginPath);
    bool unloadPlugin(const std::string& pluginName);
    void loadAllPlugins();
    void unloadAllPlugins();
    
    // Plugin discovery
    void addPluginPath(const std::string& path);
    std::vector<std::string> discoverPlugins();
    
    // Plugin interaction
    bool isPluginLoaded(const std::string& pluginName) const;
    IPlugin* getPlugin(const std::string& pluginName) const;
    std::vector<std::string> getLoadedPluginNames() const;
    
    // Command handling
    bool registerCommand(const std::string& pluginName, const PluginCommand& command);
    bool unregisterCommand(const std::string& commandName);
    bool executePluginCommand(const Command& cmd);
    bool isPluginCommand(const std::string& commandName) const;
    
    // Event system
    void broadcastEvent(PluginEvent event, const std::map<std::string, std::string>& context = {});
    
    // Plugin information
    std::vector<PluginInfo> getPluginInfos() const;
    void listPlugins() const;
};

/**
 * Plugin API - Provides safe interfaces for plugins to interact with the shell
 */
class PluginAPI {
private:
    Shell* shell;
    
public:
    explicit PluginAPI(Shell* shell);
    
    // Shell interaction
    void print(const std::string& message);
    void println(const std::string& message);
    void printError(const std::string& message);
    
    // History access
    void addToHistory(const std::string& command);
    const std::vector<std::string>& getHistory() const;
    
    // Configuration access
    std::string getConfigValue(const std::string& key, const std::string& defaultValue = "") const;
    void setConfigValue(const std::string& key, const std::string& value);
    
    // Environment
    std::string getCurrentDirectory() const;
    int getLastExitCode() const;
    void setLastExitCode(int code);
    
    // Utility functions
    std::string expandPath(const std::string& path) const;
    bool fileExists(const std::string& path) const;
    std::vector<std::string> listDirectory(const std::string& path) const;
};

// Plugin entry point macros
#define LYNX_PLUGIN_ENTRY_POINT(PluginClass) \
    extern "C" { \
        IPlugin* createPlugin() { \
            return new PluginClass(); \
        } \
        void destroyPlugin(IPlugin* plugin) { \
            delete plugin; \
        } \
        const char* getPluginApiVersion() { \
            return "1.0.0"; \
        } \
    }

#define LYNX_PLUGIN_API_VERSION "1.0.0"

#endif // PLUGIN_H