# Theme Development Guide

Create beautiful, customizable themes for Lynx Shell with this comprehensive guide.

## 🎨 Quick Start

### 1. Create Your First Theme

```bash
# Navigate to Lynx source directory
cd /path/to/Lynx

# Create a new theme from template
./create_theme.sh my_awesome_theme

# Install the theme
cp examples/themes/my_awesome_theme.theme ~/.lynx/themes/

# Apply the theme
lynx
> set_theme my_awesome_theme
```

### 2. Test Your Theme

```bash
# Start Lynx with your theme
lynx
> set_theme my_awesome_theme
> help  # See commands with your colors
> ls    # See file listings with your colors
```

## 🎭 Theme File Formats

Lynx supports multiple theme file formats for flexibility:

### 1. INI Format (.theme)

```ini
# my_theme.theme
[metadata]
name = "My Awesome Theme"
version = "1.0.0"
author = "Your Name"
description = "A beautiful theme for Lynx Shell"

[colors]
# Basic colors
primary = "#00ff88"
secondary = "#ff6b00"
background = "#1a1a1a"
foreground = "#ffffff"
accent = "#ff4757"

# Text colors
text_normal = "#ffffff"
text_bright = "#f1f2f6"
text_dim = "#747d8c"
text_error = "#ff4757"
text_warning = "#ffa502"
text_success = "#2ed573"
text_info = "#3742fa"

# Interface elements
prompt = "#00ff88"
prompt_user = "#70a5fd"
prompt_host = "#5352ed"
prompt_path = "#ff6348"
prompt_symbol = "#ffffff"

# Command elements
command = "#ffffff"
command_builtin = "#70a5fd"
command_external = "#7bed9f"
command_alias = "#ffb8b8"
command_error = "#ff4757"

# File type colors
file_regular = "#ffffff"
file_directory = "#70a5fd"
file_executable = "#2ed573"
file_symlink = "#ff6b6b"
file_archive = "#ffa502"
file_image = "#ff6348"
file_video = "#a55eea"
file_audio = "#26de81"
file_document = "#fed330"

[styles]
# Text styles (bold, italic, underline, strikethrough)
prompt_style = "bold"
error_style = "bold"
warning_style = "italic"
success_style = "bold"
directory_style = "bold"
executable_style = "bold,underline"

[advanced]
# Advanced theming options
use_true_color = true
use_background_color = false
transparency = 0.9
cursor_style = "block"
cursor_blink = true
```

### 2. JSON Format (.json)

```json
{
  "metadata": {
    "name": "Cyberpunk Theme",
    "version": "2.0.0",
    "author": "Future Coder",
    "description": "A futuristic cyberpunk theme",
    "license": "MIT"
  },
  "colors": {
    "primary": "#00ff41",
    "secondary": "#ff0080",
    "background": "#0d0208",
    "foreground": "#00ff41",
    "accent": "#ff0080",
    "text": {
      "normal": "#00ff41",
      "bright": "#39ff14",
      "dim": "#008f11",
      "error": "#ff0040",
      "warning": "#ffff00",
      "success": "#00ff80",
      "info": "#00ffff"
    },
    "prompt": {
      "user": "#ff0080",
      "host": "#00ffff",
      "path": "#ffff00",
      "symbol": "#00ff41"
    },
    "files": {
      "regular": "#00ff41",
      "directory": "#00ffff",
      "executable": "#ff0080",
      "symlink": "#ffff00",
      "archive": "#ff8000",
      "image": "#ff0040",
      "video": "#8000ff",
      "audio": "#00ff80",
      "document": "#ffff80"
    }
  },
  "styles": {
    "prompt": ["bold"],
    "error": ["bold", "italic"],
    "warning": ["italic"],
    "success": ["bold"],
    "directory": ["bold"],
    "executable": ["bold", "underline"]
  },
  "advanced": {
    "use_true_color": true,
    "gradient_prompts": true,
    "animation_speed": 150,
    "cursor": {
      "style": "block",
      "blink": true,
      "color": "#00ff41"
    }
  }
}
```

### 3. YAML Format (.yaml/.yml)

