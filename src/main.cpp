#include <ncurses.h>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

bool fuzzyMatch(const std::string &str, const std::string &pattern) {
    if (pattern.empty()) return true;
    
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

// Helper function to create a display-friendly version of a string
std::string createDisplayString(const std::string &input, int maxWidth) {
    std::string result = input;
    std::replace(result.begin(), result.end(), '\n', ' ');
    
    // Remove consecutive spaces that might result from newline replacements
    auto newEnd = std::unique(result.begin(), result.end(),
                            [](char a, char b) { return a == ' ' && b == ' '; });
    result.erase(newEnd, result.end());
    
    // Truncate the string if it's longer than maxWidth
    if (result.length() > maxWidth - 3) { // -3 to make room for "..."
        result = result.substr(0, maxWidth - 3) + "...";
    }
    
    return result;
}

void displayItems(const std::vector<std::string> &items, int startIndex, int selectedIndex, int maxLines, int maxWidth) {
    // Clear the region after the search input line
    for (int i = 1; i < maxLines; i++) {
        move(i, 0);
        clrtoeol();
    }

    // Print items line by line, starting from row=1
    int row = 1;
    for (int i = 0; i < (int)items.size(); i++) {
        move(row, 0);
        
        // Create a display-friendly version of the string with width limitation
        std::string displayStr = createDisplayString(items[i], maxWidth);

        if (i == selectedIndex) {
            attron(A_REVERSE);
            printw("%s", displayStr.c_str());
            attroff(A_REVERSE);
        } else {
            printw("%s", displayStr.c_str());
        }
        row++;
        if (row >= maxLines) break;
    }
}

std::string fzf(const std::vector<std::string> &items) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int height, width;
    getmaxyx(stdscr, height, width);

    std::string query;
    int selectedIndex = 0;
    std::vector<std::string> filteredItems = items;

    while (true) {
        move(0, 0);
        clrtoeol();
        printw("Search: %s", query.c_str());

        filteredItems.clear();
        for (const auto &item : items) {
            if (fuzzyMatch(item, query)) {
                filteredItems.push_back(item);
            }
        }

        if (!filteredItems.empty()) {
            if (selectedIndex >= (int)filteredItems.size()) {
                selectedIndex = filteredItems.size() - 1;
            }
        } else {
            selectedIndex = 0;
        }

        displayItems(filteredItems, 0, selectedIndex, height, width);

        int ch = getch();
        if (ch == KEY_UP) {
            if (selectedIndex > 0) {
                selectedIndex--;
            }
        } else if (ch == KEY_DOWN) {
            if (selectedIndex < (int)filteredItems.size() - 1) {
                selectedIndex++;
            }
        } else if (ch == 10) { // Enter key
            if (!filteredItems.empty()) {
                endwin(); 
                return filteredItems[selectedIndex];
            }
        } else if (ch == 27) { // ESC
            endwin();
            return "";
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (!query.empty()) {
                query.pop_back();
            }
        } else if (std::isprint(ch)) {
            query.push_back(static_cast<char>(ch));
        }
    }
}

int main() {
    std::vector<std::string> sqlQueries = {
        "SELECT * FROM users WHERE status = 'active'",
        
        "SELECT \n"
        "    u.id,\n"
        "    u.name,\n"
        "    u.email,\n"
        "    COUNT(o.id) as total_orders\n"
        "FROM users u\n"
        "LEFT JOIN orders o ON u.id = o.user_id\n"
        "GROUP BY u.id, u.name, u.email\n"
        "HAVING COUNT(o.id) > 5",

        "INSERT INTO orders (\n"
        "    user_id,\n"
        "    product_id,\n"
        "    quantity,\n"
        "    price_per_unit\n"
        ") VALUES (\n"
        "    1,\n"
        "    100,\n"
        "    5,\n"
        "    29.99\n"
        ")",

        "WITH monthly_sales AS (\n"
        "    SELECT\n"
        "        DATE_TRUNC('month', created_at) as sale_month,\n"
        "        SUM(total_amount) as revenue\n"
        "    FROM orders\n"
        "    WHERE status = 'completed'\n"
        "    GROUP BY DATE_TRUNC('month', created_at)\n"
        ")\n"
        "SELECT\n"
        "    sale_month,\n"
        "    revenue,\n"
        "    LAG(revenue) OVER (ORDER BY sale_month) as prev_month_revenue\n"
        "FROM monthly_sales\n"
        "ORDER BY sale_month DESC",

        "CREATE TABLE products (\n"
        "    id SERIAL PRIMARY KEY,\n"
        "    name VARCHAR(100) NOT NULL,\n"
        "    description TEXT,\n"
        "    price DECIMAL(10,2) NOT NULL,\n"
        "    category_id INTEGER REFERENCES categories(id),\n"
        "    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,\n"
        "    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP\n"
        ")",

        "UPDATE inventory\n"
        "SET \n"
        "    quantity = quantity - oi.order_quantity,\n"
        "    last_updated = CURRENT_TIMESTAMP\n"
        "FROM order_items oi\n"
        "WHERE inventory.product_id = oi.product_id\n"
        "AND oi.order_id = 12345"
    };
    
    std::string result = fzf(sqlQueries);
    if (!result.empty()) {
        printf("You selected: %s\n", result.c_str());
    } else {
        printf("No selection.\n");
    }
    return 0;
}
