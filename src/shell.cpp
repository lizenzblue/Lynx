#include "shell.h"
#include "command.h"
#include "utils.h"
#include "config.h"
#include <iostream>
#include <unistd.h>

Shell::Shell() : running(true), lastExitCode(0) {
    currentDirectory = Utils::getCurrentDirectory();
    
    // Initialize configuration system
    configManager = std::make_unique<ConfigManager>();
    
    // Display welcome message if configured
    std::string welcomeMsg = configManager->getSetting("welcome_message");
    if (!welcomeMsg.empty() && configManager->getBoolSetting("show_welcome", true)) {
        std::cout << configManager->getThemeManager()->colorize(welcomeMsg, "info") << std::endl;
    }
}

Shell::~Shell() {
    // Cleanup if needed
}

void Shell::run() {
    while (running) {
        displayPrompt();
        std::string input = readInput();
        
        if (!input.empty()) {
            addToHistory(input);
            executeCommand(input);
        }
    }
}

void Shell::displayPrompt() {
    std::string username = Utils::getUsername();
    std::string hostname = Utils::getHostname();
    std::string cwd = Utils::getCurrentDirectory();
    
    // Use themed prompt
    std::string prompt = configManager->getThemeManager()->formatPrompt(
        username, hostname, cwd, lastExitCode);
    
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
    
    // Expand aliases first
    std::string expandedInput = configManager->getAliasManager()->expandAlias(input);
    
    Command cmd = CommandParser::parseCommand(expandedInput);
    
    lastExitCode = 0;  // Reset exit code
    
    if (CommandExecutor::isBuiltinCommand(cmd.name)) {
        if (!CommandExecutor::executeBuiltinCommand(cmd, this)) {
            lastExitCode = 1;
        }
    } else {
        lastExitCode = CommandExecutor::executeExternalCommand(cmd);
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