```yaml
# futuristic.yml
metadata:
  name: "Futuristic Theme"
  version: "1.5.0"
  author: "Space Developer"
  description: "A space-age theme for modern terminals"

colors:
  primary: "#64ffda"
  secondary: "#ff4081"
  background: "#263238"
  foreground: "#eceff1"

  text:
    normal: "#eceff1"
    bright: "#ffffff"
    dim: "#90a4ae"
    error: "#f44336"
    warning: "#ff9800"
    success: "#4caf50"
    info: "#2196f3"

  prompt:
    user: "#64ffda"
    host: "#e91e63"
    path: "#ffeb3b"
    symbol: "#ffffff"

  files:
    regular: "#eceff1"
    directory: "#64ffda"
    executable: "#4caf50"
    symlink: "#ff4081"

styles:
  prompt: [bold]
  error: [bold, italic]
  directory: [bold]

advanced:
  use_true_color: true
  gradient_effects: true
  transparency: 0.95
```

## 🎨 Color Reference

### Color Formats

Lynx supports multiple color formats:

```ini
# Hex colors (preferred)
color = "#ff6b6b"     # RGB hex
color = "#ff6b6baa"   # RGBA hex with alpha

# RGB values
color = "rgb(255, 107, 107)"
color = "rgba(255, 107, 107, 0.8)"

# HSL values
color = "hsl(0, 100%, 71%)"
color = "hsla(0, 100%, 71%, 0.8)"

# Named colors
color = "red"         # Basic color names
color = "bright_red"  # Extended color names

# ANSI color codes
color = "196"         # 256-color palette
color = "38;2;255;107;107"  # True color escape
```

### Theme Color Categories

#### Essential Colors

```ini
[colors]
# Core theme colors
primary = "#main_theme_color"      # Primary brand color
secondary = "#accent_color"        # Secondary accent color
background = "#bg_color"           # Main background
foreground = "#text_color"         # Main text color
accent = "#highlight_color"        # Accent/highlight color
```

#### Text Colors

```ini
# Text variations
text_normal = "#ffffff"     # Normal text
text_bright = "#f8f9fa"     # Bright/highlighted text
text_dim = "#6c757d"        # Dimmed/secondary text
text_error = "#dc3545"      # Error messages
text_warning = "#ffc107"    # Warning messages
text_success = "#28a745"    # Success messages
text_info = "#17a2b8"       # Info messages
```

#### Prompt Colors

```ini
# Shell prompt elements
prompt = "#main_prompt_color"       # General prompt color
prompt_user = "#user_color"         # Username
prompt_host = "#hostname_color"     # Hostname
prompt_path = "#path_color"         # Current path
prompt_symbol = "#symbol_color"     # Prompt symbol ($ > #)
prompt_time = "#time_color"         # Timestamp
prompt_git = "#git_color"           # Git status
```

#### Command Colors

```ini
# Command syntax highlighting
command = "#command_color"          # General commands
command_builtin = "#builtin_color"  # Built-in commands
command_external = "#external_color" # External programs
command_alias = "#alias_color"      # Aliases
command_keyword = "#keyword_color"  # Keywords (if, for, etc.)
command_operator = "#operator_color" # Operators (|, &, etc.)
command_argument = "#arg_color"     # Command arguments
command_flag = "#flag_color"        # Command flags (-v, --help)
command_error = "#error_cmd_color"  # Invalid commands
```

#### File Type Colors

```ini
# File and directory colors
file_regular = "#regular_file_color"     # Regular files
file_directory = "#directory_color"      # Directories
file_executable = "#executable_color"    # Executable files
file_symlink = "#symlink_color"          # Symbolic links
file_broken_symlink = "#broken_color"    # Broken symlinks
file_socket = "#socket_color"            # Socket files
file_pipe = "#pipe_color"                # Named pipes
file_block_device = "#block_color"       # Block devices
file_char_device = "#char_color"         # Character devices

# File extensions
file_archive = "#archive_color"          # .zip, .tar, .gz
file_image = "#image_color"              # .jpg, .png, .gif
file_video = "#video_color"              # .mp4, .avi, .mov
file_audio = "#audio_color"              # .mp3, .wav, .flac
file_document = "#document_color"        # .pdf, .doc, .txt
file_code = "#code_color"                # .cpp, .py, .js
file_config = "#config_color"            # .conf, .ini, .json
file_log = "#log_color"                  # .log files
```

