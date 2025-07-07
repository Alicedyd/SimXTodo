# SimXTodo

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-blue)]()
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey)]()

> A lightweight, terminal-based todo list manager for developers who prefer simplicity and efficiency.

## ✨ Features

- **🎯 Simple & Focused**: Clean interface without unnecessary bloat
- **⌨️ Keyboard-Driven**: Vim-like navigation for maximum productivity
- **🌈 Color-Coded Status**: Visual status indicators with three distinct states
- **💾 Persistent Storage**: Automatic save/load functionality
- **🌍 Unicode Support**: Full UTF-8 support including Chinese characters
- **📱 Responsive Layout**: Adapts to different terminal sizes
- **⚡ Fast Performance**: Written in C for optimal speed

## 🚀 Quick Start

### Prerequisites

- **Linux/macOS**: Unix-like operating system
- **CMake**: Version 3.10 or higher
- **ncurses**: Development libraries
- **GCC/Clang**: C compiler with C99 support

### Installation

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
./SimXTodo
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

Your todos are automatically saved to the following path according to your system in a binary format. The file is:

Windows

```
%APPDATA%/SimXTodo/todo
```

MacOS

```
~/Library/Application Support/SimXTodo/todo
```

Linux

```
~/.config/simxtodo/todo
```

- Created automatically on first use
- Saved automatically when you quit the application
- Loaded automatically when you start the application

## 🏗️ Project Structure

```
SimXTodo/
├── src/
│   ├── todo_list.c      # Core todo list operations
│   ├── utils.c          # Utility functions (time, UTF-8)
│   └── window_control.c # ncurses UI management
├── include/
│   ├── todo_list.h      # Todo list data structures
│   ├── utils.h          # Utility function declarations
│   └── window_control.h # UI function declarations
├── main.c               # Main application loop
├── CMakeLists.txt       # Build configuration
└── README.md           # This file
```

## 📋 Roadmap

- [ ] **Search functionality** - Find todos by content
- [ ] **Categories/Tags** - Organize todos with labels
- [ ] **Due dates** - Add deadline tracking
- [ ] **Export options** - Export to text/JSON formats
- [ ] **Theme customization** - Custom color schemes
- [ ] **Multiple lists** - Support for separate todo lists

## 🐛 Known Issues

- Long todo items may wrap oddly in narrow terminals
- Some terminal emulators may have issues with UTF-8 input

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Alicedyd** - [GitHub Profile](https://github.com/Alicedyd)

## 🙏 Acknowledgments

- Built with [ncurses](https://invisible-island.net/ncurses/) library
- Inspired by vim's keyboard-driven philosophy
- Thanks to the open source community for continuous feedback

---

**Happy Todo Managing! 📝**
