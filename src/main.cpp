#include <ncurses.h>
#include <string>
#include <vector>
#include <cctype>

// A simple fuzzy matching function
// Checks if all characters in 'pattern' appear in 'str' in order.
bool fuzzyMatch(const std::string &str, const std::string &pattern) {
    if (pattern.empty()) return true; // Empty pattern matches everything
    
    size_t patternIndex = 0;
    for (char ch : str) {
        if (std::tolower(ch) == std::tolower(pattern[patternIndex])) {
            patternIndex++;
            if (patternIndex == pattern.size()) {
                return true;
            }
        }
    }
    return false;
}

// Displays the given items in ncurses, highlighting the currently selected index.
void displayItems(const std::vector<std::string> &items, int startIndex, int selectedIndex, int maxLines) {
    // Clear the region after the search input line
    // (We assume the input line is at row=0)
    for (int i = 1; i < maxLines; i++) {
        move(i, 0);
        clrtoeol();
    }

    // Print items line by line, starting from row=1
    int row = 1;
    for (int i = 0; i < (int)items.size(); i++) {
        // For large lists, you may want to limit how many are displayed
        // or implement scrolling. For simplicity, we display all here.
        move(row, 0);
        if (i == selectedIndex) {
            // Highlight the selected line
            attron(A_REVERSE);
            printw("%s", items[i].c_str());
            attroff(A_REVERSE);
        } else {
            printw("%s", items[i].c_str());
        }
        row++;
        if (row >= maxLines) break; // Avoid writing beyond screen
    }
}

// The main fuzzy-finder function
// Returns the selected string upon pressing Enter.
std::string fzf(const std::vector<std::string> &items) {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);  // Enable arrow keys

    // Get screen height & width
    int height, width;
    getmaxyx(stdscr, height, width);

    std::string query;
    int selectedIndex = 0;

    // We'll keep a separate list of currently filtered items
    std::vector<std::string> filteredItems = items;

    // Main loop
    while (true) {
        // Display the prompt & current query
        move(0, 0);
        clrtoeol();
        printw("Search: %s", query.c_str());

        // Filter items
        filteredItems.clear();
        for (const auto &item : items) {
            if (fuzzyMatch(item, query)) {
                filteredItems.push_back(item);
            }
        }

        // Ensure selectedIndex is in range
        if (!filteredItems.empty()) {
            if (selectedIndex >= (int)filteredItems.size()) {
                selectedIndex = filteredItems.size() - 1;
            }
        } else {
            selectedIndex = 0;
        }

        // Display filtered items starting at row=1
        displayItems(filteredItems, 0, selectedIndex, height);

        // Process user input
        int ch = getch();
        if (ch == KEY_UP) {
            if (selectedIndex > 0) {
                selectedIndex--;
            }
        } else if (ch == KEY_DOWN) {
            if (selectedIndex < (int)filteredItems.size() - 1) {
                selectedIndex++;
            }
        } else if (ch == 10 /* Enter key */) {
            // Return the selected item if there is any
            if (!filteredItems.empty()) {
                endwin(); 
                return filteredItems[selectedIndex];
            }
            // If filtered list is empty, do nothing
        } else if (ch == 27 /* ESC */) {
            // ESC pressed: cancel and return empty string
            endwin();
            return "";
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            // Handle backspace
            if (!query.empty()) {
                query.pop_back();
            }
        } else if (std::isprint(ch)) {
            // Append typed character to the query
            query.push_back(static_cast<char>(ch));
        }
    }
}

int main() {
    std::vector<std::string> myItems = {
        "apple", "banana", "grape", "orange", "blueberry",
        "mango", "strawberry", "watermelon", "kiwi", "apricot", "avocado"
    };

    std::string result = fzf(myItems);
    // If ESC was pressed, result is empty
    if (!result.empty()) {
        printf("You selected: %s\n", result.c_str());
    } else {
        printf("No selection.\n");
    }

    return 0;
}