#### Interface Colors

```ini
# UI elements
border = "#border_color"            # Window borders
header = "#header_color"            # Headers/titles
footer = "#footer_color"            # Footers/status
menu = "#menu_color"                # Menu items
menu_selected = "#menu_sel_color"   # Selected menu items
progress_bar = "#progress_color"    # Progress indicators
completion = "#completion_color"    # Tab completion
search_match = "#search_color"      # Search highlights
```

## 🎯 Style Options

### Text Styles

```ini
[styles]
# Available styles
style_name = "bold"                 # Bold text
style_name = "italic"               # Italic text
style_name = "underline"            # Underlined text
style_name = "strikethrough"        # Strikethrough text
style_name = "reverse"              # Reverse video
style_name = "blink"                # Blinking text (use sparingly)

# Combined styles
style_name = "bold,italic"          # Multiple styles
style_name = "bold,underline,italic" # Multiple styles
```

### Advanced Styling

```ini
[advanced]
# True color support
use_true_color = true               # Enable 24-bit color
use_256_color = false               # Fallback to 256 colors

# Background effects
use_background_color = true         # Apply background colors
transparency = 0.9                  # Background transparency (0.0-1.0)

# Cursor styling
cursor_style = "block"              # block, underline, bar
cursor_blink = true                 # Enable cursor blinking
cursor_color = "#ffffff"            # Cursor color

# Text effects
text_shadow = true                  # Enable text shadows
gradient_text = true                # Enable gradient text effects
animation_speed = 200               # Animation speed (ms)
```

## 📝 Complete Theme Examples

### 1. Minimalist Theme

```ini
# minimal.theme
[metadata]
name = "Minimal"
version = "1.0.0"
author = "Clean Coder"
description = "A clean, minimalist theme"

[colors]
primary = "#333333"
secondary = "#666666"
background = "#ffffff"
foreground = "#333333"
accent = "#0066cc"

text_normal = "#333333"
text_bright = "#000000"
text_dim = "#666666"
text_error = "#cc0000"
text_warning = "#cc6600"
text_success = "#006600"
text_info = "#0066cc"

prompt = "#333333"
prompt_user = "#666666"
prompt_host = "#666666"
prompt_path = "#0066cc"
prompt_symbol = "#333333"

command = "#333333"
command_builtin = "#0066cc"
command_external = "#006600"
command_alias = "#cc6600"

file_regular = "#333333"
file_directory = "#0066cc"
file_executable = "#006600"
file_symlink = "#cc6600"

[styles]
prompt_style = "bold"
directory_style = "bold"
executable_style = "bold"

[advanced]
use_true_color = true
use_background_color = false
cursor_style = "bar"
cursor_blink = false
```

### 2. Dark Neon Theme

```json
{
  "metadata": {
    "name": "Dark Neon",
    "version": "1.0.0",
    "author": "Neon Designer",
    "description": "A dark theme with neon accents"
  },
  "colors": {
    "primary": "#ff0066",
    "secondary": "#00ffff",
    "background": "#1a0033",
    "foreground": "#ffffff",
    "accent": "#ff3366",
    "text": {
      "normal": "#ffffff",
      "bright": "#ff99cc",
      "dim": "#6699cc",
      "error": "#ff3366",
      "warning": "#ffcc00",
      "success": "#00ff99",
      "info": "#0099ff"
    },
    "prompt": {
      "user": "#ff0066",
      "host": "#00ffff",
      "path": "#ff99cc",
      "symbol": "#ffffff",
      "git": "#00ff99"
    },
    "commands": {
      "default": "#ffffff",
      "builtin": "#00ffff",
      "external": "#ff99cc",
      "alias": "#ffcc00",
      "keyword": "#ff0066"
    },
    "files": {
      "regular": "#ffffff",
      "directory": "#00ffff",
      "executable": "#00ff99",
      "symlink": "#ff99cc",
      "archive": "#ffcc00",
      "image": "#ff6699",
      "video": "#9966ff",
      "audio": "#66ff99",
      "document": "#ffff66",
      "code": "#ff9999"
    }
  },
  "styles": {
    "prompt": ["bold"],
    "error": ["bold"],
    "warning": ["italic"],
    "success": ["bold"],
    "directory": ["bold"],
    "executable": ["bold", "underline"],
    "keyword": ["bold", "italic"]
  },
  "advanced": {
    "use_true_color": true,
    "gradient_effects": true,
    "glow_effects": true,
    "cursor": {
      "style": "block",
      "blink": true,
      "color": "#ff0066"
    }
  }
}
```

