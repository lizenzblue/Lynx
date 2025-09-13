#include "theme_manager.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <algorithm>

// ANSI Color Utilities Implementation
namespace AnsiColors {
    std::string rgb(int r, int g, int b) {
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    
    std::string rgb256(int colorCode) {
        return "\033[38;5;" + std::to_string(colorCode) + "m";
    }
    
    std::string hexToAnsi(const std::string& hexColor) {
        if (hexColor.length() != 7 || hexColor[0] != '#') {
            return "";
        }
        
        try {
            int r = std::stoi(hexColor.substr(1, 2), nullptr, 16);
            int g = std::stoi(hexColor.substr(3, 2), nullptr, 16);
            int b = std::stoi(hexColor.substr(5, 2), nullptr, 16);
            return rgb(r, g, b);
        } catch (const std::exception&) {
            return "";
        }
    }
    
    bool isTerminalColorSupported() {
        const char* term = getenv("TERM");
        if (!term) return false;
        
        std::string termStr(term);
        return termStr.find("color") != std::string::npos ||
               termStr.find("xterm") != std::string::npos ||
               termStr.find("screen") != std::string::npos;
    }
}

// External Theme Manager Implementation
ExternalThemeManager::ExternalThemeManager() : currentTheme(nullptr) {
    initializeDefaultTheme();
    
    // Add default theme paths
    std::string homeDir = Utils::getHomeDirectory();
    addThemePath(homeDir + "/.lynx/themes");
    addThemePath("/usr/local/share/lynx/themes");
    addThemePath("./themes");
    
    // Set default theme as current
    currentThemeName = "default";
    currentTheme = defaultTheme.get();
}

ExternalThemeManager::~ExternalThemeManager() = default;

void ExternalThemeManager::initializeDefaultTheme() {
    defaultTheme = std::make_unique<ThemeConfig>();
    defaultTheme->name = "default";
    defaultTheme->version = "1.0.0";
    defaultTheme->author = "Lynx Shell";
    defaultTheme->description = "Default Lynx theme";
    
    // Initialize default colors
    defaultTheme->colors.promptPrefix = AnsiColors::BRIGHT_BLUE;
    defaultTheme->colors.promptDirectory = AnsiColors::BRIGHT_CYAN;
    defaultTheme->colors.promptSuffix = AnsiColors::BRIGHT_GREEN;
    defaultTheme->colors.promptInput = AnsiColors::WHITE;
    
    defaultTheme->colors.outputNormal = AnsiColors::WHITE;
    defaultTheme->colors.outputSuccess = AnsiColors::BRIGHT_GREEN;
    defaultTheme->colors.outputWarning = AnsiColors::BRIGHT_YELLOW;
    defaultTheme->colors.outputError = AnsiColors::BRIGHT_RED;
    defaultTheme->colors.outputInfo = AnsiColors::BRIGHT_CYAN;
    
    defaultTheme->colors.syntaxKeyword = AnsiColors::BRIGHT_BLUE;
    defaultTheme->colors.syntaxString = AnsiColors::BRIGHT_GREEN;
    defaultTheme->colors.syntaxNumber = AnsiColors::BRIGHT_MAGENTA;
    defaultTheme->colors.syntaxComment = AnsiColors::BRIGHT_BLACK;
    defaultTheme->colors.syntaxOperator = AnsiColors::BRIGHT_YELLOW;
    
    defaultTheme->colors.reset = AnsiColors::RESET;
    defaultTheme->colors.bold = AnsiColors::BOLD;
    defaultTheme->colors.dim = AnsiColors::DIM;
    defaultTheme->colors.underline = AnsiColors::UNDERLINE;
    
    defaultTheme->promptFormat = "{prefix}[{directory}]{suffix} ";
    defaultTheme->showGitBranch = false;
    defaultTheme->showExitCode = true;
    defaultTheme->showTimestamp = false;
}

void ExternalThemeManager::addThemePath(const std::string& path) {
    if (std::find(themePaths.begin(), themePaths.end(), path) == themePaths.end()) {
        themePaths.push_back(path);
    }
}

void ExternalThemeManager::discoverThemes() {
    for (const auto& path : themePaths) {
        if (!std::filesystem::exists(path)) {
            continue;
        }
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    if (filename.find(".json") != std::string::npos || 
                        filename.find(".ini") != std::string::npos || 
                        filename.find(".theme") != std::string::npos) {
                        loadThemeFromFile(entry.path().string());
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error scanning theme directory " << path << ": " << e.what() << std::endl;
        }
    }
}

bool ExternalThemeManager::loadTheme(const std::string& themePath) {
    return loadThemeFromFile(themePath);
}

bool ExternalThemeManager::loadThemeFromFile(const std::string& themeFile) {
    if (!std::filesystem::exists(themeFile)) {
        return false;
    }
    
    ThemeConfig theme;
    
    std::string ext = std::filesystem::path(themeFile).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    bool success = false;
    if (ext == ".json") {
        success = loadThemeFromJSON(themeFile);
    } else if (ext == ".ini" || ext == ".theme") {
        success = loadThemeFromINI(themeFile);
    } else {
        success = parseThemeFile(themeFile, theme);
    }
    
    if (success && !theme.name.empty()) {
        auto newTheme = std::make_unique<ThemeConfig>(std::move(theme));
        if (validateTheme(*newTheme)) {
            availableThemes[newTheme->name] = std::move(newTheme);
            return true;
        }
    }
    
    return false;
}

bool ExternalThemeManager::loadThemeFromJSON(const std::string& jsonFile) {
    // This is a simplified JSON parser - in a real implementation,
    // you might want to use a proper JSON library like nlohmann/json
    std::ifstream file(jsonFile);
    if (!file.is_open()) {
        return false;
    }
    
    auto theme = std::make_unique<ThemeConfig>();
    std::string line;
    
    // Simple JSON parsing (this is basic - consider using a proper JSON library)
    while (std::getline(file, line)) {
        // Remove whitespace and quotes
        line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");
        line = std::regex_replace(line, std::regex("\""), "");
        
        if (line.find("name:") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                theme->name = line.substr(pos + 1);
                theme->name = std::regex_replace(theme->name, std::regex("^\\s+|\\s+$|,$"), "");
            }
        }
        // Add more JSON parsing logic here...
    }
    
