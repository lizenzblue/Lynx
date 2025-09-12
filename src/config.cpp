#include "config.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

// Color constants
namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BRIGHT_BLACK = "\033[90m";
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";
}

ConfigManager::ConfigManager() {
    initializePaths();
    createConfigDirectory();
    
    themeManager = std::make_unique<ThemeManager>(this);
    aliasManager = std::make_unique<AliasManager>(this);
    
    // Load configuration or create defaults
    if (!loadConfig()) {
        createDefaultConfig();
        loadConfig();
    }
}

ConfigManager::~ConfigManager() = default;

void ConfigManager::initializePaths() {
    const char* home = getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        home = pw ? pw->pw_dir : "/tmp";
    }
    
    configDirPath = std::string(home) + "/.lynx";
    configFilePath = configDirPath + "/config";
}

void ConfigManager::createConfigDirectory() {
    struct stat st;
    if (stat(configDirPath.c_str(), &st) == -1) {
        mkdir(configDirPath.c_str(), 0755);
        mkdir((configDirPath + "/themes").c_str(), 0755);
        mkdir((configDirPath + "/plugins").c_str(), 0755);
    }
}

bool ConfigManager::loadConfig() {
    std::ifstream file(configFilePath);
    if (!file.is_open()) {
        return false;
    }
    
    settings.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        auto pair = parseLine(line);
        if (!pair.first.empty()) {
            settings[pair.first] = pair.second;
        }
    }
    
    file.close();
    
    // Load theme and aliases
    themeManager->loadTheme(getSetting("theme", "default"));
    aliasManager->loadAliases();
    aliasManager->loadFunctions();
    
    return true;
}

bool ConfigManager::saveConfig() {
    std::ofstream file(configFilePath);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Lynx Shell Configuration File\n";
    file << "# Generated automatically - you can edit this file\n\n";
    
    // Write all settings
    for (const auto& pair : settings) {
        file << pair.first << "=" << pair.second << "\n";
    }
    
    file.close();
    
    // Save theme and aliases
    aliasManager->saveAliases();
    aliasManager->saveFunctions();
    
    return true;
}

bool ConfigManager::createDefaultConfig() {
    // Default settings
    setSetting("theme", "default");
    setSetting("prompt_format", "{user}@{host}:{cwd}$ ");
    setSetting("history_size", "1000");
    setSetting("auto_cd", "true");
    setSetting("case_sensitive", "false");
    setSetting("tab_completion", "true");
    setSetting("color_output", "true");
    setSetting("welcome_message", "Welcome to Lynx Shell! Type 'help' for commands.");
    setSetting("exit_on_eof", "true");
    setSetting("command_timeout", "30");
    
    return saveConfig();
}

void ConfigManager::setSetting(const std::string& key, const std::string& value) {
    if (validateSetting(key, value)) {
        settings[key] = value;
    }
}

std::string ConfigManager::getSetting(const std::string& key, const std::string& defaultValue) {
    auto it = settings.find(key);
    return (it != settings.end()) ? it->second : defaultValue;
}

bool ConfigManager::hasSetting(const std::string& key) {
    return settings.find(key) != settings.end();
}

void ConfigManager::removeSetting(const std::string& key) {
    settings.erase(key);
}

int ConfigManager::getIntSetting(const std::string& key, int defaultValue) {
    std::string value = getSetting(key);
    if (value.empty()) return defaultValue;
    
    try {
        return std::stoi(value);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

bool ConfigManager::getBoolSetting(const std::string& key, bool defaultValue) {
    std::string value = getSetting(key);
    if (value.empty()) return defaultValue;
    
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value == "true" || value == "yes" || value == "1" || value == "on";
}

std::vector<std::string> ConfigManager::getListSetting(const std::string& key) {
    std::string value = getSetting(key);
    if (value.empty()) return {};
    
    return Utils::split(value, ',');
}

bool ConfigManager::validateSetting(const std::string& key, const std::string& value) {
    // Add validation rules here
    if (key == "history_size") {
        try {
            int size = std::stoi(value);
            return size >= 0 && size <= 10000;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    if (key == "command_timeout") {
        try {
            int timeout = std::stoi(value);
            return timeout >= 0 && timeout <= 3600;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    // Most settings are valid by default
    return true;
}

std::pair<std::string, std::string> ConfigManager::parseLine(const std::string& line) {
    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        return {"", ""};
    }
    
    std::string key = Utils::trim(line.substr(0, pos));
    std::string value = Utils::trim(line.substr(pos + 1));
    
    // Remove quotes if present
    if (value.length() >= 2 && 
        ((value.front() == '"' && value.back() == '"') ||
         (value.front() == '\'' && value.back() == '\''))) {
        value = value.substr(1, value.length() - 2);
    }
    
    // Expand environment variables
    value = expandPath(value);
    
    return {key, value};
}

std::string ConfigManager::expandPath(const std::string& path) {
    std::string result = path;
    
    // Replace ~ with home directory
    if (result.front() == '~') {
        const char* home = getenv("HOME");
        if (home) {
            result = std::string(home) + result.substr(1);
        }
    }
    
    return result;
}