### 3. Retro Terminal Theme

```yaml
# retro.yml
metadata:
  name: "Retro Terminal"
  version: "1.0.0"
  author: "Vintage Coder"
  description: "Classic green-on-black terminal theme"

colors:
  primary: "#00ff00"
  secondary: "#ffff00"
  background: "#000000"
  foreground: "#00ff00"
  accent: "#00ffff"

  text:
    normal: "#00ff00"
    bright: "#00ff80"
    dim: "#008000"
    error: "#ff0000"
    warning: "#ffff00"
    success: "#00ff80"
    info: "#00ffff"

  prompt:
    user: "#00ff00"
    host: "#00ff00"
    path: "#ffff00"
    symbol: "#00ff00"

  commands:
    default: "#00ff00"
    builtin: "#00ffff"
    external: "#00ff80"
    alias: "#ffff00"

  files:
    regular: "#00ff00"
    directory: "#00ffff"
    executable: "#ffff00"
    symlink: "#ff8000"

styles:
  prompt: [bold]
  directory: [bold]
  executable: [bold]

advanced:
  use_true_color: false
  scanlines: true
  flicker_effect: true
  cursor:
    style: "block"
    blink: true
    color: "#00ff00"
```

## 🔧 Theme Configuration

### Theme Metadata

```ini
[metadata]
name = "Theme Name"              # Display name
version = "1.0.0"                # Version number
author = "Your Name"             # Author name
email = "your@email.com"         # Contact email
description = "Theme description" # Brief description
license = "MIT"                  # License type
homepage = "https://github..."   # Project homepage
keywords = "dark,modern,blue"    # Search keywords
category = "dark"                # Theme category
compatibility = ">=1.0.0"       # Lynx version compatibility
```

### Advanced Options

```ini
[advanced]
# Color support
use_true_color = true            # 24-bit color support
use_256_color = true             # 256-color fallback
use_16_color = false             # 16-color fallback

# Visual effects
gradient_prompts = true          # Gradient prompt colors
glow_effects = true              # Text glow effects
shadow_effects = false           # Text shadow effects
transparency = 0.9               # Background transparency
blur_radius = 5                  # Background blur

# Animation
enable_animations = true         # Enable animations
animation_speed = 200            # Animation duration (ms)
transition_speed = 150           # Color transition speed

# Cursor
cursor_style = "block"           # block, underline, bar
cursor_blink = true              # Cursor blinking
cursor_blink_speed = 500         # Blink interval (ms)
cursor_color = "#ffffff"         # Cursor color

# Font effects
font_weight = "normal"           # normal, bold, light
font_style = "normal"            # normal, italic
letter_spacing = 0               # Letter spacing adjustment
line_height = 1.0                # Line height multiplier

# Terminal effects
scanlines = false                # CRT scanline effect
noise = false                    # Static noise effect
flicker = false                  # Screen flicker effect
```

## 🎨 Color Schemes and Palettes

### Popular Color Palettes

#### Solarized Dark

```ini
background = "#002b36"
foreground = "#839496"
primary = "#268bd2"
secondary = "#2aa198"
accent = "#b58900"
error = "#dc322f"
success = "#859900"
warning = "#cb4b16"
```

#### Monokai

```ini
background = "#272822"
foreground = "#f8f8f2"
primary = "#f92672"
secondary = "#a6e22e"
accent = "#fd971f"
error = "#f92672"
success = "#a6e22e"
warning = "#e6db74"
```

#### Dracula

