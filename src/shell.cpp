#include "shell.h"
#include "command.h"
#include "utils.h"
#include <iostream>
#include <unistd.h>

Shell::Shell() : running(true) {
    currentDirectory = Utils::getCurrentDirectory();
    
    // Initialize some basic environment variables
    setEnvironmentVariable("SHELL", "lynx");
    setEnvironmentVariable("USER", Utils::getUsername());
    setEnvironmentVariable("PWD", currentDirectory);
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
    
    // Simple prompt format: user@hostname:current_dir$
    std::cout << username << "@" << hostname << ":" << cwd << "$ ";
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
    
    Command cmd = CommandParser::parseCommand(input);
    
    if (CommandExecutor::isBuiltinCommand(cmd.name)) {
        CommandExecutor::executeBuiltinCommand(cmd);
    } else {
        CommandExecutor::executeExternalCommand(cmd);
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

void Shell::setEnvironmentVariable(const std::string& key, const std::string& value) {
    environment[key] = value;
    Utils::setEnvVar(key, value);
}

std::string Shell::getEnvironmentVariable(const std::string& key) {
    auto it = environment.find(key);
    if (it != environment.end()) {
        return it->second;
    }
    return Utils::getEnvVar(key);
}

void Shell::printEnvironment() {
    for (const auto& pair : environment) {
        std::cout << pair.first << "=" << pair.second << std::endl;
    }
}

bool Shell::isRunning() const {
    return running;
}

void Shell::exit() {
    running = false;
}
