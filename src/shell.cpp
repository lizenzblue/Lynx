#include "shell.h"
#include "command.h"
#include "utils.h"
#include "config.h"
#include "plugin.h"
#include "theme_manager.h"
#include <iostream>
#include <unistd.h>

Shell::Shell() : running(true), lastExitCode(0) {
    currentDirectory = Utils::getCurrentDirectory();
    
    // Initialize configuration system
    configManager = std::make_unique<ConfigManager>();
    
    // Initialize theme system
    themeManager = std::make_unique<ExternalThemeManager>();
    themeManager->discoverThemes();
    
    // Load theme from config
    std::string themeName = configManager->getSetting("theme", "default");
    themeManager->setTheme(themeName);
    
    // Initialize plugin system
    pluginManager = std::make_unique<PluginManager>(this);
    pluginManager->loadAllPlugins();
    
    // Display welcome message if configured
    std::string welcomeMsg = configManager->getSetting("welcome_message", "Welcome to Lynx Shell!");
    if (configManager->getSetting("show_welcome", "true") == "true") {
        const ThemeConfig* theme = themeManager->getCurrentTheme();
        if (theme) {
            std::cout << themeManager->applyColor(welcomeMsg, theme->colors.outputInfo) << std::endl;
        } else {
            std::cout << welcomeMsg << std::endl;
        }
    }
}

Shell::~Shell() {
    // Unload plugins before destruction
    if (pluginManager) {
        pluginManager->unloadAllPlugins();
    }
}

void Shell::run() {
    // Broadcast shell startup event to plugins
    if (pluginManager) {
        pluginManager->broadcastEvent(PluginEvent::SHELL_STARTUP);
    }
    
    while (running) {
        displayPrompt();
        std::string input = readInput();
        
        if (!input.empty()) {
            addToHistory(input);
            executeCommand(input);
        }
    }
    
    // Broadcast shell shutdown event to plugins
    if (pluginManager) {
        pluginManager->broadcastEvent(PluginEvent::SHELL_SHUTDOWN);
    }
}

void Shell::displayPrompt() {
    // Broadcast prompt display event to plugins
    if (pluginManager) {
        std::map<std::string, std::string> context;
        context["directory"] = Utils::getCurrentDirectory();
        context["exit_code"] = std::to_string(lastExitCode);
        pluginManager->broadcastEvent(PluginEvent::PROMPT_DISPLAY, context);
    }
    
    std::string cwd = Utils::getCurrentDirectory();
    
    // Use themed prompt
    std::string prompt;
    if (themeManager) {
        prompt = themeManager->formatPrompt(cwd, lastExitCode);
    } else {
        prompt = cwd + " $ ";
    }
    
    std::cout << prompt;
}

std::string Shell::readInput() {
    std::string input;
    std::getline(std::cin, input);
    
    // Handle Ctrl+D (EOF)
    if (std::cin.eof()) {
        running = false;
        return "";
    }
    
    return Utils::trim(input);
}

void Shell::executeCommand(const std::string& input) {
    if (input.empty()) return;
    
    // Broadcast input received event to plugins
    if (pluginManager) {
        std::map<std::string, std::string> context;
        context["input"] = input;
        pluginManager->broadcastEvent(PluginEvent::INPUT_RECEIVED, context);
    }
    
    // Expand aliases first (if alias manager exists)
    std::string expandedInput = input;
    // TODO: Add alias expansion when alias manager is implemented
    
    Command cmd = CommandParser::parseCommand(expandedInput);
    
    // Broadcast command before event to plugins
    if (pluginManager) {
        std::map<std::string, std::string> context;
        context["command"] = cmd.name;
        context["args"] = "";
        for (const auto& arg : cmd.args) {
            context["args"] += arg + " ";
        }
        pluginManager->broadcastEvent(PluginEvent::COMMAND_BEFORE, context);
    }
    
    lastExitCode = 0;  // Reset exit code
    bool commandExecuted = false;
    
    // First check if it's a plugin command
    if (pluginManager && pluginManager->isPluginCommand(cmd.name)) {
        commandExecuted = pluginManager->executePluginCommand(cmd);
        if (!commandExecuted) {
            lastExitCode = 1;
        }
    }
    // Then check built-in commands
    else if (CommandExecutor::isBuiltinCommand(cmd.name)) {
        commandExecuted = CommandExecutor::executeBuiltinCommand(cmd, this);
        if (!commandExecuted) {
            lastExitCode = 1;
        }
    }
    // Finally try external commands
    else {
        lastExitCode = CommandExecutor::executeExternalCommand(cmd);
        commandExecuted = (lastExitCode == 0);
    }
    
    // Broadcast command after event to plugins
    if (pluginManager) {
        std::map<std::string, std::string> context;
        context["command"] = cmd.name;
        context["exit_code"] = std::to_string(lastExitCode);
        context["success"] = commandExecuted ? "true" : "false";
        pluginManager->broadcastEvent(PluginEvent::COMMAND_AFTER, context);
    }
}

void Shell::addToHistory(const std::string& command) {
    history.push_back(command);
}

void Shell::printHistory() {
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << ": " << history[i] << std::endl;
    }
}

bool Shell::isRunning() const {
    return running;
}

void Shell::exit() {
    running = false;
}
