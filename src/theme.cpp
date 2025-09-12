#include "config.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

ThemeManager::ThemeManager(ConfigManager* configManager) : config(configManager) {
    loadDefaultThemes();
    currentTheme = "default";
}

bool ThemeManager::loadTheme(const std::string& themeName) {
    std::string themeFile = config->getConfigDir() + "/themes/" + themeName + ".theme";
    
    std::ifstream file(themeFile);
    if (!file.is_open()) {
        // If theme file doesn't exist, use default theme
        if (themeName != "default") {
            return loadTheme("default");
        }
        return false;
    }
    
    std::unordered_map<std::string, std::string> theme;
    std::string line;
    
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = Utils::trim(line.substr(0, pos));
            std::string value = Utils::trim(line.substr(pos + 1));
            
            // Remove quotes
            if (value.length() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }
            
            theme[key] = value;
        }
    }
    
    file.close();
    themes[themeName] = theme;
    currentTheme = themeName;
    return true;
}

bool ThemeManager::saveTheme(const std::string& themeName) {
    auto it = themes.find(themeName);
    if (it == themes.end()) {
        return false;
    }
    
    std::string themeFile = config->getConfigDir() + "/themes/" + themeName + ".theme";
    std::ofstream file(themeFile);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Lynx Shell Theme: " << themeName << "\n";
    file << "# Color and formatting configuration\n\n";
    
    for (const auto& pair : it->second) {
        file << pair.first << "=\"" << pair.second << "\"\n";
    }
    
    file.close();
    return true;
}

std::vector<std::string> ThemeManager::getAvailableThemes() {
    std::vector<std::string> themeList;
    
    // Add loaded themes
    for (const auto& pair : themes) {
        themeList.push_back(pair.first);
    }
    
    return themeList;
}

void ThemeManager::setCurrentTheme(const std::string& themeName) {
    if (loadTheme(themeName)) {
        currentTheme = themeName;
        config->setSetting("theme", themeName);
    }
}

std::string ThemeManager::getColor(const std::string& element) {
    auto themeIt = themes.find(currentTheme);
    if (themeIt == themes.end()) {
        return "";
    }
    
    auto colorIt = themeIt->second.find(element + "_color");
    if (colorIt != themeIt->second.end()) {
        return getANSICode(colorIt->second);
    }
    
    return "";
}

std::string ThemeManager::getPromptFormat() {
    auto themeIt = themes.find(currentTheme);
    if (themeIt != themes.end()) {
        auto formatIt = themeIt->second.find("prompt_format");
        if (formatIt != themeIt->second.end()) {
            return formatIt->second;
        }
    }
    
    return config->getSetting("prompt_format", "{user}@{host}:{cwd}$ ");
}

std::string ThemeManager::formatPrompt(const std::string& user, const std::string& host, 
                                     const std::string& cwd, int exitCode) {
    std::string format = getPromptFormat();
    std::string result = format;
    
    // Get colors
    std::string userColor = getColor("prompt_user");
    std::string hostColor = getColor("prompt_host");
    std::string cwdColor = getColor("prompt_cwd");
    std::string symbolColor = getColor("prompt_symbol");
    std::string errorColor = getColor("prompt_error");
    
    // Replace placeholders
    size_t pos = 0;
    while ((pos = result.find("{user}", pos)) != std::string::npos) {
        result.replace(pos, 6, userColor + user + resetColor());
        pos += userColor.length() + user.length() + resetColor().length();
    }
    
    pos = 0;
    while ((pos = result.find("{host}", pos)) != std::string::npos) {
        result.replace(pos, 6, hostColor + host + resetColor());
        pos += hostColor.length() + host.length() + resetColor().length();
    }
    
    pos = 0;
    while ((pos = result.find("{cwd}", pos)) != std::string::npos) {
        // Shorten path if too long
        std::string displayCwd = cwd;
        if (displayCwd.length() > 40) {
            displayCwd = "..." + displayCwd.substr(displayCwd.length() - 37);
        }
        result.replace(pos, 5, cwdColor + displayCwd + resetColor());
        pos += cwdColor.length() + displayCwd.length() + resetColor().length();
    }
    
    // Add exit code if non-zero
    if (exitCode != 0) {
        pos = result.find("$ ");
        if (pos != std::string::npos) {
            std::string errorInfo = errorColor + "[" + std::to_string(exitCode) + "]" + resetColor();
            result.insert(pos, errorInfo + " ");
        }
    }
    
    // Colorize the prompt symbol
    pos = result.rfind("$ ");
    if (pos != std::string::npos) {
        result.replace(pos, 2, symbolColor + "$ " + resetColor());
    }
    
    return result;
}