```ini
background = "#282a36"
foreground = "#f8f8f2"
primary = "#bd93f9"
secondary = "#8be9fd"
accent = "#ff79c6"
error = "#ff5555"
success = "#50fa7b"
warning = "#f1fa8c"
```

#### Nord

```ini
background = "#2e3440"
foreground = "#d8dee9"
primary = "#5e81ac"
secondary = "#88c0d0"
accent = "#81a1c1"
error = "#bf616a"
success = "#a3be8c"
warning = "#ebcb8b"
```

### Creating Color Harmonies

```ini
# Monochromatic (variations of one color)
base = "#3498db"
light = "#85c1e9"
dark = "#1f618d"

# Complementary (opposite colors)
primary = "#e74c3c"    # Red
complement = "#3ce74c" # Green

# Triadic (three evenly spaced colors)
color1 = "#e74c3c"     # Red
color2 = "#4ce73c"     # Green
color3 = "#3c4ce7"     # Blue

# Analogous (adjacent colors)
color1 = "#e74c3c"     # Red
color2 = "#e7993c"     # Orange
color3 = "#e7e73c"     # Yellow
```

## 🔧 Building and Testing Themes

### Theme Validation

Create a theme validator script:

```bash
#!/bin/bash
# validate_theme.sh

THEME_FILE="$1"

if [ ! -f "$THEME_FILE" ]; then
    echo "❌ Theme file not found: $THEME_FILE"
    exit 1
fi

echo "🎨 Validating theme: $THEME_FILE"

# Check file format
case "$THEME_FILE" in
    *.theme|*.ini)
        echo "✅ Format: INI"
        # Validate INI format
        ;;
    *.json)
        echo "✅ Format: JSON"
        # Validate JSON format
        if ! jq . "$THEME_FILE" > /dev/null 2>&1; then
            echo "❌ Invalid JSON format"
            exit 1
        fi
        ;;
    *.yaml|*.yml)
        echo "✅ Format: YAML"
        # Validate YAML format (requires yq or python)
        ;;
    *)
        echo "❌ Unsupported format"
        exit 1
        ;;
esac

# Check required sections
echo "✅ Theme validation passed"
```

### Theme Testing

```bash
#!/bin/bash
# test_theme.sh

THEME_NAME="$1"

echo "🧪 Testing theme: $THEME_NAME"

# Copy theme to user directory
cp "themes/${THEME_NAME}.theme" ~/.lynx/themes/

# Start Lynx with theme
echo "set_theme $THEME_NAME" | lynx --test-mode

# Test various commands
echo "help" | lynx --theme="$THEME_NAME"
echo "ls -la" | lynx --theme="$THEME_NAME"
echo "echo 'Test message'" | lynx --theme="$THEME_NAME"

echo "✅ Theme testing completed"
```

### Live Theme Editing

For rapid theme development:

```bash
#!/bin/bash
# live_edit_theme.sh

THEME_FILE="$1"
THEME_NAME=$(basename "$THEME_FILE" .theme)

# Watch for file changes and reload theme
while inotifywait -e modify "$THEME_FILE"; do
    echo "🔄 Reloading theme: $THEME_NAME"
    cp "$THEME_FILE" ~/.lynx/themes/
    echo "reload_theme" | nc localhost 9999  # Send to Lynx if running
done
```

## 🎯 Best Practices

### 1. Accessibility

```ini
# Ensure sufficient contrast
background = "#000000"
foreground = "#ffffff"  # High contrast

# Use colorblind-friendly colors
error = "#d73027"       # Red that works for colorblind users
success = "#1a9850"     # Green that works for colorblind users
warning = "#fe9929"     # Orange instead of yellow
info = "#3690c0"        # Blue

# Provide alternative styling
[styles]
error_style = "bold,underline"     # Don't rely only on color
warning_style = "italic"           # Use text styles too
success_style = "bold"
```

### 2. Performance

```ini
# Minimize expensive effects in themes used frequently
[advanced]
gradient_effects = false    # Can slow down rendering
glow_effects = false       # GPU-intensive effect
animations = false         # Disable for better performance

# Use efficient color formats
color = "#ff6b6b"          # Hex is fastest
# Avoid: color = "rgba(255, 107, 107, 0.8)"  # Slower parsing
```

