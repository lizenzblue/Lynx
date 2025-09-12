#include "config.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

AliasManager::AliasManager(ConfigManager* configManager) : config(configManager) {
    // Initialize with some default aliases
    setAlias("ll", "ls -la");
    setAlias("la", "ls -A");
    setAlias("l", "ls -CF");
    setAlias("..", "cd ..");
    setAlias("...", "cd ../..");
    setAlias("cls", "clear");
    setAlias("grep", "grep --color=auto");
    setAlias("egrep", "egrep --color=auto");
    setAlias("fgrep", "fgrep --color=auto");
}

void AliasManager::setAlias(const std::string& name, const std::string& command) {
    aliases[name] = command;
}

void AliasManager::removeAlias(const std::string& name) {
    aliases.erase(name);
}

bool AliasManager::hasAlias(const std::string& name) {
    return aliases.find(name) != aliases.end();
}

std::string AliasManager::getAlias(const std::string& name) {
    auto it = aliases.find(name);
    return (it != aliases.end()) ? it->second : "";
}

std::string AliasManager::expandAlias(const std::string& command) {
    std::vector<std::string> tokens = Utils::split(command, ' ');
    if (tokens.empty()) {
        return command;
    }
    
    std::string firstWord = tokens[0];
    
    // Check if it's an alias
    if (hasAlias(firstWord)) {
        std::string aliasCommand = getAlias(firstWord);
        
        // Replace the first word with the alias expansion
        std::string result = aliasCommand;
        
        // Append remaining arguments
        for (size_t i = 1; i < tokens.size(); ++i) {
            result += " " + tokens[i];
        }
        
        // Recursively expand in case alias contains other aliases
        // Prevent infinite recursion by limiting depth
        static int expansionDepth = 0;
        if (expansionDepth < 10) {
            expansionDepth++;
            result = expandAlias(result);
            expansionDepth--;
        }
        
        return result;
    }
    
    return command;
}

std::vector<std::pair<std::string, std::string>> AliasManager::getAllAliases() {
    std::vector<std::pair<std::string, std::string>> result;
    for (const auto& pair : aliases) {
        result.push_back(pair);
    }
    
    // Sort alphabetically
    std::sort(result.begin(), result.end());
    
    return result;
}

void AliasManager::setFunction(const std::string& name, const std::string& body) {
    functions[name] = body;
}

void AliasManager::removeFunction(const std::string& name) {
    functions.erase(name);
}

bool AliasManager::hasFunction(const std::string& name) {
    return functions.find(name) != functions.end();
}

std::string AliasManager::getFunction(const std::string& name) {
    auto it = functions.find(name);
    return (it != functions.end()) ? it->second : "";
}

bool AliasManager::loadAliases() {
    std::string aliasFile = getAliasFilePath();
    std::ifstream file(aliasFile);
    
    if (!file.is_open()) {
        return false;
    }
    
    aliases.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string name = Utils::trim(line.substr(0, pos));
            std::string command = Utils::trim(line.substr(pos + 1));
            
            // Remove quotes if present
            if (command.length() >= 2 && command.front() == '"' && command.back() == '"') {
                command = command.substr(1, command.length() - 2);
            }
            
            aliases[name] = command;
        }
    }
    
    file.close();
    return true;
}

bool AliasManager::saveAliases() {
    std::string aliasFile = getAliasFilePath();
    std::ofstream file(aliasFile);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Lynx Shell Aliases\n";
    file << "# Format: alias_name=command\n\n";
    
    auto allAliases = getAllAliases();
    for (const auto& pair : allAliases) {
        file << pair.first << "=\"" << pair.second << "\"\n";
    }
    
    file.close();
    return true;
}

bool AliasManager::loadFunctions() {
    std::string functionFile = getFunctionFilePath();
    std::ifstream file(functionFile);
    
    if (!file.is_open()) {
        return false;
    }
    
    functions.clear();
    std::string line;
    std::string currentFunction;
    std::string currentBody;
    bool inFunction = false;
    
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        if (line.find("function ") == 0) {
            // Save previous function if any
            if (inFunction && !currentFunction.empty()) {
                functions[currentFunction] = currentBody;
            }
            
            // Start new function
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos) {
                currentFunction = Utils::trim(line.substr(spacePos + 1));
                currentBody = "";
                inFunction = true;
            }
        } else if (line == "}" && inFunction) {
            // End function
            if (!currentFunction.empty()) {
                functions[currentFunction] = currentBody;
            }
            currentFunction = "";
            currentBody = "";
            inFunction = false;
        } else if (inFunction) {
            // Add line to function body
            if (!currentBody.empty()) {
                currentBody += "\n";
            }
            currentBody += line;
        }
    }
    
    file.close();
    return true;
}

bool AliasManager::saveFunctions() {
    std::string functionFile = getFunctionFilePath();
    std::ofstream file(functionFile);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Lynx Shell Functions\n";
    file << "# Format: function name { body }\n\n";
    
    for (const auto& pair : functions) {
        file << "function " << pair.first << " {\n";
        file << pair.second << "\n";
        file << "}\n\n";
    }
    
    file.close();
    return true;
}

std::string AliasManager::getAliasFilePath() {
    return config->getConfigDir() + "/aliases";
}

std::string AliasManager::getFunctionFilePath() {
    return config->getConfigDir() + "/functions";
}