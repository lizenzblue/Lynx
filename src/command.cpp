#include "command.h"
#include "shell.h"
#include "config.h"
#include "utils.h"
#include "version.h"
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
        // Clear screen command
        std::cout << "\033[2J\033[H" << std::flush;
        return true;
    } else if (cmd.name == "version") {
        return executeVersion();
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
           commandName == "clear" || commandName == "version";
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
    std::cout << "  version         - Show version information" << std::endl;
    std::cout << std::endl;
    std::cout << "Configuration is loaded from ~/.lynx/ files at startup." << std::endl;
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

bool CommandExecutor::executeVersion() {
    std::cout << Version::getVersionString() << std::endl;
    return true;
}