std::string ThemeManager::colorize(const std::string& text, const std::string& color) {
    return getANSICode(color) + text + resetColor();
}

std::string ThemeManager::resetColor() {
    return Colors::RESET;
}

void ThemeManager::loadDefaultThemes() {
    // Default theme
    std::unordered_map<std::string, std::string> defaultTheme = {
        {"prompt_format", "{user}@{host}:{cwd}$ "},
        {"prompt_user_color", "green"},
        {"prompt_host_color", "blue"},
        {"prompt_cwd_color", "cyan"},
        {"prompt_symbol_color", "white"},
        {"prompt_error_color", "red"},
        {"command_color", "white"},
        {"output_color", "white"},
        {"error_color", "red"},
        {"success_color", "green"},
        {"warning_color", "yellow"},
        {"info_color", "blue"}
    };
    themes["default"] = defaultTheme;
    
    // Dark theme
    std::unordered_map<std::string, std::string> darkTheme = {
        {"prompt_format", "┌─[{user}@{host}]─[{cwd}]\n└─$ "},
        {"prompt_user_color", "bright_green"},
        {"prompt_host_color", "bright_blue"},
        {"prompt_cwd_color", "bright_cyan"},
        {"prompt_symbol_color", "bright_white"},
        {"prompt_error_color", "bright_red"},
        {"command_color", "bright_white"},
        {"output_color", "bright_white"},
        {"error_color", "bright_red"},
        {"success_color", "bright_green"},
        {"warning_color", "bright_yellow"},
        {"info_color", "bright_blue"}
    };
    themes["dark"] = darkTheme;
    
    // Minimal theme
    std::unordered_map<std::string, std::string> minimalTheme = {
        {"prompt_format", "$ "},
        {"prompt_symbol_color", "white"},
        {"command_color", "white"},
        {"output_color", "white"},
        {"error_color", "red"},
        {"success_color", "green"},
        {"warning_color", "yellow"},
        {"info_color", "blue"}
    };
    themes["minimal"] = minimalTheme;
    
    // Rainbow theme
    std::unordered_map<std::string, std::string> rainbowTheme = {
        {"prompt_format", "🌈 {user}@{host}:{cwd} ➜ "},
        {"prompt_user_color", "magenta"},
        {"prompt_host_color", "yellow"},
        {"prompt_cwd_color", "cyan"},
        {"prompt_symbol_color", "green"},
        {"prompt_error_color", "red"},
        {"command_color", "bright_white"},
        {"output_color", "white"},
        {"error_color", "bright_red"},
        {"success_color", "bright_green"},
        {"warning_color", "bright_yellow"},
        {"info_color", "bright_blue"}
    };
    themes["rainbow"] = rainbowTheme;
}

std::string ThemeManager::getANSICode(const std::string& colorName) {
    std::string lowerColor = colorName;
    std::transform(lowerColor.begin(), lowerColor.end(), lowerColor.begin(), ::tolower);
    
    if (lowerColor == "black") return Colors::BLACK;
    if (lowerColor == "red") return Colors::RED;
    if (lowerColor == "green") return Colors::GREEN;
    if (lowerColor == "yellow") return Colors::YELLOW;
    if (lowerColor == "blue") return Colors::BLUE;
    if (lowerColor == "magenta") return Colors::MAGENTA;
    if (lowerColor == "cyan") return Colors::CYAN;
    if (lowerColor == "white") return Colors::WHITE;
    if (lowerColor == "bright_black") return Colors::BRIGHT_BLACK;
    if (lowerColor == "bright_red") return Colors::BRIGHT_RED;
    if (lowerColor == "bright_green") return Colors::BRIGHT_GREEN;
    if (lowerColor == "bright_yellow") return Colors::BRIGHT_YELLOW;
    if (lowerColor == "bright_blue") return Colors::BRIGHT_BLUE;
    if (lowerColor == "bright_magenta") return Colors::BRIGHT_MAGENTA;
    if (lowerColor == "bright_cyan") return Colors::BRIGHT_CYAN;
    if (lowerColor == "bright_white") return Colors::BRIGHT_WHITE;
    
    return ""; // Default to no color
}