#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

struct Command {
    std::string name;
    std::vector<std::string> args;
    
    Command() = default;
    Command(const std::string& cmdName, const std::vector<std::string>& cmdArgs);
};

class CommandParser {
public:
    static Command parseCommand(const std::string& input);
    static std::vector<std::string> tokenize(const std::string& input);
};

class CommandExecutor {
public:
    static bool executeBuiltinCommand(const Command& cmd);
    static void executeExternalCommand(const Command& cmd);
    static bool isBuiltinCommand(const std::string& commandName);
    
private:
    static bool executeCD(const std::vector<std::string>& args);
    static bool executePWD();
    static bool executeExit();
    static bool executeHelp();
    static bool executeHistory();
    static bool executeEnv();
};

#endif // COMMAND_H
