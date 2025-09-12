# Lynx Shell Customization Guide

## 🎨 Complete Customization System

Your Lynx shell now includes a comprehensive customization system! Here's everything you can customize:

## 📁 Configuration Files

The shell automatically creates a configuration directory at `~/.lynx/` with these files:

```
~/.lynx/
├── config          # Main configuration file
├── aliases         # Command aliases
├── functions       # Custom shell functions
└── themes/         # Color themes
    ├── default.theme
    ├── dark.theme
    ├── minimal.theme
    └── rainbow.theme
```

## ⚙️ Configuration Options

### Basic Settings

```bash
# Set configuration values at runtime
set theme dark
set history_size 2000
set welcome_message "Welcome to my custom shell!"
set prompt_format "{user}@{host}:{cwd} ➜ "
```

### Available Settings:

- `theme` - Color theme (default, dark, minimal, rainbow)
- `prompt_format` - Prompt layout with variables: {user}, {host}, {cwd}
- `history_size` - Number of commands to remember (default: 1000)
- `auto_cd` - Change directory without typing 'cd' (true/false)
- `case_sensitive` - Case-sensitive command matching (true/false)
- `tab_completion` - Enable tab completion (true/false)
- `color_output` - Colorize output (true/false)
- `welcome_message` - Message shown at startup
- `exit_on_eof` - Exit on Ctrl+D (true/false)
- `command_timeout` - Timeout for commands in seconds

## 🎭 Themes

### Built-in Themes:

1. **default** - Classic look with colored prompts
2. **dark** - Modern dark theme with bright colors and two-line prompt
3. **minimal** - Simple, distraction-free single symbol prompt
4. **rainbow** - Colorful theme with emoji and vibrant colors

### Using Themes:

```bash
# List available themes
theme

# Switch themes
theme dark
theme minimal
theme rainbow

# Theme persists between sessions
```

### Custom Themes:

Create new themes in `~/.lynx/themes/mytheme.theme`:

```ini
# My Custom Theme
prompt_user_color="bright_green"
prompt_host_color="bright_blue"
prompt_cwd_color="bright_cyan"
prompt_format="🚀 {user}@{host} in {cwd} $ "
error_color="bright_red"
success_color="bright_green"
```

## 🔗 Aliases

### Built-in Aliases:

- `ll` → `ls -la` (detailed list)
- `la` → `ls -A` (show hidden files)
- `l` → `ls -CF` (classified list)
- `..` → `cd ..` (go up one directory)
- `...` → `cd ../..` (go up two directories)
- `cls` → `clear` (clear screen)

### Managing Aliases:

```bash
# List all aliases
alias

# Show specific alias
alias ll

# Create new alias
alias myls ls -la --color=auto

# Aliases are automatically saved
```

## 🎯 Advanced Features

### Runtime Configuration:

```bash
# Change settings without restarting
set prompt_format "λ {cwd} → "
set theme dark
alias gc git commit -m
```

### Configuration Persistence:

- All changes are automatically saved
- Settings persist between shell sessions
- Configuration files are human-readable and editable

### Sample Configurations:

**Power User Setup:**

```bash
set theme dark
set prompt_format "┌─[{user}@{host}]─[{cwd}]\n└─λ "
set history_size 5000
alias gst git status
alias gco git checkout
alias ll ls -la --color=auto
```

**Minimal Setup:**

```bash
set theme minimal
set welcome_message ""
set prompt_format "$ "
```

**Colorful Setup:**

```bash
set theme rainbow
set prompt_format "🦄 {user}@{host} in 📁{cwd} ✨ "
```

## 📋 Quick Reference

### Configuration Commands:

- `set [key] [value]` - Set configuration option
- `set [key]` - Show current value
- `set` - List all settings

### Theme Commands:

- `theme` - List available themes
- `theme [name]` - Switch to theme

### Alias Commands:

- `alias` - List all aliases
- `alias [name]` - Show specific alias
- `alias [name] [command]` - Create alias

### Other Commands:

- `help` - Show all commands
- `history` - Show command history
- `clear` - Clear screen
- `pwd` - Show current directory
- `cd [dir]` - Change directory
- `env` - Show environment variables

## 🔧 Manual Configuration

You can also manually edit configuration files:

### Main Config (`~/.lynx/config`):

```ini
theme=dark
prompt_format={user}@{host}:{cwd}$
history_size=1000
welcome_message=Welcome to Lynx Shell!
color_output=true
```

### Aliases (`~/.lynx/aliases`):

```ini
ll="ls -la"
gs="git status"
build="make debug"
```

Your shell is now fully customizable! Experiment with different themes, create useful aliases, and make it truly yours! 🎉