    if (!theme->name.empty() && validateTheme(*theme)) {
        availableThemes[theme->name] = std::move(theme);
        return true;
    }
    
    return false;
}

bool ExternalThemeManager::loadThemeFromINI(const std::string& iniFile) {
    std::ifstream file(iniFile);
    if (!file.is_open()) {
        return false;
    }
    
    auto theme = std::make_unique<ThemeConfig>();
    std::string line;
    std::string currentSection;
    
    while (std::getline(file, line)) {
        // Remove comments and trim whitespace
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");
        
        if (line.empty()) continue;
        
        // Check for section headers
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        
        // Parse key-value pairs
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            
            // Trim key and value
            key = std::regex_replace(key, std::regex("^\\s+|\\s+$"), "");
            value = std::regex_replace(value, std::regex("^\\s+|\\s+$"), "");
            
            // Process based on section
            if (currentSection == "info") {
                if (key == "name") theme->name = value;
                else if (key == "version") theme->version = value;
                else if (key == "author") theme->author = value;
                else if (key == "description") theme->description = value;
            } else if (currentSection == "colors") {
                std::string colorCode = resolveColorCode(value);
                if (key == "prompt_prefix") theme->colors.promptPrefix = colorCode;
                else if (key == "prompt_directory") theme->colors.promptDirectory = colorCode;
                else if (key == "prompt_suffix") theme->colors.promptSuffix = colorCode;
                else if (key == "output_success") theme->colors.outputSuccess = colorCode;
                else if (key == "output_error") theme->colors.outputError = colorCode;
                // Add more color mappings...
            } else if (currentSection == "settings") {
                if (key == "prompt_format") theme->promptFormat = value;
                else if (key == "show_git_branch") theme->showGitBranch = (value == "true");
                else if (key == "show_exit_code") theme->showExitCode = (value == "true");
                else if (key == "show_timestamp") theme->showTimestamp = (value == "true");
            }
        }
    }
    
    if (!theme->name.empty() && validateTheme(*theme)) {
        availableThemes[theme->name] = std::move(theme);
        return true;
    }
    
    return false;
}

bool ExternalThemeManager::setTheme(const std::string& themeName) {
    auto it = availableThemes.find(themeName);
    if (it != availableThemes.end()) {
        currentThemeName = themeName;
        currentTheme = it->second.get();
        return true;
    } else if (themeName == "default") {
        currentThemeName = "default";
        currentTheme = defaultTheme.get();
        return true;
    }
    return false;
}

const ThemeConfig* ExternalThemeManager::getCurrentTheme() const {
    return currentTheme;
}

std::string ExternalThemeManager::getCurrentThemeName() const {
    return currentThemeName;
}

std::vector<std::string> ExternalThemeManager::getAvailableThemes() const {
    std::vector<std::string> themes;
    themes.push_back("default");
    
    for (const auto& [name, theme] : availableThemes) {
        themes.push_back(name);
    }
    
    return themes;
}

