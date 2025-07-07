# SimXTodo

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-blue)]()
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()

> A lightweight, cross-platform, terminal-based todo list manager for developers who prefer simplicity and efficiency.

## ✨ Features

- **🎯 Simple & Focused**: Clean interface without unnecessary bloat
- **⌨️ Keyboard-Driven**: Vim-like navigation for maximum productivity
- **🌈 Color-Coded Status**: Visual status indicators with three distinct states
- **💾 Persistent Storage**: Automatic save/load functionality with platform-specific paths
- **🌍 Unicode Support**: Full UTF-8 support including Chinese characters
- **📱 Responsive Layout**: Adapts to different terminal sizes
- **⚡ Fast Performance**: Written in C for optimal speed
- **🖥️ Cross-Platform**: Native support for Windows, macOS, and Linux
- **📁 Smart File Management**: Automatic config directory and file creation

## 🚀 Quick Start

### Prerequisites

- **Windows**: Windows 7 or later
- **Linux/macOS**: Unix-like operating system
- **CMake**: Version 3.10 or higher
- **ncurses**: Development libraries (PDCurses for Windows)
- **GCC/Clang/MSVC**: C compiler with C99 support

### Installation

#### Windows

```bash
# Using MinGW-w64 (recommended)
# Install dependencies first:
# - CMake: https://cmake.org/download/
# - MinGW-w64: https://www.mingw-w64.org/
# - PDCurses: https://pdcurses.org/

# Clone and build
git clone https://github.com/Alicedyd/SimXTodo.git
cd SimXTodo
mkdir build && cd build
cmake -G "MinGW Makefiles" .. && make
```

#### Ubuntu/Debian

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake libncurses5-dev libncursesw5-dev

# Clone and build
git clone https://github.com/Alicedyd/SimXTodo.git
cd SimXTodo
mkdir build && cd build
cmake .. && make
```

#### macOS

```bash
# Install dependencies (if needed)
brew install cmake ncurses

# Clone and build
git clone https://github.com/Alicedyd/SimXTodo.git
cd SimXTodo
mkdir build && cd build
cmake .. && make
```

#### Arch Linux

```bash
# Install dependencies
sudo pacman -S cmake ncurses

# Clone and build
git clone https://github.com/Alicedyd/SimXTodo.git
cd SimXTodo
mkdir build && cd build
cmake .. && make
```

### Running

```bash
# From the build directory
./SimXTodo          # Linux/macOS
SimXTodo.exe        # Windows
```

## 📖 Usage

### Status Types

SimXTodo uses a simple three-state system with color coding:

| Status | Color | Description |
|--------|-------|-------------|
| **TODO** | 🔴 Red | Tasks that haven't been started |
| **DOING** | 🟡 Yellow | Tasks currently in progress |
| **DONE** | 🟢 Green | Completed tasks |

### Keyboard Shortcuts

#### Navigation

- `j` - Move cursor down
- `k` - Move cursor up

#### Item Management

- `a` - **Add** new todo item
- `d` - **Delete** selected item (with confirmation)
- `c` - **Change** status (TODO → DOING → DONE → TODO)
- `m` - **Modify** item content

#### Organization

- `J` - Move item **down** in the list
- `K` - Move item **up** in the list

#### General

- `h` - Show **help** menu
- `q` - **Quit** application

### Data Storage

Your todos are automatically saved in a platform-appropriate location using a binary format. The configuration file is:

#### Windows

```
%APPDATA%\SimXTodo\todo
```

*Example: `C:\Users\YourName\AppData\Roaming\SimXTodo\todo`*

#### macOS

```
~/Library/Application Support/SimXTodo/todo
```

*Example: `/Users/YourName/Library/Application Support/SimXTodo/todo`*

#### Linux

```
~/.config/simxtodo/todo
```

*Example: `/home/YourName/.config/simxtodo/todo`*

**File Management Features:**

- Created automatically on first use with proper directory structure
- Saved automatically when you quit the application
- Loaded automatically when you start the application
- Cross-platform path detection ensures consistent experience
- Follows OS-specific configuration directory conventions

## 🏗️ Project Structure

```
SimXTodo/
├── src/
│   ├── todo_list.c      # Core todo list operations
│   ├── utils.c          # Utility functions (time, UTF-8, cross-platform paths)
│   └── window_control.c # ncurses UI management
├── include/
│   ├── todo_list.h      # Todo list data structures
│   ├── utils.h          # Utility function declarations
│   └── window_control.h # UI function declarations
├── main.c               # Main application loop
├── CMakeLists.txt       # Cross-platform build configuration
└── README.md           # This file
```

## 🛠️ Build System

The project uses CMake for cross-platform building with the following features:

- **Automatic library detection**: Finds ncurses/PDCurses based on platform
- **Compiler optimization**: Platform-specific compile flags
- **Windows integration**: Automatic shell32 linking for Windows APIs
- **macOS framework support**: CoreFoundation framework linking

### Build Options

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Specify custom ncurses path (if needed)
cmake -DCURSES_INCLUDE_DIR=/custom/path/include ..
```

## 📋 Roadmap

- [x] **Cross-platform support** - Windows, macOS, and Linux compatibility
- [x] **Smart file management** - Automatic config directory creation
- [ ] **Search functionality** - Find todos by content
- [ ] **Categories/Tags** - Organize todos with labels
- [ ] **Due dates** - Add deadline tracking
- [ ] **Export options** - Export to text/JSON formats
- [ ] **Theme customization** - Custom color schemes
- [ ] **Multiple lists** - Support for separate todo lists
- [ ] **Sync support** - Cloud synchronization options

## 🐛 Known Issues

- Long todo items may wrap oddly in narrow terminals
- Some terminal emulators may have issues with UTF-8 input
- Windows CMD may require UTF-8 codepage setting (`chcp 65001`)

## 🔧 Troubleshooting

### Windows Issues

- **"ncurses not found"**: Install PDCurses library
- **Character encoding issues**: Run `chcp 65001` in CMD before starting
- **Missing DLLs**: Ensure MinGW/MSVC runtime libraries are available

### Linux/macOS Issues

- **Permission denied**: Check if the config directory is writable
- **Segmentation fault**: Usually indicates corrupted config file - delete and restart

### General

- **Config file corruption**: Delete the config file to reset all data
- **Build failures**: Ensure all dependencies are properly installed

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Alicedyd** - [GitHub Profile](https://github.com/Alicedyd)

## 🙏 Acknowledgments

- Built with [ncurses](https://invisible-island.net/ncurses/) library (Linux/macOS)
- Windows support via [PDCurses](https://pdcurses.org/)
- Inspired by vim's keyboard-driven philosophy
- Thanks to the open source community for continuous feedback
- Cross-platform file system conventions for seamless user experience

---

**Happy Todo Managing Across All Platforms! 📝🖥️**
