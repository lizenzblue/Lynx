#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include <memory>

/**
 * Theme Color Structure
 */
struct ThemeColors {
    // Prompt colors
    std::string promptPrefix;
    std::string promptDirectory;
    std::string promptSuffix;
    std::string promptInput;
    
    // Output colors
    std::string outputNormal;
    std::string outputSuccess;
    std::string outputWarning;
    std::string outputError;
    std::string outputInfo;
    
    // Syntax highlighting (for future use)
    std::string syntaxKeyword;
    std::string syntaxString;
    std::string syntaxNumber;
    std::string syntaxComment;
    std::string syntaxOperator;
    
    // Special elements
    std::string highlightBackground;
    std::string selectionBackground;
    std::string cursorColor;
    
    // Reset codes
    std::string reset;
    std::string bold;
    std::string dim;
    std::string underline;
};

/**
 * Theme Configuration Structure
 */
struct ThemeConfig {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    ThemeColors colors;
    
    // Additional theme settings
    std::map<std::string, std::string> customSettings;
    
    // Prompt format settings
    std::string promptFormat;
    bool showGitBranch;
    bool showExitCode;
    bool showTimestamp;
};

/**
 * External Theme Manager
 * Handles loading themes from external files without recompilation
 */
class ExternalThemeManager {
private:
    std::map<std::string, std::unique_ptr<ThemeConfig>> availableThemes;
    std::string currentThemeName;
    ThemeConfig* currentTheme;
    std::vector<std::string> themePaths;
    
    // Default fallback theme
    std::unique_ptr<ThemeConfig> defaultTheme;

public:
    ExternalThemeManager();
    ~ExternalThemeManager();
    
    // Theme discovery and loading
    void addThemePath(const std::string& path);
    void discoverThemes();
    bool loadTheme(const std::string& themePath);
    bool loadThemeFromFile(const std::string& themeFile);
    
    // Theme management
    bool setTheme(const std::string& themeName);
    const ThemeConfig* getCurrentTheme() const;
    std::string getCurrentThemeName() const;
    
    // Theme information
    std::vector<std::string> getAvailableThemes() const;
    const ThemeConfig* getTheme(const std::string& themeName) const;
    void listThemes() const;
    
    // Theme creation and export
    bool createTheme(const std::string& themeName, const ThemeConfig& config);
    bool exportTheme(const std::string& themeName, const std::string& outputPath) const;
    
    // Color utilities
    std::string applyColor(const std::string& text, const std::string& colorCode) const;
    std::string formatPrompt(const std::string& directory, int exitCode = 0) const;
    
    // File format support
    bool loadThemeFromJSON(const std::string& jsonFile);
    bool loadThemeFromINI(const std::string& iniFile);
    bool saveThemeToJSON(const std::string& themeName, const std::string& outputFile) const;
    bool saveThemeToINI(const std::string& themeName, const std::string& outputFile) const;
    
    // Validation
    bool validateTheme(const ThemeConfig& theme) const;
    std::vector<std::string> getThemeErrors(const ThemeConfig& theme) const;
    
private:
    void initializeDefaultTheme();
    bool parseThemeFile(const std::string& filePath, ThemeConfig& theme);
    std::string resolveColorCode(const std::string& colorName) const;
    bool isValidColorCode(const std::string& colorCode) const;
};

/**
 * ANSI Color Codes and Utilities
 */
namespace AnsiColors {
    // Basic colors
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    
    // Bright colors
    const std::string BRIGHT_BLACK = "\033[90m";
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";
    
    // Background colors
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
    
    // Style codes
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string UNDERLINE = "\033[4m";
    const std::string BLINK = "\033[5m";
    const std::string REVERSE = "\033[7m";
    
    // Utility functions
    std::string rgb(int r, int g, int b);
    std::string rgb256(int colorCode);
    std::string hexToAnsi(const std::string& hexColor);
    bool isTerminalColorSupported();
}

#endif // THEME_MANAGER_H