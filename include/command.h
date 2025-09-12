#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

// Forward declaration
class Shell;

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
    static bool executeBuiltinCommand(const Command& cmd, Shell* shell = nullptr);
    static int executeExternalCommand(const Command& cmd);
    static bool isBuiltinCommand(const std::string& commandName);
    
private:
    static bool executeCD(const std::vector<std::string>& args);
    static bool executePWD();
    static bool executeExit();
    static bool executeHelp();
    static bool executeHistory(Shell* shell);
    static bool executeEnv();
    static bool executeSet(const std::vector<std::string>& args, Shell* shell);
    static bool executeAlias(const std::vector<std::string>& args, Shell* shell);
    static bool executeTheme(const std::vector<std::string>& args, Shell* shell);
};

#endif // COMMAND_H
