# 💾 R-Type Database Documentation

## 🧭 Overview

The R-Type project uses **SQLite3** as an embedded database to manage player accounts, authentication, and gameplay statistics.  
SQLite was chosen because it is:
- **Lightweight and self-contained** (no external server)
- **Cross-platform** (works perfectly on Windows & Linux)
- **Fast** for local read/write operations
- **Easy to integrate** via **vcpkg**
- **Reliable and persistent** even in offline environments

This design ensures the game remains **fully deployable**, even in **low-connectivity or resource-limited setups**, which is especially important in many African development contexts.

---

## 🏗️ Database Initialization

At server startup, the application checks whether the database file exists.  
If not, it creates a new one and executes the following SQL statement to create the `users` table:

```cpp
const char* createTableSQL = R"(
    CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT UNIQUE NOT NULL,
        password_hash TEXT NOT NULL,
        auth_token TEXT,
        session_token TEXT,
        last_login INTEGER,
        client_hash TEXT,
        nb_games_played INTEGER DEFAULT 0,
        nb_games_won INTEGER DEFAULT 0
    );
)";
