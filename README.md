# C++ Fuzzy Finder (cpp-fzf)

A lightweight fuzzy finder implementation in C++ using ncurses, inspired by [fzf](https://github.com/junegunn/fzf). This tool provides an interactive interface for fuzzy searching through text items with real-time filtering and navigation capabilities.

![image](https://github.com/user-attachments/assets/6ebbcb0a-dba5-4496-9b6a-75fd12f5bce1)

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
# then use build/fzf_cpp binary

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
