#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <unordered_map>

class Shell {
private:
    std::vector<std::string> history;
    std::unordered_map<std::string, std::string> environment;
    bool running;
    std::string currentDirectory;

public:
    Shell();
    ~Shell();
    
    void run();
    void displayPrompt();
    std::string readInput();
    void executeCommand(const std::string& input);
    void addToHistory(const std::string& command);
    void printHistory();
    void setEnvironmentVariable(const std::string& key, const std::string& value);
    std::string getEnvironmentVariable(const std::string& key);
    void printEnvironment();
    bool isRunning() const;
    void exit();
};

#endif // SHELL_H
