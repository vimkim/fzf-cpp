#include "fzf.hpp"

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
