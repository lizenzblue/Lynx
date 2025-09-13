#include "../../../include/plugin.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

/**
 * Time Plugin - Adds time-related commands to Lynx Shell
 * Commands: time, date, uptime, timer
 */
class TimePlugin : public IPlugin {
private:
    PluginInfo info;
    PluginAPI* api;
    std::time_t startTime;

public:
    TimePlugin() {
        info.name = "time";
        info.version = "1.0.0";
        info.author = "Lynx Shell Team";
        info.description = "Time and date utilities plugin";
        info.apiVersion = 1;
        
        startTime = std::time(nullptr);
    }

    bool initialize(Shell* shell) override {
        api = new PluginAPI(shell);
        
        // Register commands
        PluginCommand timeCmd;
        timeCmd.name = "time";
        timeCmd.description = "Display current time";
        timeCmd.usage = "time [format]";
        timeCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleTimeCommand(cmd, shell);
        };
        
        PluginCommand dateCmd;
        dateCmd.name = "date";
        dateCmd.description = "Display current date";
        dateCmd.usage = "date [format]";
        dateCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleDateCommand(cmd, shell);
        };
        
        PluginCommand uptimeCmd;
        uptimeCmd.name = "uptime";
        uptimeCmd.description = "Display shell uptime";
        uptimeCmd.usage = "uptime";
        uptimeCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleUptimeCommand(cmd, shell);
        };
        
        // Register commands with plugin manager
        auto* pluginManager = shell->getPluginManager();
        if (pluginManager) {
            pluginManager->registerCommand(info.name, timeCmd);
            pluginManager->registerCommand(info.name, dateCmd);
            pluginManager->registerCommand(info.name, uptimeCmd);
        }
        
        api->println("Time plugin loaded - Added commands: time, date, uptime");
        return true;
    }

    void shutdown() override {
        delete api;
        api = nullptr;
    }

    const PluginInfo& getInfo() const override {
        return info;
    }

    void onEvent(PluginEvent event, const std::map<std::string, std::string>& context) override {
        switch (event) {
            case PluginEvent::SHELL_STARTUP:
                api->println("🕐 Time plugin ready!");
                break;
            case PluginEvent::COMMAND_BEFORE:
                if (context.find("command") != context.end()) {
                    std::string cmd = context.at("command");
                    if (cmd == "time" || cmd == "date" || cmd == "uptime") {
                        // Log time command usage
                        std::time_t now = std::time(nullptr);
                        api->setConfigValue("time_plugin_last_used", std::to_string(now));
                    }
                }
                break;
            default:
                break;
        }
    }

private:
    bool handleTimeCommand(const Command& cmd, Shell* shell) {
        std::time_t now = std::time(nullptr);
        std::string format = "%H:%M:%S";
        
        if (!cmd.args.empty()) {
            format = cmd.args[0];
        }
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), format.c_str());
        
        api->println("Current time: " + ss.str());
        return true;
    }
    
    bool handleDateCommand(const Command& cmd, Shell* shell) {
        std::time_t now = std::time(nullptr);
        std::string format = "%Y-%m-%d";
        
        if (!cmd.args.empty()) {
            format = cmd.args[0];
        }
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), format.c_str());
        
        api->println("Current date: " + ss.str());
        return true;
    }
    
    bool handleUptimeCommand(const Command& cmd, Shell* shell) {
        std::time_t now = std::time(nullptr);
        double uptime = std::difftime(now, startTime);
        
        int hours = static_cast<int>(uptime) / 3600;
        int minutes = (static_cast<int>(uptime) % 3600) / 60;
        int seconds = static_cast<int>(uptime) % 60;
        
        std::stringstream ss;
        ss << "Shell uptime: " << hours << "h " << minutes << "m " << seconds << "s";
        
        api->println(ss.str());
        return true;
    }
};

// Export the plugin
LYNX_PLUGIN_ENTRY_POINT(TimePlugin)