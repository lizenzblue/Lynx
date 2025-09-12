#include "command.h"
#include "shell.h"
#include "config.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

Command::Command(const std::string& cmdName, const std::vector<std::string>& cmdArgs)
    : name(cmdName), args(cmdArgs) {}

Command CommandParser::parseCommand(const std::string& input) {
    std::vector<std::string> tokens = tokenize(input);
    
    if (tokens.empty()) {
        return Command();
    }
    
    std::string commandName = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());
    
    return Command(commandName, args);
}

std::vector<std::string> CommandParser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool CommandExecutor::executeBuiltinCommand(const Command& cmd, Shell* shell) {
    if (cmd.name == "cd") {
        return executeCD(cmd.args);
    } else if (cmd.name == "pwd") {
        return executePWD();
    } else if (cmd.name == "exit") {
        return executeExit();
    } else if (cmd.name == "help") {
        return executeHelp();
    } else if (cmd.name == "history") {
        return executeHistory(shell);
    } else if (cmd.name == "env") {
        return executeEnv();
    } else if (cmd.name == "clear") {
        Utils::clearScreen();
        return true;
    } else if (cmd.name == "set") {
        return executeSet(cmd.args, shell);
    } else if (cmd.name == "alias") {
        return executeAlias(cmd.args, shell);
    } else if (cmd.name == "theme") {
        return executeTheme(cmd.args, shell);
    }
    
    return false;
}

int CommandExecutor::executeExternalCommand(const Command& cmd) {
    // Prepare arguments for execvp
    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(cmd.name.c_str()));
    
    for (const auto& arg : cmd.args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        if (execvp(argv[0], argv.data()) == -1) {
            std::cerr << "lynx: command not found: " << cmd.name << std::endl;
            exit(127);
        }
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        // Fork failed
        std::cerr << "lynx: failed to fork process" << std::endl;
        return 1;
    }
    
    return 0;
}

bool CommandExecutor::isBuiltinCommand(const std::string& commandName) {
    return commandName == "cd" || commandName == "pwd" || commandName == "exit" ||
           commandName == "help" || commandName == "history" || commandName == "env" ||
           commandName == "clear" || commandName == "set" || commandName == "alias" ||
           commandName == "theme";
}

bool CommandExecutor::executeCD(const std::vector<std::string>& args) {
    std::string path;
    
    if (args.empty()) {
        // No argument, go to home directory
        path = Utils::getEnvVar("HOME");
    } else {
        path = args[0];
    }
    
    if (Utils::changeDirectory(path)) {
        Utils::setEnvVar("PWD", Utils::getCurrentDirectory());
        return true;
    } else {
        std::cerr << "lynx: cd: " << path << ": No such file or directory" << std::endl;
        return false;
    }
}

bool CommandExecutor::executePWD() {
    std::cout << Utils::getCurrentDirectory() << std::endl;
    return true;
}

bool CommandExecutor::executeExit() {
    exit(0);
    return true;
}

bool CommandExecutor::executeHelp() {
    std::cout << "Lynx Shell - Available Commands:" << std::endl;
    std::cout << "  cd <directory>  - Change directory" << std::endl;
    std::cout << "  pwd             - Print working directory" << std::endl;
    std::cout << "  exit            - Exit the shell" << std::endl;
    std::cout << "  help            - Show this help message" << std::endl;
    std::cout << "  history         - Show command history" << std::endl;
    std::cout << "  env             - Display environment variables" << std::endl;
    std::cout << "  clear           - Clear the screen" << std::endl;
    std::cout << "  set <key> <val> - Set configuration option" << std::endl;
    std::cout << "  alias [name]    - List or show specific alias" << std::endl;
    std::cout << "  theme [name]    - List or set theme" << std::endl;
    std::cout << std::endl;
    std::cout << "You can also run any external command available in your PATH." << std::endl;
    return true;
}

