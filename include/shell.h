#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class ConfigManager;
class PluginManager;
class ExternalThemeManager;

class Shell {
private:
    std::vector<std::string> history;
    bool running;
    std::string currentDirectory;
    int lastExitCode;
    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<PluginManager> pluginManager;
    std::unique_ptr<ExternalThemeManager> themeManager;

public:
    Shell();
    ~Shell();
    
    void run();
    void displayPrompt();
    std::string readInput();
    void executeCommand(const std::string& input);
    void addToHistory(const std::string& command);
    void printHistory();
    bool isRunning() const;
    void exit();
    
    // Configuration access
    ConfigManager* getConfigManager() { return configManager.get(); }
    void setLastExitCode(int code) { lastExitCode = code; }
    int getLastExitCode() const { return lastExitCode; }
    
    // History access
    const std::vector<std::string>& getHistory() const { return history; }
    
    // Plugin system access
    PluginManager* getPluginManager() { return pluginManager.get(); }
    
    // Theme system access
    ExternalThemeManager* getThemeManager() { return themeManager.get(); }
};

#endif // SHELL_H
