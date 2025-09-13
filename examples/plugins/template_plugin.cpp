#include "../../include/plugin.h"
#include <iostream>

/**
 * Template Plugin - A template for creating new Lynx Shell plugins
 * 
 * To create a new plugin:
 * 1. Copy this file and rename it to your_plugin_name.cpp
 * 2. Replace "Template" with your plugin name throughout the file
 * 3. Update the plugin info (name, version, author, description)
 * 4. Implement your commands and event handlers
 * 5. Compile with: ./build_plugin.sh your_plugin_name
 */
class TemplatePlugin : public IPlugin {
private:
    PluginInfo info;
    PluginAPI* api;

public:
    TemplatePlugin() {
        // Configure your plugin information
        info.name = "template";
        info.version = "1.0.0";
        info.author = "Your Name";
        info.description = "Template plugin for Lynx Shell";
        info.apiVersion = 1;
    }

    bool initialize(Shell* shell) override {
        api = new PluginAPI(shell);
        
        // Register your commands here
        PluginCommand exampleCmd;
        exampleCmd.name = "example";
        exampleCmd.description = "An example command";
        exampleCmd.usage = "example [argument]";
        exampleCmd.handler = [this](const Command& cmd, Shell* shell) -> bool {
            return handleExampleCommand(cmd, shell);
        };
        
        // Register command with plugin manager
        auto* pluginManager = shell->getPluginManager();
        if (pluginManager) {
            pluginManager->registerCommand(info.name, exampleCmd);
        }
        
        api->println("Template plugin loaded - Added command: example");
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
                // Handle shell startup
                api->println("Template plugin: Shell started!");
                break;
                
            case PluginEvent::SHELL_SHUTDOWN:
                // Handle shell shutdown
                break;
                
            case PluginEvent::COMMAND_BEFORE:
                // Handle before command execution
                // You can access context["command"] and context["args"]
                break;
                
            case PluginEvent::COMMAND_AFTER:
                // Handle after command execution
                // You can access context["exit_code"] and context["success"]
                break;
                
            case PluginEvent::PROMPT_DISPLAY:
                // Handle prompt display
                // You can access context["directory"] and context["exit_code"]
                break;
                
            case PluginEvent::INPUT_RECEIVED:
                // Handle input received
                // You can access context["input"]
                break;
        }
    }

    void configure(const std::map<std::string, std::string>& config) override {
        // Handle plugin configuration
        // You can access configuration values passed to your plugin
        for (const auto& [key, value] : config) {
            api->println("Config: " + key + " = " + value);
        }
    }

private:
    bool handleExampleCommand(const Command& cmd, Shell* shell) {
        api->println("Example command executed!");
        
        if (!cmd.args.empty()) {
            api->println("Arguments received:");
            for (size_t i = 0; i < cmd.args.size(); ++i) {
                api->println("  arg[" + std::to_string(i) + "] = " + cmd.args[i]);
            }
        } else {
            api->println("No arguments provided");
        }
        
        // Access shell information
        api->println("Current directory: " + api->getCurrentDirectory());
        api->println("Last exit code: " + std::to_string(api->getLastExitCode()));
        
        // Access configuration
        std::string configValue = api->getConfigValue("example_setting", "default_value");
        api->println("Config value: " + configValue);
        
        // Return true for success, false for failure
        return true;
    }
};

// Export the plugin (replace TemplatePlugin with your class name)
LYNX_PLUGIN_ENTRY_POINT(TemplatePlugin)