#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Forward declarations
class ThemeManager;
class AliasManager;

class ConfigManager {
private:
    std::unordered_map<std::string, std::string> settings;
    std::string configFilePath;
    std::string configDirPath;
    
    std::unique_ptr<ThemeManager> themeManager;
    std::unique_ptr<AliasManager> aliasManager;
    
public:
    ConfigManager();
    ~ConfigManager();
    
    // Config file operations
    bool loadConfig();
    bool saveConfig();
    bool createDefaultConfig();
    
    // Setting management
    void setSetting(const std::string& key, const std::string& value);
    std::string getSetting(const std::string& key, const std::string& defaultValue = "");
    bool hasSetting(const std::string& key);
    void removeSetting(const std::string& key);
    
    // Specialized getters with type conversion
    int getIntSetting(const std::string& key, int defaultValue = 0);
    bool getBoolSetting(const std::string& key, bool defaultValue = false);
    std::vector<std::string> getListSetting(const std::string& key);
    
    // Managers
    ThemeManager* getThemeManager() { return themeManager.get(); }
    AliasManager* getAliasManager() { return aliasManager.get(); }
    
    // Validation
    bool validateSetting(const std::string& key, const std::string& value);
    
    // Config paths
    std::string getConfigDir() const { return configDirPath; }
    std::string getConfigFile() const { return configFilePath; }
    
private:
    void initializePaths();
    void createConfigDirectory();
    std::pair<std::string, std::string> parseLine(const std::string& line);
    std::string expandPath(const std::string& path);
};

class ThemeManager {
private:
    ConfigManager* config;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> themes;
    std::string currentTheme;
    
public:
    explicit ThemeManager(ConfigManager* configManager);
    
    // Theme management
    bool loadTheme(const std::string& themeName);
    bool saveTheme(const std::string& themeName);
    std::vector<std::string> getAvailableThemes();
    void setCurrentTheme(const std::string& themeName);
    std::string getCurrentTheme() const { return currentTheme; }
    
    // Color and formatting
    std::string getColor(const std::string& element);
    std::string getPromptFormat();
    std::string formatPrompt(const std::string& user, const std::string& host, 
                           const std::string& cwd, int exitCode = 0);
    
    // ANSI color codes
    std::string colorize(const std::string& text, const std::string& color);
    std::string resetColor();
    
private:
    void loadDefaultThemes();
    std::string getANSICode(const std::string& colorName);
};

class AliasManager {
private:
    ConfigManager* config;
    std::unordered_map<std::string, std::string> aliases;
    std::unordered_map<std::string, std::string> functions;
    
public:
    explicit AliasManager(ConfigManager* configManager);
    
    // Alias management
    void setAlias(const std::string& name, const std::string& command);
    void removeAlias(const std::string& name);
    bool hasAlias(const std::string& name);
    std::string getAlias(const std::string& name);
    std::string expandAlias(const std::string& command);
    std::vector<std::pair<std::string, std::string>> getAllAliases();
    
    // Function management
    void setFunction(const std::string& name, const std::string& body);
    void removeFunction(const std::string& name);
    bool hasFunction(const std::string& name);
    std::string getFunction(const std::string& name);
    
    // File operations
    bool loadAliases();
    bool saveAliases();
    bool loadFunctions();
    bool saveFunctions();
    
private:
    std::string getAliasFilePath();
    std::string getFunctionFilePath();
};

// Color constants
namespace Colors {
    extern const std::string RESET;
    extern const std::string BLACK;
    extern const std::string RED;
    extern const std::string GREEN;
    extern const std::string YELLOW;
    extern const std::string BLUE;
    extern const std::string MAGENTA;
    extern const std::string CYAN;
    extern const std::string WHITE;
    extern const std::string BRIGHT_BLACK;
    extern const std::string BRIGHT_RED;
    extern const std::string BRIGHT_GREEN;
    extern const std::string BRIGHT_YELLOW;
    extern const std::string BRIGHT_BLUE;
    extern const std::string BRIGHT_MAGENTA;
    extern const std::string BRIGHT_CYAN;
    extern const std::string BRIGHT_WHITE;
}

#endif // CONFIG_H