### 3. Consistency

```ini
# Use consistent color relationships
primary = "#007acc"
primary_light = "#40a9ff"     # Lighter version of primary
primary_dark = "#003a75"      # Darker version of primary

# Maintain visual hierarchy
text_normal = "#ffffff"       # Most readable
text_bright = "#f0f0f0"       # Slightly less prominent
text_dim = "#999999"          # Least prominent
```

### 4. Documentation

Always include comprehensive documentation:

```ini
# theme_name.theme
#
# Theme Name: My Awesome Theme
# Author: Your Name
# Version: 1.0.0
# Description: A beautiful theme for Lynx Shell
#
# Features:
# - High contrast for readability
# - Colorblind-friendly palette
# - Optimized for dark terminals
#
# Installation:
# 1. Copy to ~/.lynx/themes/
# 2. Run: lynx
# 3. Execute: set_theme my_awesome_theme
#
```

## 🚀 Advanced Theme Features

### Dynamic Themes

Create themes that change based on context:

```json
{
  "metadata": {
    "name": "Dynamic Theme",
    "type": "dynamic"
  },
  "conditions": {
    "time_of_day": {
      "morning": {
        "colors": {
          "background": "#f8f9fa",
          "foreground": "#212529"
        }
      },
      "evening": {
        "colors": {
          "background": "#212529",
          "foreground": "#f8f9fa"
        }
      }
    },
    "git_status": {
      "clean": {
        "colors": {
          "prompt_git": "#28a745"
        }
      },
      "dirty": {
        "colors": {
          "prompt_git": "#dc3545"
        }
      }
    }
  }
}
```

### Theme Inheritance

```json
{
  "metadata": {
    "name": "My Custom Theme",
    "extends": "default"
  },
  "colors": {
    "primary": "#custom_color",
    "secondary": "#another_custom_color"
    // Other colors inherited from default theme
  }
}
```

### Responsive Themes

```json
{
  "metadata": {
    "name": "Responsive Theme"
  },
  "responsive": {
    "terminal_width": {
      "small": {
        "max_width": 80,
        "colors": {
          "prompt": "#short_prompt_color"
        },
        "prompt_format": "minimal"
      },
      "large": {
        "min_width": 120,
        "colors": {
          "prompt": "#full_prompt_color"
        },
        "prompt_format": "detailed"
      }
    }
  }
}
```

## 📚 Theme Distribution

### Publishing Your Theme

1. **Create a Repository**

```bash
mkdir lynx-theme-awesome
cd lynx-theme-awesome
git init
```

2. **Add Theme Files**

```
lynx-theme-awesome/
├── README.md
├── LICENSE
├── awesome.theme          # Main theme file
├── awesome-light.theme    # Light variant
├── awesome-dark.theme     # Dark variant
├── screenshots/           # Theme screenshots
├── install.sh            # Installation script
└── examples/             # Usage examples
```

3. **Create Installation Script**

```bash
#!/bin/bash
# install.sh

THEME_DIR="$HOME/.lynx/themes"
mkdir -p "$THEME_DIR"

echo "Installing Awesome Theme..."
cp *.theme "$THEME_DIR/"

echo "✅ Theme installed!"
echo "Apply with: lynx"
echo "Then run: set_theme awesome"
```

### Theme Gallery

Create preview images:

```bash
#!/bin/bash
# generate_previews.sh

THEME_NAME="awesome"

# Generate screenshots for different commands
echo "help" | lynx --theme="$THEME_NAME" --screenshot="preview_help.png"
echo "ls -la /usr/bin | head -20" | lynx --theme="$THEME_NAME" --screenshot="preview_ls.png"
echo "git status" | lynx --theme="$THEME_NAME" --screenshot="preview_git.png"

echo "✅ Preview images generated"
```

## 🎉 Conclusion

You now have everything you need to create stunning themes for Lynx Shell! Remember:

- Start with existing themes as inspiration
- Test your themes thoroughly
- Consider accessibility and performance
- Document your themes well
- Share your creations with the community

Happy theming! 🎨✨