bool CommandExecutor::executeHistory(Shell* shell) {
    if (!shell) {
        std::cout << "History functionality requires shell context" << std::endl;
        return false;
    }
    
    const auto& history = shell->getHistory();
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << ": " << history[i] << std::endl;
    }
    return true;
}

bool CommandExecutor::executeEnv() {
    extern char **environ;
    for (char **env = environ; *env != nullptr; env++) {
        std::cout << *env << std::endl;
    }
    return true;
}

bool CommandExecutor::executeSet(const std::vector<std::string>& args, Shell* shell) {
    if (!shell) {
        std::cout << "Set command requires shell context" << std::endl;
        return false;
    }
    
    ConfigManager* config = shell->getConfigManager();
    
    if (args.empty()) {
        // Show some basic configuration help
        std::cout << "Configuration settings:" << std::endl;
        std::cout << "  theme            - Current color theme" << std::endl;
        std::cout << "  prompt_format    - Prompt display format" << std::endl;
        std::cout << "  history_size     - Command history size" << std::endl;
        std::cout << "  welcome_message  - Startup message" << std::endl;
        std::cout << "  color_output     - Enable colored output" << std::endl;
        std::cout << std::endl;
        std::cout << "Usage: set <key> <value>  or  set <key> to view" << std::endl;
        return true;
    } else if (args.size() == 1) {
        // Show specific setting
        std::string value = config->getSetting(args[0], "");
        if (value.empty()) {
            std::cout << "Setting '" << args[0] << "' not found" << std::endl;
            return false;
        } else {
            std::cout << args[0] << "=" << value << std::endl;
        }
    } else if (args.size() == 2) {
        // Set a value
        config->setSetting(args[0], args[1]);
        config->saveConfig();
        std::cout << "Set " << args[0] << "=" << args[1] << std::endl;
    } else {
        std::cout << "Usage: set [key] [value]" << std::endl;
        return false;
    }
    
    return true;
}

bool CommandExecutor::executeAlias(const std::vector<std::string>& args, Shell* shell) {
    if (!shell) {
        std::cout << "Alias command requires shell context" << std::endl;
        return false;
    }
    
    AliasManager* aliasManager = shell->getConfigManager()->getAliasManager();
    
    if (args.empty()) {
        // List all aliases
        auto aliases = aliasManager->getAllAliases();
        std::cout << "Current aliases:" << std::endl;
        for (const auto& pair : aliases) {
            std::cout << "  " << pair.first << "=" << pair.second << std::endl;
        }
    } else if (args.size() == 1) {
        // Show specific alias
        if (aliasManager->hasAlias(args[0])) {
            std::cout << args[0] << "=" << aliasManager->getAlias(args[0]) << std::endl;
        } else {
            std::cout << "Alias '" << args[0] << "' not found" << std::endl;
            return false;
        }
    } else {
        // Set alias: alias name=command
        std::string name = args[0];
        std::string command = Utils::join(std::vector<std::string>(args.begin() + 1, args.end()), " ");
        aliasManager->setAlias(name, command);
        aliasManager->saveAliases();
        std::cout << "Set alias: " << name << "=" << command << std::endl;
    }
    
    return true;
}

bool CommandExecutor::executeTheme(const std::vector<std::string>& args, Shell* shell) {
    if (!shell) {
        std::cout << "Theme command requires shell context" << std::endl;
        return false;
    }
    
    ThemeManager* themeManager = shell->getConfigManager()->getThemeManager();
    
    if (args.empty()) {
        // List available themes
        auto themes = themeManager->getAvailableThemes();
        std::cout << "Available themes:" << std::endl;
        for (const auto& theme : themes) {
            if (theme == themeManager->getCurrentTheme()) {
                std::cout << "* " << theme << " (current)" << std::endl;
            } else {
                std::cout << "  " << theme << std::endl;
            }
        }
    } else {
        // Set theme
        std::string themeName = args[0];
        themeManager->setCurrentTheme(themeName);
        shell->getConfigManager()->saveConfig();
        std::cout << "Theme set to: " << themeName << std::endl;
    }
    
    return true;
}