const ThemeConfig* ExternalThemeManager::getTheme(const std::string& themeName) const {
    if (themeName == "default") {
        return defaultTheme.get();
    }
    
    auto it = availableThemes.find(themeName);
    return (it != availableThemes.end()) ? it->second.get() : nullptr;
}

void ExternalThemeManager::listThemes() const {
    std::cout << "Available Themes:" << std::endl;
    std::cout << "  default (built-in)" << std::endl;
    
    for (const auto& [name, theme] : availableThemes) {
        std::cout << "  " << name << " v" << theme->version 
                  << " by " << theme->author << std::endl;
        std::cout << "    " << theme->description << std::endl;
    }
}

std::string ExternalThemeManager::applyColor(const std::string& text, const std::string& colorCode) const {
    if (!AnsiColors::isTerminalColorSupported()) {
        return text;
    }
    return colorCode + text + currentTheme->colors.reset;
}

std::string ExternalThemeManager::formatPrompt(const std::string& directory, int exitCode) const {
    if (!currentTheme) {
        return directory + " $ ";
    }
    
    std::string prompt = currentTheme->promptFormat;
    
    // Replace placeholders
    prompt = std::regex_replace(prompt, std::regex("\\{prefix\\}"), currentTheme->colors.promptPrefix);
    prompt = std::regex_replace(prompt, std::regex("\\{directory\\}"), 
                               currentTheme->colors.promptDirectory + directory + currentTheme->colors.reset);
    prompt = std::regex_replace(prompt, std::regex("\\{suffix\\}"), currentTheme->colors.promptSuffix);
    
    if (currentTheme->showExitCode && exitCode != 0) {
        prompt += currentTheme->colors.outputError + "[" + std::to_string(exitCode) + "]" + 
                 currentTheme->colors.reset + " ";
    }
    
    return prompt + currentTheme->colors.reset;
}

bool ExternalThemeManager::validateTheme(const ThemeConfig& theme) const {
    return !theme.name.empty() && 
           !theme.version.empty() &&
           isValidColorCode(theme.colors.promptPrefix) &&
           isValidColorCode(theme.colors.outputNormal);
}

std::vector<std::string> ExternalThemeManager::getThemeErrors(const ThemeConfig& theme) const {
    std::vector<std::string> errors;
    
    if (theme.name.empty()) {
        errors.push_back("Theme name is required");
    }
    if (theme.version.empty()) {
        errors.push_back("Theme version is required");
    }
    if (!isValidColorCode(theme.colors.promptPrefix)) {
        errors.push_back("Invalid prompt prefix color code");
    }
    
    return errors;
}

bool ExternalThemeManager::parseThemeFile(const std::string& filePath, ThemeConfig& theme) {
    // Fallback parser for custom theme file formats
    (void)theme; // Suppress unused parameter warning
    return loadThemeFromINI(filePath);
}

std::string ExternalThemeManager::resolveColorCode(const std::string& colorName) const {
    // Map color names to ANSI codes
    static const std::map<std::string, std::string> colorMap = {
        {"black", AnsiColors::BLACK},
        {"red", AnsiColors::RED},
        {"green", AnsiColors::GREEN},
        {"yellow", AnsiColors::YELLOW},
        {"blue", AnsiColors::BLUE},
        {"magenta", AnsiColors::MAGENTA},
        {"cyan", AnsiColors::CYAN},
        {"white", AnsiColors::WHITE},
        {"bright_black", AnsiColors::BRIGHT_BLACK},
        {"bright_red", AnsiColors::BRIGHT_RED},
        {"bright_green", AnsiColors::BRIGHT_GREEN},
        {"bright_yellow", AnsiColors::BRIGHT_YELLOW},
        {"bright_blue", AnsiColors::BRIGHT_BLUE},
        {"bright_magenta", AnsiColors::BRIGHT_MAGENTA},
        {"bright_cyan", AnsiColors::BRIGHT_CYAN},
        {"bright_white", AnsiColors::BRIGHT_WHITE},
        {"reset", AnsiColors::RESET},
        {"bold", AnsiColors::BOLD},
        {"dim", AnsiColors::DIM},
        {"underline", AnsiColors::UNDERLINE}
    };
    
    auto it = colorMap.find(colorName);
    if (it != colorMap.end()) {
        return it->second;
    }
    
    // Check if it's a hex color
    if (colorName.length() == 7 && colorName[0] == '#') {
        return AnsiColors::hexToAnsi(colorName);
    }
    
    // Return as-is if it's already an ANSI code
    return colorName;
}

bool ExternalThemeManager::isValidColorCode(const std::string& colorCode) const {
    // Basic validation - check if it looks like an ANSI escape sequence
    return colorCode.empty() || 
           colorCode.find("\033[") == 0 || 
           colorCode.find("#") == 0;
}