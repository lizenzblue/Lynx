#include "../../../include/plugin.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

/**
 * File Utilities Plugin - Adds enhanced file operations to Lynx Shell
 * Commands: lsa, tree, find, count
 */
class FileUtilsPlugin : public IPlugin {
private:
    PluginInfo info;
    PluginAPI* api;

public:
    FileUtilsPlugin() {
        info.name = "fileutils";
        info.version = "1.0.0";
        info.author = "Lynx Shell Team";
        info.description = "Enhanced file operations and utilities";
        info.apiVersion = 1;
    }

    bool initialize(Shell* shell) override {
        api = new PluginAPI(shell);
        
        // Register commands
        PluginCommand lsaCmd;
        lsaCmd.name = "lsa";
        lsaCmd.description = "List files with detailed information";
        lsaCmd.usage = "lsa [directory]";
        lsaCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleLsaCommand(cmd, shell);
        };
        
        PluginCommand treeCmd;
        treeCmd.name = "tree";
        treeCmd.description = "Display directory tree structure";
        treeCmd.usage = "tree [directory] [depth]";
        treeCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleTreeCommand(cmd, shell);
        };
        
        PluginCommand findCmd;
        findCmd.name = "find";
        findCmd.description = "Find files by name pattern";
        findCmd.usage = "find <pattern> [directory]";
        findCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleFindCommand(cmd, shell);
        };
        
        PluginCommand countCmd;
        countCmd.name = "count";
        countCmd.description = "Count files and directories";
        countCmd.usage = "count [directory]";
        countCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleCountCommand(cmd, shell);
        };
        
        // Register commands with plugin manager
        auto* pluginManager = shell->getPluginManager();
        if (pluginManager) {
            pluginManager->registerCommand(info.name, lsaCmd);
            pluginManager->registerCommand(info.name, treeCmd);
            pluginManager->registerCommand(info.name, findCmd);
            pluginManager->registerCommand(info.name, countCmd);
        }
        
        api->println("File utilities plugin loaded - Added commands: lsa, tree, find, count");
        return true;
    }

    void shutdown() override {
        delete api;
        api = nullptr;
    }

    const PluginInfo& getInfo() const override {
        return info;
    }

private:
    bool handleLsaCommand(const Command& cmd, Shell* shell) {
        std::string directory = cmd.args.empty() ? api->getCurrentDirectory() : cmd.args[0];
        
        if (!api->fileExists(directory)) {
            api->printError("Directory does not exist: " + directory);
            return false;
        }
        
        try {
            api->println("Directory: " + directory);
            api->println("Type    Size    Name");
            api->println("----    ----    ----");
            
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                std::string type = entry.is_directory() ? "DIR " : "FILE";
                std::string size = entry.is_directory() ? "    " : std::to_string(entry.file_size());
                std::string name = entry.path().filename().string();
                
                api->println(type + "    " + size + "    " + name);
            }
            
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            api->printError("Error listing directory: " + std::string(e.what()));
            return false;
        }
    }
    
    bool handleTreeCommand(const Command& cmd, Shell* shell) {
        std::string directory = cmd.args.empty() ? api->getCurrentDirectory() : cmd.args[0];
        int maxDepth = cmd.args.size() > 1 ? std::stoi(cmd.args[1]) : 3;
        
        if (!api->fileExists(directory)) {
            api->printError("Directory does not exist: " + directory);
            return false;
        }
        
        api->println("Directory tree for: " + directory);
        printTree(directory, "", 0, maxDepth);
        return true;
    }
    
    bool handleFindCommand(const Command& cmd, Shell* shell) {
        if (cmd.args.empty()) {
            api->printError("Usage: find <pattern> [directory]");
            return false;
        }
        
        std::string pattern = cmd.args[0];
        std::string directory = cmd.args.size() > 1 ? cmd.args[1] : api->getCurrentDirectory();
        
        if (!api->fileExists(directory)) {
            api->printError("Directory does not exist: " + directory);
            return false;
        }
        
        api->println("Searching for '" + pattern + "' in " + directory);
        
        int count = 0;
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
                std::string filename = entry.path().filename().string();
                if (filename.find(pattern) != std::string::npos) {
                    api->println("  " + entry.path().string());
                    count++;
                }
            }
            
            api->println("Found " + std::to_string(count) + " matching files");
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            api->printError("Error searching: " + std::string(e.what()));
            return false;
        }
    }
    
    bool handleCountCommand(const Command& cmd, Shell* shell) {
        std::string directory = cmd.args.empty() ? api->getCurrentDirectory() : cmd.args[0];
        
        if (!api->fileExists(directory)) {
            api->printError("Directory does not exist: " + directory);
            return false;
        }
        
        int fileCount = 0;
        int dirCount = 0;
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_directory()) {
                    dirCount++;
                } else {
                    fileCount++;
                }
            }
            
            api->println("In " + directory + ":");
            api->println("  Files: " + std::to_string(fileCount));
            api->println("  Directories: " + std::to_string(dirCount));
            api->println("  Total: " + std::to_string(fileCount + dirCount));
            
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            api->printError("Error counting: " + std::string(e.what()));
            return false;
        }
    }
    
    void printTree(const std::string& path, const std::string& prefix, int depth, int maxDepth) {
        if (depth >= maxDepth) return;
        
        try {
            std::vector<std::filesystem::directory_entry> entries;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                entries.push_back(entry);
            }
            
            std::sort(entries.begin(), entries.end(),
                     [](const auto& a, const auto& b) {
                         return a.path().filename() < b.path().filename();
                     });
            
            for (size_t i = 0; i < entries.size(); ++i) {
                const auto& entry = entries[i];
                bool isLast = (i == entries.size() - 1);
                
                std::string connector = isLast ? "└── " : "├── ";
                std::string name = entry.path().filename().string();
                
                api->println(prefix + connector + name);
                
                if (entry.is_directory()) {
                    std::string newPrefix = prefix + (isLast ? "    " : "│   ");
                    printTree(entry.path().string(), newPrefix, depth + 1, maxDepth);
                }
            }
        } catch (const std::filesystem::filesystem_error&) {
            // Skip directories we can't access
        }
    }
};

// Export the plugin
LYNX_PLUGIN_ENTRY_POINT(FileUtilsPlugin)