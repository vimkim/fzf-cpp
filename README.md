# C++ Fuzzy Finder (cpp-fzf)

A lightweight fuzzy finder implementation in C++ using ncurses, inspired by [fzf](https://github.com/junegunn/fzf). This tool provides an interactive interface for fuzzy searching through text items with real-time filtering and navigation capabilities.

![image](https://github.com/user-attachments/assets/6ebbcb0a-dba5-4496-9b6a-75fd12f5bce1)

## Why yet another fuzzy finder?

While excellent fuzzy finders like [fzf](https://github.com/junegunn/fzf) exist, there are specific scenarios where a native C++ implementation provides distinct advantages:

### Simplified Integration

- **No External Dependencies**: Eliminate the need to bundle external binaries or manage subprocess calls
- **Native C++ Solution**: Avoid cross-language complications and additional build dependencies
- **Header-Only Library**: Easy to include in existing C++ projects with minimal setup

### Use Cases

#### Perfect For

- Embedded systems where installing additional runtime dependencies isn't feasible
- Applications requiring tight integration with existing C++ codebases
- Projects where process spawning is restricted or undesirable
- Lightweight applications where full fzf functionality isn't necessary
- Development environments with strict dependency management policies

#### Consider Alternatives When

- You need advanced features available in full-featured fuzzy finders
- Your application can easily manage external process dependencies
- Performance requirements exceed what a simple implementation can provide

### Design Philosophy

This project prioritizes:

- Simplicity of integration over feature completeness
- Minimal dependencies over maximum functionality
- Native C++ compatibility over cross-language support

If you need more advanced fuzzy finding capabilities, consider using fzf as a subprocess through popen or similar mechanisms. This library aims to fill the gap for simpler use cases where native C++ integration is more valuable than extensive features.

## Getting Started

### Basic Integration

```cpp
#include "fzf.hpp"

// Example usage
std::vector<std::string> candidates = {"file1.txt", "file2.txt", "other.doc"};
FuzzyFinder finder;
auto results = finder.find("fil", candidates);
```

## Features

- Interactive fuzzy search interface
- Real-time filtering as you type
- Case-insensitive matching
- Multi-line text support with smart truncation
- Terminal UI using ncurses

## Prerequisites

- C++ compiler (tested with g++11 on Ubuntu 22.04)
- ncurses library
- CMake

## Building

```bash
# Using CMake
cmake -S . -B build
cmake --build build
# then use build/example binary

# Or, you can use just
just configure-build
# this is equivalent to the above two cmake commands
```

## Usage

### Basic Usage

```cpp
std::vector<std::string> items = {"item1", "item2", "item3"};
std::string selected = fzf(items);
```

### Interactive Controls

- **Type** to filter items
- **↑/↓** to navigate through items
- **Enter** to select an item
- **Esc** to cancel selection
- **Backspace** to modify search query

### Interface Features

- The search query is displayed at the top
- Matched items are shown below the search bar
- Selected item is highlighted
- Long items are truncated with "..." if they exceed the terminal width
- Multi-line items are flattened to single lines for display

### Example Integration

```cpp
#include <vector>
#include <string>

int main() {
    std::vector<std::string> items = {
        "SELECT * FROM users",
        "INSERT INTO orders (id, name) VALUES (1, 'test')",
        "UPDATE users SET status = 'active'"
    };

    std::string selected = fzf(items);
    if (!selected.empty()) {
        // Process the selected item
        std::cout << "Selected: " << selected << std::endl;
    }
    return 0;
}
```

## Key Components

- `fuzzyMatch`: Implements the fuzzy matching algorithm
- `createDisplayString`: Handles string formatting and truncation
- `displayItems`: Manages the ncurses display
- `fzf`: Main function that provides the interactive interface

## Technical Notes

- Terminal size is automatically detected and respected
- Multi-line strings are flattened with spaces replacing newlines
- The interface adjusts to terminal resizing
- Memory efficient with minimal string copying
- Clean terminal restoration after exit

## Error Handling

The implementation includes basic error handling:

- Returns empty string on ESC or if no selection is made
- Automatically adjusts selection index when filtered results change
- Handles terminal resize events gracefully

## Contributing

Feel free to submit issues and enhancement requests. This implementation can be extended with features like:

- Custom key bindings
- Multiple selection support
- Preview window
- Custom scoring algorithms
- Color customization

## License

This project is open-source and available under the Apache 2.0 License.
