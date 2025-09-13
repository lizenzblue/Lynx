#include "plugin.h"
#include "shell.h"
#include "command.h"
#include "config.h"
#include "utils.h"
#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <algorithm>

// Plugin Manager Implementation
PluginManager::PluginManager(Shell* shell) : shell(shell) {
    // Add default plugin paths
    std::string homeDir = Utils::getHomeDirectory();
    addPluginPath(homeDir + "/.lynx/plugins");
    addPluginPath("/usr/local/lib/lynx/plugins");
    addPluginPath("./plugins");
}

PluginManager::~PluginManager() {
    unloadAllPlugins();
}

void PluginManager::addPluginPath(const std::string& path) {
    if (std::find(pluginPaths.begin(), pluginPaths.end(), path) == pluginPaths.end()) {
        pluginPaths.push_back(path);
    }
}

std::vector<std::string> PluginManager::discoverPlugins() {
    std::vector<std::string> pluginFiles;
    
    for (const auto& path : pluginPaths) {
        if (!std::filesystem::exists(path)) {
            continue;
        }
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    // Look for shared library files (.so, .dylib, .dll)
                    if (filename.find(".so") != std::string::npos || 
                        filename.find(".dylib") != std::string::npos || 
                        filename.find(".dll") != std::string::npos) {
                        pluginFiles.push_back(entry.path().string());
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error scanning plugin directory " << path << ": " << e.what() << std::endl;
        }
    }
    
    return pluginFiles;
}

bool PluginManager::loadPlugin(const std::string& pluginPath) {
    // Load the shared library
    void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot load plugin " << pluginPath << ": " << dlerror() << std::endl;
        return false;
    }
    
    // Clear any existing error
    dlerror();
    
    // Get the plugin creation function
    typedef IPlugin* (*create_plugin_t)();
    create_plugin_t createPlugin = (create_plugin_t) dlsym(handle, "createPlugin");
    
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol createPlugin: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }
    
    // Check API version compatibility
    typedef const char* (*get_api_version_t)();
    get_api_version_t getApiVersion = (get_api_version_t) dlsym(handle, "getPluginApiVersion");
    
    if (getApiVersion) {
        std::string apiVersion = getApiVersion();
        if (apiVersion != LYNX_PLUGIN_API_VERSION) {
            std::cerr << "Plugin " << pluginPath << " has incompatible API version: " 
                      << apiVersion << " (expected: " << LYNX_PLUGIN_API_VERSION << ")" << std::endl;
            dlclose(handle);
            return false;
        }
    }
    
    // Create the plugin instance
    std::unique_ptr<IPlugin> plugin(createPlugin());
    if (!plugin) {
        std::cerr << "Failed to create plugin instance from " << pluginPath << std::endl;
        dlclose(handle);
        return false;
    }
    
    // Initialize the plugin
    if (!plugin->initialize(shell)) {
        std::cerr << "Failed to initialize plugin from " << pluginPath << std::endl;
        dlclose(handle);
        return false;
    }
    
    // Get plugin info and store
    const PluginInfo& info = plugin->getInfo();
    
    // Check if plugin with same name is already loaded
    if (isPluginLoaded(info.name)) {
        std::cerr << "Plugin " << info.name << " is already loaded" << std::endl;
        plugin->shutdown();
        dlclose(handle);
        return false;
    }
    
    // Store the plugin
    loadedPlugins[info.name] = std::move(plugin);
    
    std::cout << "Loaded plugin: " << info.name << " v" << info.version 
              << " by " << info.author << std::endl;
    
    // Broadcast plugin loaded event
    std::map<std::string, std::string> context;
    context["plugin_name"] = info.name;
    context["plugin_version"] = info.version;
    broadcastEvent(PluginEvent::SHELL_STARTUP, context);
    
    return true;
}

bool PluginManager::unloadPlugin(const std::string& pluginName) {
    auto it = loadedPlugins.find(pluginName);
    if (it == loadedPlugins.end()) {
        return false;
    }
    
    // Shutdown the plugin
    it->second->shutdown();
    
    // Remove any commands registered by this plugin
    auto cmdIt = pluginCommands.begin();
    while (cmdIt != pluginCommands.end()) {
        // Note: We would need to track which plugin registered which command
        // For now, we'll implement a simple approach
        ++cmdIt;
    }
    
    // Remove the plugin
    loadedPlugins.erase(it);
    
    std::cout << "Unloaded plugin: " << pluginName << std::endl;
    return true;
}

void PluginManager::loadAllPlugins() {
    std::vector<std::string> plugins = discoverPlugins();
    
    for (const auto& pluginPath : plugins) {
        loadPlugin(pluginPath);
    }
}

void PluginManager::unloadAllPlugins() {
    // Broadcast shutdown event before unloading
    broadcastEvent(PluginEvent::SHELL_SHUTDOWN);
    
    // Unload all plugins
    for (auto& [name, plugin] : loadedPlugins) {
        plugin->shutdown();
    }
    
    loadedPlugins.clear();
    pluginCommands.clear();
}

bool PluginManager::isPluginLoaded(const std::string& pluginName) const {
    return loadedPlugins.find(pluginName) != loadedPlugins.end();
}

IPlugin* PluginManager::getPlugin(const std::string& pluginName) const {
    auto it = loadedPlugins.find(pluginName);
    return (it != loadedPlugins.end()) ? it->second.get() : nullptr;
}

std::vector<std::string> PluginManager::getLoadedPluginNames() const {
    std::vector<std::string> names;
    for (const auto& [name, plugin] : loadedPlugins) {
        names.push_back(name);
    }
    return names;
}

bool PluginManager::registerCommand(const std::string& pluginName, const PluginCommand& command) {
    if (!isPluginLoaded(pluginName)) {
        return false;
    }
    
    if (pluginCommands.find(command.name) != pluginCommands.end()) {
        std::cerr << "Command " << command.name << " is already registered" << std::endl;
        return false;
    }
    
    pluginCommands[command.name] = command;
    return true;
}

bool PluginManager::unregisterCommand(const std::string& commandName) {
    auto it = pluginCommands.find(commandName);
    if (it != pluginCommands.end()) {
        pluginCommands.erase(it);
        return true;
    }
    return false;
}

bool PluginManager::executePluginCommand(const Command& cmd) {
    auto it = pluginCommands.find(cmd.name);
    if (it != pluginCommands.end()) {
        return it->second.handler(cmd, shell);
    }
    return false;
}

bool PluginManager::isPluginCommand(const std::string& commandName) const {
    return pluginCommands.find(commandName) != pluginCommands.end();
}

void PluginManager::broadcastEvent(PluginEvent event, const std::map<std::string, std::string>& context) {
    for (auto& [name, plugin] : loadedPlugins) {
        try {
            plugin->onEvent(event, context);
        } catch (const std::exception& e) {
            std::cerr << "Plugin " << name << " threw exception during event handling: " 
                      << e.what() << std::endl;
        }
    }
}

std::vector<PluginInfo> PluginManager::getPluginInfos() const {
    std::vector<PluginInfo> infos;
    for (const auto& [name, plugin] : loadedPlugins) {
        infos.push_back(plugin->getInfo());
    }
    return infos;
}

void PluginManager::listPlugins() const {
    std::cout << "Loaded Plugins:" << std::endl;
    for (const auto& [name, plugin] : loadedPlugins) {
        const PluginInfo& info = plugin->getInfo();
        std::cout << "  " << info.name << " v" << info.version 
                  << " by " << info.author << std::endl;
        std::cout << "    " << info.description << std::endl;
    }
}

// Plugin API Implementation
PluginAPI::PluginAPI(Shell* shell) : shell(shell) {}

void PluginAPI::print(const std::string& message) {
    std::cout << message;
}

void PluginAPI::println(const std::string& message) {
    std::cout << message << std::endl;
}

void PluginAPI::printError(const std::string& message) {
    std::cerr << message << std::endl;
}

void PluginAPI::addToHistory(const std::string& command) {
    if (shell) {
        shell->addToHistory(command);
    }
}

const std::vector<std::string>& PluginAPI::getHistory() const {
    static std::vector<std::string> empty;
    return shell ? shell->getHistory() : empty;
}

std::string PluginAPI::getConfigValue(const std::string& key, const std::string& defaultValue) const {
    if (shell && shell->getConfigManager()) {
        return shell->getConfigManager()->getSetting(key, defaultValue);
    }
    return defaultValue;
}

void PluginAPI::setConfigValue(const std::string& key, const std::string& value) {
    if (shell && shell->getConfigManager()) {
        shell->getConfigManager()->setSetting(key, value);
    }
}

std::string PluginAPI::getCurrentDirectory() const {
    return Utils::getCurrentDirectory();
}

int PluginAPI::getLastExitCode() const {
    return shell ? shell->getLastExitCode() : 0;
}

void PluginAPI::setLastExitCode(int code) {
    if (shell) {
        shell->setLastExitCode(code);
    }
}

std::string PluginAPI::expandPath(const std::string& path) const {
    return Utils::expandPath(path);
}

bool PluginAPI::fileExists(const std::string& path) const {
    return std::filesystem::exists(path);
}

std::vector<std::string> PluginAPI::listDirectory(const std::string& path) const {
    std::vector<std::string> files;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            files.push_back(entry.path().filename().string());
        }
    } catch (const std::filesystem::filesystem_error&) {
        // Return empty vector on error
    }
    
    return files;
}